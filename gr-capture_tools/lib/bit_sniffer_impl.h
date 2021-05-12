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

#ifndef INCLUDED_CAPTURE_TOOLS_BIT_SNIFFER_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_BIT_SNIFFER_IMPL_H

#include <capture_tools/bit_sniffer.h>

namespace gr {
  namespace capture_tools {

    class bit_sniffer_impl : public bit_sniffer
    {
     private:
      bool d_bitstuff;
      bool d_diff;
      // Nothing to declare in this block.
    void handler(pmt::pmt_t msg);
    char printable_char(char ch);
        int d_cnt;
        uint8_t *d_last;
        int *d_since_change;
        int d_last_size;

        int d_fade_out;
        bool d_hexadecimal;
        int d_offset;
        int d_bits_per_word;
        bool d_lsb;
        bool d_parity;
        bool d_ascii;
        bool d_binary;
        int d_special;
        bool d_scroll;
        bool d_invert;
        bool d_info;
        bool d_show_magnitude;
        FILE *d_fp;

     public:
      bit_sniffer_impl(int fade_out=200, bool hexadecimal=0, int offset=0, int bits_per_word=8, bool lsb=0, bool parity=0, bool ascii=0, bool binary=1, int special=0, bool scroll=0);
      ~bit_sniffer_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);


        void set_fade_out(int);
        void set_hexadecimal(bool);
        void set_offset(int);
        void set_bits_per_word(int);
        void set_lsb(bool);
        void set_parity(bool);
        void set_ascii(bool);
        void set_binary(bool);
        void set_special(int);
        void set_scroll(bool);
        void set_invert(bool);
        void set_info(bool);
        void set_show_magnitude(bool);
        void set_output(const char *filename);
      void set_diff(bool val);
      void set_bitstuff(bool val);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_BIT_SNIFFER_IMPL_H */

