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


#ifndef INCLUDED_CAPTURE_TOOLS_BIT_SNIFFER_H
#define INCLUDED_CAPTURE_TOOLS_BIT_SNIFFER_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Presents packet content in console for various settings
     * \ingroup capture_tools
     *
     * \details
     * Accepts a message input containing a bit array. Depending
     * on various settings it presents the bit array in binary, hex
     * or string format. It also shows with color codes which values
     * have recently changed.
     *
     * Scrolling can be turned on and off.
     */
    class CAPTURE_TOOLS_API bit_sniffer : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<bit_sniffer> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::bit_sniffer.
       *
       * To avoid accidental use of raw pointers, capture_tools::bit_sniffer's
       * constructor is in a private implementation
       * class. capture_tools::bit_sniffer::make is the public interface for
       * creating new instances.
       */
      static sptr make(int fade_out=200, bool hexadecimal=0, int offset=0, int bits_per_word=8, bool lsb=0, bool parity=0, bool ascii=0, bool binary=1, int special=0, bool scroll=0);

        virtual void set_fade_out(int) = 0;
        virtual void set_hexadecimal(bool) = 0;
        virtual void set_offset(int) = 0;
        virtual void set_bits_per_word(int) = 0;
        virtual void set_lsb(bool) = 0;
        virtual void set_parity(bool) = 0;
        virtual void set_ascii(bool) = 0;
        virtual void set_binary(bool) = 0;
        virtual void set_special(int) = 0;
        virtual void set_scroll(bool) = 0;
        virtual void set_invert(bool) = 0;
        virtual void set_info(bool) = 0;
        virtual void set_output(const char *filename) = 0;
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_BIT_SNIFFER_H */

