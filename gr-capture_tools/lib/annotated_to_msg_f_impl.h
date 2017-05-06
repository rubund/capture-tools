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

#ifndef INCLUDED_CAPTURE_TOOLS_ANNOTATED_TO_MSG_F_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_ANNOTATED_TO_MSG_F_IMPL_H

#include <capture_tools/annotated_to_msg_f.h>
#include <vector>

namespace gr {
  namespace capture_tools {

    class annotated_to_msg_f_impl : public annotated_to_msg_f
    {
     private:
      // Nothing to declare in this block.
        int d_state;
        std::vector<uint8_t> d_receive_buffer;
        uint32_t d_sync_word;
        uint32_t d_sync_word_mask;
        int d_sync_word_len;
        uint32_t d_input_buffer;
        int d_start_counter;

        int d_n_to_catch;
        int d_packet_counter;

     public:
      annotated_to_msg_f_impl();
      ~annotated_to_msg_f_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

      void set_sync_word(const std::vector<uint8_t> s);
      void set_packet_length(int val);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_ANNOTATED_TO_MSG_F_IMPL_H */

