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

#include <gnuradio/io_signature.h>
#include "xrun_monitor_ff_impl.h"

namespace gr {
  namespace capture_tools {

    xrun_monitor_ff::sptr
    xrun_monitor_ff::make()
    {
      return gnuradio::get_initial_sptr
        (new xrun_monitor_ff_impl());
    }

    /*
     * The private constructor
     */
    xrun_monitor_ff_impl::xrun_monitor_ff_impl()
      : gr::block("xrun_monitor_ff",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
        d_n = 0;
        d_produce_per = 100;
        d_write_index = 0;
        d_read_index = 0;
        d_length = 1000000;
        d_buffer = new float[d_length];
        d_first = true;
        d_starting = true;
    }

    /*
     * Our virtual destructor.
     */
    xrun_monitor_ff_impl::~xrun_monitor_ff_impl()
    {
        delete(d_buffer);
    }

    void
    xrun_monitor_ff_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        if (d_first) {
            ninput_items_required[0] = d_length / 2;
            d_first = false;
        }
        else
            ninput_items_required[0] = d_produce_per; //noutput_items;

    }

    int
    xrun_monitor_ff_impl::general_work(int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      // Do <+signal processing+>
      int current_fill;
      if (d_read_index < d_write_index)
        current_fill = d_write_index - d_read_index;
      else if (d_read_index == d_write_index)
        current_fill = 0;
      else {
        current_fill = d_length - d_read_index + d_write_index;
      }
      float fill_percentage;
      fill_percentage = (((float)current_fill)/((float)d_length))*100;
      if (d_starting && fill_percentage < 50) {
        noutput_items = 0;
      }
      else {
        d_starting = false;
      }


      int produced = 0;
      int consumed = 0;
      int remaining;
      int to_produce_here;
      int toread;
      int outpos = 0;

      if (d_read_index < d_write_index) {
        toread = d_write_index - d_read_index;
        to_produce_here = std::min(toread, noutput_items);
        memcpy(out, d_buffer + d_read_index, sizeof(float) * to_produce_here);
        d_read_index += to_produce_here;
        remaining = noutput_items - to_produce_here;
        outpos = to_produce_here;
        produced += to_produce_here;
      }
      else if (d_read_index == d_write_index){
        remaining = noutput_items;
        outpos = 0;
      }
      else {
        toread = d_length - d_read_index;
        to_produce_here = std::min(toread, noutput_items);
        memcpy(out, d_buffer + d_read_index, sizeof(float) * to_produce_here);
        remaining = noutput_items - to_produce_here;
        produced += to_produce_here;
        if (remaining > 0) {
            int to_produce_here2 = std::min(d_write_index, remaining);
            memcpy(out+to_produce_here, d_buffer, sizeof(float) * to_produce_here2);
            d_read_index = to_produce_here2;
            remaining = remaining - to_produce_here2;
            outpos = to_produce_here + to_produce_here2;
            produced += to_produce_here2;
        }
        else if (remaining == 0) {
            if ((d_read_index + to_produce_here ) < d_length)
                d_read_index += to_produce_here;
            else
                d_read_index = 0;
            outpos = to_produce_here;
        }
        else {
            outpos = to_produce_here;
            d_read_index += to_produce_here;
        }
      }

      int to_produce = std::min(ninput_items[0], remaining);
      memcpy(out+outpos, in, sizeof(float) * to_produce);
      produced += to_produce;
      consumed += to_produce;
      //if ((to_produce_here + to_produce) <= d_length) {
      //}
      //else {
      //    memcpy(out+to_produce_here, in, sizeof(float) * (d_length - (to_produce_here + to_produce)));
      //    memcpy(out+to_produce_here, in, sizeof(float) * (d_length - (to_produce_here + to_produce)));
      //}
      int tosave = ninput_items[0] - (to_produce);

      if (d_read_index < d_write_index)
        current_fill = d_write_index - d_read_index;
      else if (d_read_index == d_write_index)
        current_fill = 0;
      else {
        current_fill = d_length - d_read_index + d_write_index;
      }

      if(tosave >= (d_length-current_fill)) {
        printf("Buffer full!\n");
        tosave = d_length - current_fill - 1;
      }

      if ((d_write_index + tosave) < d_length) {
        memcpy(d_buffer + d_write_index, in + to_produce, sizeof(float) * tosave);
        d_write_index = d_write_index + tosave;
      }
      else {
        memcpy(d_buffer + d_write_index, in + to_produce, sizeof(float) * (d_length - d_write_index));
        memcpy(d_buffer, in + to_produce + (d_length - d_write_index), sizeof(float) * (tosave - (d_length - d_write_index)));
        d_write_index = tosave - (d_length - d_write_index);
      }
      consumed += tosave;

      d_n += noutput_items;

        if (d_n > 100000) {
            printf("ninput_items: %d, noutput_items: %d\n", ninput_items[0], noutput_items);
            d_n = 0;
        }

      if (d_read_index < d_write_index)
        current_fill = d_write_index - d_read_index;
      else if (d_read_index == d_write_index)
        current_fill = 0;
      else {
        current_fill = d_length - d_read_index + d_write_index;
      }

      consume_each(consumed);
      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace capture_tools */
} /* namespace gr */

