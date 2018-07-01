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

#ifndef INCLUDED_CAPTURE_TOOLS_XRUN_MONITOR_FF_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_XRUN_MONITOR_FF_IMPL_H

#include <capture_tools/xrun_monitor_ff.h>

namespace gr {
  namespace capture_tools {

    class xrun_monitor_ff_impl : public xrun_monitor_ff
    {
     private:
      // Nothing to declare in this block.
      int d_n;
      int d_produce_per;
      float *d_buffer;
      int d_write_index;
      int d_read_index;
      int d_length;
      bool d_first;
      bool d_starting;

     public:
      xrun_monitor_ff_impl();
      ~xrun_monitor_ff_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      // Where all the action really happens
      int general_work(int noutput_items,
         gr_vector_int &ninpnut_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_XRUN_MONITOR_FF_IMPL_H */

