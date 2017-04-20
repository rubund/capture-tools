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

#ifndef INCLUDED_CAPTURE_TOOLS_REPEAT_INPUT_N_TIMES_CC_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_REPEAT_INPUT_N_TIMES_CC_IMPL_H

#include <capture_tools/repeat_input_n_times_cc.h>

namespace gr {
  namespace capture_tools {

    class repeat_input_n_times_cc_impl : public repeat_input_n_times_cc
    {
     private:
      // Nothing to declare in this block.
        gr_complex *d_memory;
        int d_memory_cnt;
        int d_max_samples;
        int d_n_times;
        int d_state;
        int d_playback_cnt;
        int d_playback_time;
        int d_fromtag;

        std::vector<tag_t> d_all_tags;

     public:
      repeat_input_n_times_cc_impl(int n_times, int max_samples);
      ~repeat_input_n_times_cc_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_REPEAT_INPUT_N_TIMES_CC_IMPL_H */

