/* -*- c++ -*- */
/*
 * Copyright 2021 gr-capture_tools author.
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

#include <gnuradio/io_signature.h>
#include "file_sink_store_tags_impl.h"

namespace gr {
  namespace capture_tools {

    file_sink_store_tags::sptr
    file_sink_store_tags::make(size_t itemsize, const char* filename, const char* tag_filename, bool append)
    {
      return gnuradio::get_initial_sptr
        (new file_sink_store_tags_impl(itemsize, filename, tag_filename, append));
    }


    /*
     * The private constructor
     */
    file_sink_store_tags_impl::file_sink_store_tags_impl(size_t itemsize, const char* filename, const char* tag_filename, bool append)
      : gr::sync_block("file_sink_store_tags",
              gr::io_signature::make(1, 1, sizeof(itemsize)),
              gr::io_signature::make(0, 0, 0)),
              gr::blocks::file_sink_base(filename, true, append),
              d_itemsize(itemsize)
    {
    }

    /*
     * Our virtual destructor.
     */
    file_sink_store_tags_impl::~file_sink_store_tags_impl()
    {
    }

    int
    file_sink_store_tags_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      char* inbuf = (char*)input_items[0];
      int nwritten = 0;

      do_update(); // update d_fp is reqd

      if (!d_fp)
          return noutput_items; // drop output on the floor

      while (nwritten < noutput_items) {
          int count = fwrite(inbuf, d_itemsize, noutput_items - nwritten, d_fp);
          if (count == 0) {
              if (ferror(d_fp)) {
                  std::stringstream s;
                  s << "file_sink write failed with error " << fileno(d_fp) << std::endl;
                  throw std::runtime_error(s.str());
              } else { // is EOF
                  break;
              }
          }
          nwritten += count;
          inbuf += count * d_itemsize;
      }

      if (d_unbuffered)
          fflush(d_fp);

      return nwritten;
    }

    bool file_sink_store_tags_impl::stop()
    {
        do_update();
        fflush(d_fp);
        return true;
    }

  } /* namespace capture_tools */
} /* namespace gr */

