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

#ifndef INCLUDED_CAPTURE_TOOLS_CONCATENATE_CC_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_CONCATENATE_CC_IMPL_H

#include <capture_tools/concatenate_cc.h>

namespace gr {
  namespace capture_tools {

    class concatenate_cc_impl : public concatenate_cc
    {
     private:
      // Nothing to declare in this block.
        gr_complex **d_in;
        int d_inputs;
        int d_current;
        bool d_current_done;
        int d_current_samples_left;

     public:
      concatenate_cc_impl(int inputs);
      ~concatenate_cc_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_CONCATENATE_CC_IMPL_H */

