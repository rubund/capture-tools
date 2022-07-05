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

#ifndef INCLUDED_CAPTURE_TOOLS_KEEP_MIDDLE_IN_VECTOR_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_KEEP_MIDDLE_IN_VECTOR_IMPL_H

#include <capture_tools/keep_middle_in_vector.h>

namespace gr {
  namespace capture_tools {

    class keep_middle_in_vector_impl : public keep_middle_in_vector
    {
     private:
      // Nothing to declare in this block.
        int d_width;
        int d_keep;

     public:
      keep_middle_in_vector_impl(int width, int keep);
      ~keep_middle_in_vector_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_KEEP_MIDDLE_IN_VECTOR_IMPL_H */

