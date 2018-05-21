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
#include "sync_and_strobe_impl.h"

namespace gr {
  namespace capture_tools {

    sync_and_strobe::sptr
    sync_and_strobe::make(float sps, int npreamb)
    {
      return gnuradio::get_initial_sptr
        (new sync_and_strobe_impl(sps, npreamb));
    }

    /*
     * The private constructor
     */
    sync_and_strobe_impl::sync_and_strobe_impl(float sps, int npreamb)
      : gr::sync_block("sync_and_strobe",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(0, 1, sizeof(float)))
    {
        d_sps = sps;
        d_npreamb = npreamb;
        d_navg = (int)(sps*6);
        d_avg_buffer = new float[d_navg];
        memset(d_avg_buffer, 0, d_navg * sizeof(float));
        d_avg_index = 0;
        d_avg_sum = 0;
        d_symbol_cnt = 0;
        d_residue = 0;
        d_hysteresis = 0;
        d_last_crossing_cnt = -1;
        d_direction = 0;
        d_spsmargin = 7;
        d_crossings = 0;
        d_preamble_timeout = 50;
        d_timeout_cnt = 0;
        d_state = 0;
        d_strobe_offset = 7;
        sps_update();
    }

    /*
     * Our virtual destructor.
     */
    sync_and_strobe_impl::~sync_and_strobe_impl()
    {
        delete(d_avg_buffer);
    }

    void sync_and_strobe_impl::sps_update()
    {
        float effective_sps = d_sps + d_residue;
        float decimal = effective_sps - floor(effective_sps);
        if (decimal >= 0.5) {
            d_spsint = ceil(effective_sps);
            d_residue = -(1 - decimal);
        }
        else {
            d_spsint = floor(effective_sps);
            d_residue = decimal;
        }
    }

    int
    sync_and_strobe_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      float avg_val;


      for(int i=0;i<noutput_items;i++) {
        //int subtractindex = d_avg_index < (d_navg-1) ? d_avg_index + 1 
        d_avg_sum -= d_avg_buffer[d_avg_index];
        d_avg_buffer[d_avg_index] = in[i];
        d_avg_sum += d_avg_buffer[d_avg_index];
        d_avg_index++;
        if(d_avg_index == d_navg) d_avg_index = 0;

        avg_val = d_avg_sum / ((float)d_navg);

        if(d_state == 0) {
            bool found_crossing = false;
            if(d_direction == 0) {
                d_direction = in[i] >= avg_val ? 1 : -1;
            }
            else if (d_direction == 1) {
                if (in[i] < (avg_val - d_hysteresis)) {
                    found_crossing = true;
                    d_direction = -1;
                }
            }
            else if (d_direction == -1) {
                if (in[i] > (avg_val + d_hysteresis)) {
                    found_crossing = true;
                    d_direction = 1;
                }
            }

            if (d_last_crossing_cnt != -1) {
                d_last_crossing_cnt ++;
            }
            if (found_crossing) {
                if(d_last_crossing_cnt > (d_sps - d_spsmargin) && d_last_crossing_cnt < (d_sps + d_spsmargin)) {
                    d_crossings++;
                }
                else {
                    d_crossings = 0;
                }
                d_last_crossing_cnt = 0;
            }

            if(d_crossings == d_npreamb) {
                d_state = 1;
                d_crossings = 0 ;
                d_last_crossing_cnt = 0 ;
                d_symbol_cnt = 0;
                d_residue = 0;
                d_timeout_cnt = 0;
                sps_update();
                add_item_tag(0, nitems_written(0) + i, pmt::intern("preamble"), pmt::intern(""), pmt::intern(""));
            }
        }
        else if (d_state == 1) {
            if(d_symbol_cnt == d_strobe_offset) {
                add_item_tag(0, nitems_written(0) + i, pmt::intern("strobe"), pmt::intern(""), pmt::intern(""));
            }

            d_symbol_cnt++;
            if(d_symbol_cnt == d_spsint) {
                d_symbol_cnt = 0;
                sps_update();

                d_timeout_cnt++;
                if(d_timeout_cnt == d_preamble_timeout) {
                    d_direction = 0;
                    d_last_crossing_cnt = -1;
                    d_state = 0;
                }
            }
        }
      }


      memcpy(out, in, noutput_items*sizeof(float));

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

