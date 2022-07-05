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

#ifndef INCLUDED_CAPTURE_TOOLS_THRESHOLD_TAG_OTHER_STREAM_CC_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_THRESHOLD_TAG_OTHER_STREAM_CC_IMPL_H

#include <gnuradio/capture_tools/threshold_tag_other_stream_cc.h>

namespace gr {
  namespace capture_tools {

    class threshold_tag_other_stream_cc_impl : public threshold_tag_other_stream_cc
    {
     private:
      float d_low_thresh;
      float d_high_thresh;
      bool d_is_above;
      bool d_just_started;
      int d_vlen;

      pmt::pmt_t d_going_high_tag;
      pmt::pmt_t d_going_low_tag;

     public:
      threshold_tag_other_stream_cc_impl(float low_thresh, float high_thresh, int vlen);
      ~threshold_tag_other_stream_cc_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

      void set_low_thresh(float);
      void set_high_thresh(float);

      void set_going_high_tag(const std::string &tag_str);
      void set_going_low_tag(const std::string &tag_str);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_THRESHOLD_TAG_OTHER_STREAM_CC_IMPL_H */

