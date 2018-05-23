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
              gr::io_signature::make(1, 2, sizeof(float)),
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
        d_min_swing = 0.005;
        d_extreme_val_high = 0;
        d_extreme_val_low = 0;
        d_input_buffer = 0ull;
        d_start_counter = 0;
        d_sync_word_len = 0;
        d_packet_counter = 0;
        d_n_to_catch = 400;
		d_decim_in_front = 0;
        d_mag_at_addressmatch = 0;
        d_current_burst_frequency_mhz = 0;
        d_current_burst_magnitude     = 0;
        d_current_burst_id            = 0;
		d_current_burst_sample_rate   = 0;
		d_cnt_since_burst_start       = 0;
        sps_update();

        message_port_register_out(pmt::mp("packets"));
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
        d_sync_word = 0ull;
        d_sync_word_mask = 0ull;
        int swsize = s.size();
        if (swsize > 64) {
            throw std::runtime_error("Sync word can be max 64 bits long");
        }
        for(int i=0; i<swsize ;i++) {
            d_sync_word = ((d_sync_word << 1) & 0xfffffffffffffffeull) | (uint64_t)(s[i] & 0x01);
            d_sync_word_mask |= (((uint64_t)1) << i);
        }
        d_sync_word_len = swsize;
        printf("d_sync_word: 0x%016llx, d_sync_word_mask: 0x%016llx, d_sync_word_len: %d\n",d_sync_word, d_sync_word_mask, d_sync_word_len);
    }

    void
    sync_and_strobe_impl::set_strobe_offset(int val)
    {
        d_strobe_offset = val;
    }

    void
    sync_and_strobe_impl::set_preamble_timeout(int val)
    {
        d_preamble_timeout = val;
    }

    void
    sync_and_strobe_impl::set_n_to_catch(int val)
    {
        d_n_to_catch = val;
    }

    void
    sync_and_strobe_impl::set_spsmargin(float val)
    {
        d_spsmargin = val;
    }

    void
    sync_and_strobe_impl::set_hysteresis(float val)
    {
        d_hysteresis = val;
    }

    void
    sync_and_strobe_impl::set_min_swing(float val)
    {
        d_min_swing = val;
    }

    void
    sync_and_strobe_impl::set_decim_in_front(int val)
    {
        d_decim_in_front = val;
    }

    int
    sync_and_strobe_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      const float *in_mag = NULL;
      float *out = (float *) output_items[0];
      float *out_avg = NULL;

      if (output_items.size() >= 2)
        out_avg = (float *) output_items[1];
      if (input_items.size() >= 2)
        in_mag = (const float *) input_items[1];

      float avg_val;
      int nstate;

      std::vector<int> tag_positions;
      int next_tag_position = -1;
      int next_tag_position_index = -1;

      std::vector<tag_t> new_bursts;
      get_tags_in_window(new_bursts, 0, 0, noutput_items, pmt::mp("new_burst"));
      for(int i=0;i<new_bursts.size();i++) {
          int current;
          current = new_bursts[i].offset - nitems_read(0);
          tag_positions.push_back(current);
          next_tag_position_index = 0;
      }
      if(next_tag_position_index >= 0) {
          next_tag_position = tag_positions[next_tag_position_index];
      }

      for(int i=0;i<noutput_items;i++) {
        if (next_tag_position == i) {
          tag_t current = new_bursts[next_tag_position_index];
          float burst_frequency_mhz = pmt::to_float(pmt::dict_ref(current.value, pmt::mp("burst_frequency_mhz"), pmt::PMT_NIL));
          float burst_magnitude = pmt::to_float(pmt::dict_ref(current.value, pmt::mp("magnitude"), pmt::PMT_NIL));
          uint64_t burst_id = pmt::to_uint64(pmt::dict_ref(current.value, pmt::mp("id"), pmt::PMT_NIL));
          uint64_t burst_offset = pmt::to_uint64(pmt::dict_ref(current.value, pmt::mp("offset"), pmt::PMT_NIL));
          float burst_sample_rate = pmt::to_float(pmt::dict_ref(current.value, pmt::mp("sample_rate"), pmt::PMT_NIL));
          d_current_burst_frequency_mhz = burst_frequency_mhz;
          d_current_burst_magnitude = burst_magnitude;
          d_current_burst_id        = burst_id;
          d_current_burst_offset    = burst_offset;
          d_current_burst_sample_rate = burst_sample_rate;
		  d_cnt_since_burst_start = 0;

          next_tag_position_index++;
          if (next_tag_position_index == tag_positions.size()) {
            next_tag_position_index = -1;
            next_tag_position = -1;
          }
          else {
            next_tag_position = tag_positions[next_tag_position_index];
          }
        }
		else {
		  d_cnt_since_burst_start++;
		}

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
            bool wide_enough = (d_extreme_val_high - d_extreme_val_low) > d_min_swing;
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

                    if (nstate != d_state) {
                        uint8_t sliced;
                        d_start_counter = d_npreamb;
                        bool odd = (d_npreamb % 2) != 0;
                        if((d_direction == 1 && odd) || (d_direction == -1 && !odd))
                            sliced = 0;
                        else
                            sliced = 1;
                        add_item_tag(0, nitems_written(0) + i, pmt::intern("sliced"), pmt::from_bool(sliced), pmt::intern(""));
                        for (int j=0;j<d_npreamb;j++) {
                            d_input_buffer = ((d_input_buffer << 1) & 0xfffffffffffffffeull) | ((uint64_t)(sliced & 0x01));
                            sliced = (sliced == 1) ? 0 : 1;
                        }
                    }
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
                d_input_buffer = ((d_input_buffer << 1) & 0xfffffffffffffffeull) | ((uint64_t)(sliced & 0x01));
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
						d_cnt_at_addressmatch = d_cnt_since_burst_start;
                        if (in_mag != NULL) {
                            d_mag_at_addressmatch = in_mag[i];
                        }
                    }
                }
                else if (d_state == 2){
                    add_item_tag(0, nitems_written(0) + i, pmt::intern("strobe"), pmt::from_long(d_packet_counter), pmt::intern(""));
                    d_receive_buffer.push_back(sliced);
                    d_packet_counter ++;
                    if(d_packet_counter >= d_n_to_catch || in_mag[i] < 0.4*d_mag_at_addressmatch) {
                        d_direction = 0;
                        d_last_crossing_cnt = -1;
                        d_crossings = 0 ;
                        nstate = 0;
                        if (in_mag[i] < 0.4*d_mag_at_addressmatch)
                            add_item_tag(0, nitems_written(0) + i, pmt::intern("ramp_down"), pmt::intern(""), pmt::intern(""));

                        pmt::pmt_t pdu_meta = pmt::make_dict();
                        pmt::pmt_t pdu_vector = pmt::init_u8vector(d_receive_buffer.size(), d_receive_buffer);
                        pdu_meta = pmt::dict_add(pdu_meta, pmt::mp("freq"), pmt::from_float(d_current_burst_frequency_mhz));
                        pdu_meta = pmt::dict_add(pdu_meta, pmt::mp("magnitude_burst"), pmt::from_float(d_current_burst_magnitude));
                        pdu_meta = pmt::dict_add(pdu_meta, pmt::mp("id"), pmt::from_uint64(d_current_burst_id));
                        pdu_meta = pmt::dict_add(pdu_meta, pmt::mp("offset_addressmatch"), pmt::from_uint64(d_current_burst_offset + d_cnt_at_addressmatch*d_decim_in_front));
                        pdu_meta = pmt::dict_add(pdu_meta, pmt::mp("sample_rate"), pmt::from_float(d_current_burst_sample_rate));
                        pdu_meta = pmt::dict_add(pdu_meta, pmt::mp("magnitude"), pmt::from_float(20*log10(d_mag_at_addressmatch)));
                        pmt::pmt_t out_msg = pmt::cons(pdu_meta, pdu_vector);
                        message_port_pub(pmt::mp("packets"), out_msg);

                        d_start_counter = 0;
                    }
                }

              if(d_start_counter < 64)
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

