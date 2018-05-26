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

#ifndef INCLUDED_CAPTURE_TOOLS_SYNC_AND_STROBE_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_SYNC_AND_STROBE_IMPL_H

#include <capture_tools/sync_and_strobe.h>

namespace gr {
  namespace capture_tools {

    class sync_and_strobe_impl : public sync_and_strobe
    {
     private:
      // Nothing to declare in this block.
        float * d_avg_buffer;
        int d_navg;
        int d_avg_index;
        float d_avg_sum;
        int d_symbol_cnt;
        float d_residue;
        float d_sps;
        int d_spsint;
        float d_hysteresis;
        int d_last_crossing_cnt;
        int d_direction;
        int d_direction_preamble;
        float d_spsmargin;
        int d_crossings;
        int d_preamble_timeout;
        int d_timeout_cnt;
        int d_state;
        int d_npreamb;
        int d_strobe_offset;
        float d_min_swing;
        float d_extreme_val_high;
        float d_extreme_val_low;
        float d_frozen_avg_val;
        uint64_t d_input_buffer;
		int d_decim_in_front;

        uint64_t d_sync_word;
        uint64_t d_sync_word_mask;
        int d_sync_word_len;
        int d_start_counter;
        int d_packet_counter;
        std::vector<uint8_t> d_receive_buffer;
        int d_n_to_catch;

        float d_current_burst_frequency_mhz;
        float d_current_burst_magnitude;
        uint64_t d_current_burst_id;
        uint64_t d_current_burst_offset;
		uint64_t d_cnt_since_burst_start;
		uint64_t d_cnt_at_addressmatch;
        float d_mag_at_addressmatch;
		float d_current_burst_sample_rate;

        void sps_update();

     public:
      sync_and_strobe_impl(float sps, int npreamb);
      ~sync_and_strobe_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

      void set_sync_word(const std::vector<uint8_t> s);
      void set_strobe_offset(int val);
      void set_preamble_timeout(int val);
      void set_n_to_catch(int val);
      void set_spsmargin(float val);
      void set_hysteresis(float val);
      void set_decim_in_front(int val);
      void set_min_swing(float val);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_SYNC_AND_STROBE_IMPL_H */

