/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_CAPTURE_TOOLS_MSG_TO_GFSK_C_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_MSG_TO_GFSK_C_IMPL_H

#include <queue>
#include <deque>
#include <vector>
#include <capture_tools/msg_to_gfsk_c.h>
#include <gnuradio/filter/fir_filter.h>

namespace gr {
  namespace capture_tools {

    class msg_to_gfsk_c_impl : public msg_to_gfsk_c
    {
     private:
      int d_samples_per_symbol;
      float d_sensitivity;
      float d_bt;
      int d_fixed_delay;
      int d_delay_counter;
      bool d_ready_to_transmit;
      std::vector<char> *d_current_packet;
      int d_bitcounter;
      int d_upsample_counter;
      int d_wait_settle;
      int d_rampup_delay;
      int d_rampdown_delay;
      int d_max_queue_length;
      int d_state;
      bool d_tag_set;
      int d_filt_input_cnt;
      float d_rampup_alpha;
      float d_rampdown_alpha;
      float d_phase;
      float d_value_between;
      int d_fixed_delay_range;
      int d_current_delay;
      bool d_reset_phase;
      bool d_repeat_packet;
      float d_level_off;
      bool d_rampup_tag_set;
      bool d_randomize_phase;
      float d_sensitivity_range;
      float d_current_sensitivity;
      int d_packets_requested;

      boost::mutex common_mutex;

      std::vector<float> d_filter_taps;
      std::deque<std::vector<char>*> d_packets;
      float *d_filt_input;

      gr::filter::kernel::fir_filter_fff *d_fir;

      void add_packet_to_queue(pmt::pmt_t msg);
      void set_current_delay();
      void unset_current_packet();
      void init_filter();

     public:
      msg_to_gfsk_c_impl(int samples_per_symbol, float sensitivity, float bt);
      ~msg_to_gfsk_c_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

      void set_fixed_delay(int);
      void set_fixed_delay_range(int);
      void set_wait_settle(int);
      void set_rampup_delay(int);
      void set_rampdown_delay(int);
      void set_max_queue_length(int);
      void set_rampup_alpha(float);
      void set_rampdown_alpha(float);
      void set_value_between(float);
      void set_reset_phase(bool);
      void set_repeat_packet(bool);
      void set_level_off(float);
      void set_sensitivity(float);
      void set_bt(float);
      void set_randomize_phase(bool);
      void set_sensitivity_range(float);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_MSG_TO_GFSK_C_IMPL_H */

