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

#ifndef INCLUDED_CAPTURE_TOOLS_FFT_BURST_TAGGER_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_FFT_BURST_TAGGER_IMPL_H

#include <capture_tools/fft_burst_tagger.h>

namespace gr {
  namespace capture_tools {

    class fft_burst_tagger_impl : public fft_burst_tagger
    {
     private:
      // Nothing to declare in this block.

     public:
      fft_burst_tagger_impl(float center_frequency, int fft_size, int sample_rate, int burst_pre_len, int burst_post_len, int burst_width, int max_bursts, float threshold, int history_size, bool debug);
      ~fft_burst_tagger_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_FFT_BURST_TAGGER_IMPL_H */

