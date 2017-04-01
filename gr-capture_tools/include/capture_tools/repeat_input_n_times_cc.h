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


#ifndef INCLUDED_CAPTURE_TOOLS_REPEAT_INPUT_N_TIMES_CC_H
#define INCLUDED_CAPTURE_TOOLS_REPEAT_INPUT_N_TIMES_CC_H

#include <capture_tools/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Repeat the input N times
     * \ingroup capture_tools
     *
     * \details
     * The end of the input is detected (not perfectly yet).
     * Then the samples until then are repeat N times.
     * "max_samples" chooses the buffer size, and not more than this number of samples
     * can be repeated.
     */
    class CAPTURE_TOOLS_API repeat_input_n_times_cc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<repeat_input_n_times_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::repeat_input_n_times_cc.
       *
       * To avoid accidental use of raw pointers, capture_tools::repeat_input_n_times_cc's
       * constructor is in a private implementation
       * class. capture_tools::repeat_input_n_times_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int n_times, int max_samples);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_REPEAT_INPUT_N_TIMES_CC_H */

