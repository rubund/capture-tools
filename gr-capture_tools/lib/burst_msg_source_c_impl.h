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

#ifndef INCLUDED_CAPTURE_TOOLS_BURST_MSG_SOURCE_C_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_BURST_MSG_SOURCE_C_IMPL_H

#include <queue>
#include <deque>
#include <vector>
#include <capture_tools/burst_msg_source_c.h>

namespace gr {
  namespace capture_tools {

    class burst_msg_source_c_impl : public burst_msg_source_c
    {
     private:
	  std::deque<pmt::pmt_t> d_bursts;
	  void new_burst(pmt::pmt_t msg);
	  boost::mutex common_mutex;
      bool d_in_burst;
      int d_current_burst_pos;

     public:
      burst_msg_source_c_impl(int n);
      ~burst_msg_source_c_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_BURST_MSG_SOURCE_C_IMPL_H */

