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


#ifndef INCLUDED_CAPTURE_TOOLS_BURST_MSG_SOURCE_C_H
#define INCLUDED_CAPTURE_TOOLS_BURST_MSG_SOURCE_C_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief <+description of block+>
     * \ingroup capture_tools
     *
     */
    class CAPTURE_TOOLS_API burst_msg_source_c : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<burst_msg_source_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::burst_msg_source_c.
       *
       * To avoid accidental use of raw pointers, capture_tools::burst_msg_source_c's
       * constructor is in a private implementation
       * class. capture_tools::burst_msg_source_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(int n=1);

      virtual void set_repeat(bool val)=0;
      virtual void set_round_factor(float val)=0;
      virtual void set_n_zeros(int val)=0;

      virtual void set_start(int val)=0;
      virtual void set_stop(int val)=0;

      virtual void set_max_freq(float val)=0;
      virtual void set_min_freq(float val)=0;

      virtual void set_mag_threshold(float val)=0;
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_BURST_MSG_SOURCE_C_H */

