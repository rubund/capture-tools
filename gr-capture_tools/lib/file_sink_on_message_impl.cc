/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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
#include "file_sink_on_message_impl.h"

namespace gr {
  namespace capture_tools {

    file_sink_on_message::sptr
    file_sink_on_message::make(size_t itemsize, const char *filename, int number_buffered, int number_to_write, gr::msg_queue::sptr queue)
    {
      return gnuradio::get_initial_sptr
        (new file_sink_on_message_impl(itemsize, filename, number_buffered, number_to_write, queue));
    }

    /*
     * The private constructor
     */
    file_sink_on_message_impl::file_sink_on_message_impl(size_t itemsize, const char *filename, int number_buffered, int number_to_write, gr::msg_queue::sptr queue)
      : gr::sync_block("file_sink_on_message",
              gr::io_signature::make(1, 1, 4),
              gr::io_signature::make(0, 0, 0)),
        gr::blocks::file_sink_base(filename, true, 0),
    d_itemsize(4), d_queue(queue), d_number_buffered(number_buffered), d_number_to_write(number_to_write)
    {

       if (!open(filename))
          throw std::runtime_error ("can't open file");
       d_buffered = new float[d_number_buffered];
       for(int i=0;i<d_number_buffered;i++) {
             d_buffered[i] = 0;
       }
       d_index_buffered = 0;
       d_is_writing = 0;
       d_numbers_written = 0;
           printf("%d\n",number_to_write);

    }

    /*
     * Our virtual destructor.
     */
    file_sink_on_message_impl::~file_sink_on_message_impl()
    {
        delete d_buffered;
    }

    int
    file_sink_on_message_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        float *inbuf = (float *) input_items[0];
        int  nread = 0;
        float null_to_write = 0;
        int status;
        gr::message::sptr msg_in;

        do_update();				// update d_fp is reqd

        if (!d_fp)
          return noutput_items;		// drop output on the floor

        while (nread < noutput_items){
          int count = 0;
          if(d_is_writing) {
              count = fwrite (inbuf, d_itemsize, 1, d_fp);
              if (count == 0)	// FIXME add error handling
                break;
              nread += count;
              //inbuf += count * d_itemsize;
              inbuf += count;
              d_numbers_written += count;
              if(d_numbers_written >= d_number_to_write){
                  null_to_write = 0;
                  for(int i=0;i<d_number_to_write;i++) {
                      status = fwrite(&null_to_write,4,1,d_fp);
                      if(status == 0) break;
                  }
                  d_is_writing = 0;
                  d_numbers_written = 0;
                  d_queue->flush();
                  if (d_unbuffered)
                    fflush (d_fp);
              }
          }
          else {
              d_buffered[d_index_buffered] = *inbuf;
              d_index_buffered++;
              if(d_index_buffered >= d_number_buffered-1){
                  d_index_buffered = 0;
              }
              nread += 1;
              inbuf += 1;
              msg_in = d_queue->delete_head_nowait();	
              if(msg_in != 0) {
                  printf("w");
                  fflush(stdout);
                  for(int i=d_index_buffered;i<d_number_buffered;i++){
                      status = fwrite(d_buffered+i,4,1,d_fp);
                      if(status == 0) break;
                  }
                  for(int i=0;i<d_index_buffered;i++){
                      status = fwrite(d_buffered+i,4,1,d_fp);
                      if(status == 0) break;
                  }
                  d_is_writing = 1;
              }
          }
        }



        if (d_unbuffered)
            fflush (d_fp);

        //return nwritten;
        return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

