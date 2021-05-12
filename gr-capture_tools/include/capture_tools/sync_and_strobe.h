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


#ifndef INCLUDED_CAPTURE_TOOLS_SYNC_AND_STROBE_H
#define INCLUDED_CAPTURE_TOOLS_SYNC_AND_STROBE_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Search for preamble, then sync word and output message and tagged stream
     * \ingroup capture_tools
     *
     * \details
     * Accepts a quadrature demodulated input (float). If preamble present, it starts
     * searching for sync word. If found, an array is filled up. When done, the array
     * is sent out as a message. Additionally the output contains a 'strobe-tagged' version
     * of the input.
     *
     * There are also many other configurable settings for this block. See set-methods.
     */
    class CAPTURE_TOOLS_API sync_and_strobe : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<sync_and_strobe> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::sync_and_strobe.
       *
       * To avoid accidental use of raw pointers, capture_tools::sync_and_strobe's
       * constructor is in a private implementation
       * class. capture_tools::sync_and_strobe::make is the public interface for
       * creating new instances.
       */
      static sptr make(float sps, int npreamb);

      virtual void set_sync_word(const std::vector<uint8_t> s)=0;
      virtual void set_strobe_offset(int val)=0;
      virtual void set_preamble_timeout(int val)=0;
      virtual void set_n_to_catch(int val)=0;
      virtual void set_sps(float val)=0;
      virtual void set_spsmargin(float val)=0;
      virtual void set_hysteresis(float val)=0;
      virtual void set_decim_in_front(int val)=0;
      virtual void set_min_swing(float val)=0;
      virtual void set_preamble_longer(int val)=0;
      virtual void set_also_match_inverted(bool val)=0;
      virtual void set_name(std::string val)=0;
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_SYNC_AND_STROBE_H */

