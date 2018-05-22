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
              gr::io_signature::make(0, 2, sizeof(float)))
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
        d_min_swing = 0.1;
        d_extreme_val_high = 0;
        d_extreme_val_low = 0;
        d_input_buffer = 0;
        d_start_counter = 0;
        d_sync_word_len = 0;
        d_packet_counter = 0;
        d_n_to_catch = 400;
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

    void
    sync_and_strobe_impl::set_sync_word(const std::vector<uint8_t> s)
    {
        d_sync_word = 0;
        d_sync_word_mask = 0;
        int swsize = s.size();
        if (swsize > 32) {
            throw std::runtime_error("Sync word can be max 32 bits long");
        }
        for(int i=0; i<swsize ;i++) {
            d_sync_word = ((d_sync_word << 1) & 0xfffffffe) | (uint32_t)(s[i] & 0x01);
            d_sync_word_mask |= (((uint32_t)1) << i);
        }
        d_sync_word_len = swsize;
        printf("d_sync_word: 0x%08x, d_sync_word_mask: 0x%08x, d_sync_word_len: %d\n",d_sync_word, d_sync_word_mask, d_sync_word_len);
    }

    int
    sync_and_strobe_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];
      float *out_avg = NULL;

      if (output_items.size() >= 2)
        out_avg = (float *) output_items[1];

      float avg_val;
      int nstate;


      for(int i=0;i<noutput_items;i++) {
        nstate = d_state;
        //int subtractindex = d_avg_index < (d_navg-1) ? d_avg_index + 1 
        d_avg_sum -= d_avg_buffer[d_avg_index];
        d_avg_buffer[d_avg_index] = in[i];
        d_avg_sum += d_avg_buffer[d_avg_index];
        d_avg_index++;
        if(d_avg_index == d_navg) d_avg_index = 0;

        avg_val = d_avg_sum / ((float)d_navg);

        if(d_state == 0 || d_state == 1) {
            bool found_crossing = false;
            if(d_direction == 0) {
                d_direction = in[i] >= avg_val ? 1 : -1;
            }
            else if (d_direction == 1) {
                if (in[i] < (avg_val - d_hysteresis)) {
                    found_crossing = true;
                    d_direction = -1;
                    d_extreme_val_low = 0;
                }
                else {
                    d_extreme_val_high = in[i] > d_extreme_val_high ? in[i] : d_extreme_val_high;
                }
            }
            else if (d_direction == -1) {
                if (in[i] > (avg_val + d_hysteresis)) {
                    found_crossing = true;
                    d_direction = 1;
                    d_extreme_val_high = 0;
                }
                else {
                    d_extreme_val_low = in[i] < d_extreme_val_low ? in[i] : d_extreme_val_low;
                }
            }

            if (d_last_crossing_cnt != -1) {
                d_last_crossing_cnt ++;
            }
            bool wide_enough = (d_extreme_val_high - d_extreme_val_low) > d_min_swing;
            if (found_crossing && wide_enough) {
                if(d_last_crossing_cnt > (d_sps - d_spsmargin) && d_last_crossing_cnt < (d_sps + d_spsmargin)) {
                    //add_item_tag(0, nitems_written(0) + i, pmt::intern("correct_crossing"), pmt::intern("yes"), pmt::intern(""));
                    d_crossings++;
                    //printf("d_crossings: %d, d_last_crossing_cnt: %d\n" , d_crossings, d_last_crossing_cnt);
                }
                else {
                    //add_item_tag(0, nitems_written(0) + i, pmt::intern("correct_crossing"), pmt::intern("no"), pmt::intern(""));
                    d_crossings = 0;
                }
                d_last_crossing_cnt = 0;

                if(d_crossings >= d_npreamb) {
                    nstate = 1;
                    d_symbol_cnt = 0;
                    d_residue = 0;
                    d_timeout_cnt = 0;
                    d_frozen_avg_val = avg_val;
                    sps_update();
                    add_item_tag(0, nitems_written(0) + i, pmt::intern("preamble"), pmt::intern(""), pmt::intern(""));
                    //printf("\n");
                }
            }
        }
        if (d_state == 1 || d_state == 2) {
            if(d_symbol_cnt == d_strobe_offset) {
                uint8_t sliced;

                if(d_direction == 1) {
                    if (in[i] < (d_frozen_avg_val - d_hysteresis)) {
                        sliced = 0;
                        d_direction = -1;
                    }
                    else sliced = 1; 
                }
                else if(d_direction == -1) {
                    if (in[i] > (d_frozen_avg_val + d_hysteresis)) {
                        sliced = 1;
                        d_direction = 1;
                    }
                    else sliced = 0; 
                }
                d_input_buffer = ((d_input_buffer << 1) & 0xfffffffe) | ((uint32_t)(sliced & 0x01));
                if (d_state == 1) {
                    if(d_sync_word_len > 0 && d_start_counter >= d_sync_word_len && (d_input_buffer & d_sync_word_mask) == d_sync_word) {
                        add_item_tag(0, nitems_written(0) + i, pmt::intern("address"), pmt::intern(""), pmt::intern(""));
                        nstate = 2;
                        d_timeout_cnt = 0;
                        d_packet_counter = 0;
                        d_receive_buffer.clear();
                        for(int i=0;i<d_sync_word_len;i++) {
                            d_receive_buffer.push_back((d_sync_word >> (d_sync_word_len-i-1)) & 0x1);
                        }
                    }
                }
                else if (d_state == 2){
                    add_item_tag(0, nitems_written(0) + i, pmt::intern("strobe"), pmt::from_long(d_packet_counter), pmt::intern(""));
                    d_receive_buffer.push_back(sliced);
                    d_packet_counter ++;
                    if(d_packet_counter >= d_n_to_catch) {
                        d_direction = 0;
                        d_last_crossing_cnt = -1;
                        d_crossings = 0 ;
                        nstate = 0;
                        d_start_counter = 0;
                    }
                }

              if(d_start_counter < 32)
                  d_start_counter++;

            }

            d_symbol_cnt++;
            if(d_symbol_cnt == d_spsint) {
                d_symbol_cnt = 0;
                sps_update();

                d_timeout_cnt++;
                if(d_state == 1 && d_timeout_cnt == d_preamble_timeout) {
                    d_direction = 0;
                    d_last_crossing_cnt = -1;
                    d_crossings = 0 ;
                    nstate = 0;
                    d_start_counter = 0;
                }
            }
        }
        if (out_avg != NULL)
            out_avg[i] = avg_val;
        d_state = nstate;
      }


      memcpy(out, in, noutput_items*sizeof(float));

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

