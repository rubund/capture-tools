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


#ifndef INCLUDED_CAPTURE_TOOLS_MSG_TO_GFSK_C_H
#define INCLUDED_CAPTURE_TOOLS_MSG_TO_GFSK_C_H

#include <capture_tools/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief <+description of block+>
     * \ingroup capture_tools
     *
     */
    class CAPTURE_TOOLS_API msg_to_gfsk_c : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<msg_to_gfsk_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::msg_to_gfsk_c.
       *
       * To avoid accidental use of raw pointers, capture_tools::msg_to_gfsk_c's
       * constructor is in a private implementation
       * class. capture_tools::msg_to_gfsk_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(int samples_per_symbol, float sensitivity, float bt);


      virtual void set_fixed_delay(int)=0;
      virtual void set_fixed_delay_range(int)=0;
      virtual void set_wait_settle(int)=0;
      virtual void set_rampup_delay(int)=0;
      virtual void set_rampdown_delay(int)=0;
      virtual void set_max_queue_length(int)=0;
      virtual void set_rampup_alpha(float)=0;
      virtual void set_rampdown_alpha(float)=0;
      virtual void set_value_between(float)=0;
      virtual void set_reset_phase(bool)=0;
      virtual void set_repeat_packet(bool)=0;
      virtual void set_level_off(float)=0;
      virtual void set_sensitivity(float)=0;
      virtual void set_bt(float)=0;
      virtual void set_randomize_phase(bool)=0;
      virtual void set_sensitivity_range(float)=0;
      virtual void set_search_tag(const std::string &)=0;
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_MSG_TO_GFSK_C_H */

