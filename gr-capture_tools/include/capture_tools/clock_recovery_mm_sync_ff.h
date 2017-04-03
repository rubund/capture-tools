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


#ifndef INCLUDED_CAPTURE_TOOLS_CLOCK_RECOVERY_MM_SYNC_FF_H
#define INCLUDED_CAPTURE_TOOLS_CLOCK_RECOVERY_MM_SYNC_FF_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief <+description of block+>
     * \ingroup capture_tools
     *
     */
    class CAPTURE_TOOLS_API clock_recovery_mm_sync_ff : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<clock_recovery_mm_sync_ff> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::clock_recovery_mm_sync_ff.
       *
       * To avoid accidental use of raw pointers, capture_tools::clock_recovery_mm_sync_ff's
       * constructor is in a private implementation
       * class. capture_tools::clock_recovery_mm_sync_ff::make is the public interface for
       * creating new instances.
       */
      static sptr make(float omega, float gain_omega, float mu, float gain_mu, float omega_relative_limit);

       virtual float mu() const = 0;
       virtual float omega() const = 0;
       virtual float gain_mu() const = 0;
       virtual float gain_omega() const = 0;
 
       virtual void set_verbose(bool verbose) = 0;
       virtual void set_gain_mu (float gain_mu) = 0;
       virtual void set_gain_omega (float gain_omega) = 0;
       virtual void set_mu (float mu) = 0;
       virtual void set_omega (float omega) = 0;

    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_CLOCK_RECOVERY_MM_SYNC_FF_H */

