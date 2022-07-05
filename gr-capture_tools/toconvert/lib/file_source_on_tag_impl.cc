/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/thread/thread.h>
#include <gnuradio/io_signature.h>
#include "file_source_on_tag_impl.h"

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>
#include <stdio.h>

// win32 (mingw/msvc) specific
#ifdef HAVE_IO_H
#include <io.h>
#endif
#ifdef O_BINARY
#define OUR_O_BINARY O_BINARY
#else
#define OUR_O_BINARY 0
#endif
// should be handled via configure
#ifdef O_LARGEFILE
#define OUR_O_LARGEFILE O_LARGEFILE
#else
#define OUR_O_LARGEFILE 0
#endif


namespace gr {
  namespace capture_tools {

    file_source_on_tag::sptr
    file_source_on_tag::make(size_t itemsize, const char *filename, const std::string &tag_str, bool filename_in_message)
    {
      return gnuradio::get_initial_sptr
        (new file_source_on_tag_impl(itemsize, filename, tag_str, filename_in_message));
    }

    /*
     * The private constructor
     */
    file_source_on_tag_impl::file_source_on_tag_impl(size_t itemsize, const char *filename, const std::string &tag_str, bool filename_in_message)
      : gr::sync_block("file_source_on_tag",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, itemsize)),
        //gr::blocks::file_source(filename, true, 0),
        d_itemsize(itemsize), d_is_in_file(false), d_fp(0), d_new_fp(0),
        d_updated(false)
    {
        d_search_tag = pmt::intern(tag_str);
        d_tag_start = pmt::intern("file_begin");
        d_tag_stop = pmt::intern("file_end");
        d_tag_val = pmt::intern("");

        open(filename, true);
        do_update();

    }

    /*
     * Our virtual destructor.
     */
    file_source_on_tag_impl::~file_source_on_tag_impl()
    {
        if(d_fp)
          fclose ((FILE*)d_fp);
        if(d_new_fp)
          fclose ((FILE*)d_new_fp);

    }

    bool
    file_source_on_tag_impl::seek(long seek_point, int whence)
    {
      return fseek((FILE*)d_fp, seek_point *d_itemsize, whence) == 0;
    }


    void
    file_source_on_tag_impl::open(const char *filename, bool repeat)
    {
        // obtain exclusive access for duration of this function
        gr::thread::scoped_lock lock(fp_mutex);

        int fd;

        // we use "open" to use to the O_LARGEFILE flag
        if((fd = ::open(filename, O_RDONLY | OUR_O_LARGEFILE | OUR_O_BINARY)) < 0) {
            perror(filename);
            throw std::runtime_error("can't open file");
        }

        if(d_new_fp) {
            fclose(d_new_fp);
            d_new_fp = 0;
        }

        if((d_new_fp = fdopen (fd, "rb")) == NULL) {
            perror(filename);
            ::close(fd);    // don't leak file descriptor if fdopen fails
            throw std::runtime_error("can't open file");
        }

        d_updated = true;
        d_repeat = repeat;

    }

    void
    file_source_on_tag_impl::close()
    {
        // obtain exclusive access for duration of this function
        gr::thread::scoped_lock lock(fp_mutex);

        if(d_new_fp != NULL) {
            fclose(d_new_fp);
            d_new_fp = NULL;
        }
        d_updated = true;
    }

    void
    file_source_on_tag_impl::do_update()
    {
        if(d_updated) {
            gr::thread::scoped_lock lock(fp_mutex); // hold while in scope

            if(d_fp)
                fclose(d_fp);

            d_fp = d_new_fp;    // install new file pointer
            d_new_fp = 0;
            d_updated = false;
        }
    }


    int
    file_source_on_tag_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        std::vector<tag_t> tags;
        std::vector<int> tag_positions;
        int next_tag_position = -1;
        int next_tag_position_index = -1;
        bool start_now = false;

        float *in = (float*)input_items[0];
        char *o = (char*)output_items[0];
        int i;
        int size = noutput_items;
        int pos = 0;


        if(!d_is_in_file) {

            get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items, d_search_tag);
            for(int i=0;i<tags.size();i++) {
                int current;
                current = tags[i].offset - nitems_read(0);
                tag_positions.push_back(current);
                next_tag_position_index = 0;
            }
            if(next_tag_position_index >= 0) {
                next_tag_position = tag_positions[next_tag_position_index];
            }

            if (next_tag_position != -1) {
                size -= next_tag_position;
                pos += next_tag_position;
                memset(o, 0, d_itemsize*next_tag_position);
                o += d_itemsize * next_tag_position;

                next_tag_position_index++;
                if (next_tag_position_index == tag_positions.size()) {
                  next_tag_position_index = -1;
                  next_tag_position = -1;
                }
                else {
                  next_tag_position = tag_positions[next_tag_position_index];
                }
                d_is_in_file = true;
                add_item_tag(0, nitems_written(0)+pos, d_tag_start, d_tag_val);
                do_update();       // update d_fp is reqd
            }
            else {
                memset(o, 0, d_itemsize*noutput_items);
                o += d_itemsize * noutput_items;
                size = 0;
                pos = noutput_items;
            }
        }

        if(d_fp == NULL)
            throw std::runtime_error("work with file not open");

        gr::thread::scoped_lock lock(fp_mutex); // hold for the rest of this function
        while(size) {
            i = fread(o, d_itemsize, size, (FILE*)d_fp);

            size -= i;
            o += i * d_itemsize;
            pos += i;

            if(size == 0)       // done
                break;

            if(i > 0)           // short read, try again
                continue;

            // We got a zero from fread.  This is either EOF or error.  In
            // any event, if we're in repeat mode, seek back to the beginning
            // of the file and try again, else break
            if(!d_repeat)
                break;

            if(pos > 0)
                add_item_tag(0, nitems_written(0)+pos-1, d_tag_stop, d_tag_val);
            else
                add_item_tag(0, nitems_written(0)+pos, d_tag_stop, d_tag_val);

            if(fseek ((FILE *) d_fp, 0, SEEK_SET) == -1) {
                fprintf(stderr, "[%s] fseek failed\n", __FILE__);
                exit(-1);
            }
            d_is_in_file = false;
            break;
        }

        if(size > 0) {              // EOF or error
            if(size == noutput_items)       // we didn't read anything; say we're done
                return -1;
            return noutput_items - size;    // else return partial result
        }

        return noutput_items;

    }

  } /* namespace capture_tools */
} /* namespace gr */

