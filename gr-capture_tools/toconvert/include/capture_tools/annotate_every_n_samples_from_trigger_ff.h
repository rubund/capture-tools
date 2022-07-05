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


#ifndef INCLUDED_CAPTURE_TOOLS_ANNOTATE_EVERY_N_SAMPLES_FROM_TRIGGER_FF_H
#define INCLUDED_CAPTURE_TOOLS_ANNOTATE_EVERY_N_SAMPLES_FROM_TRIGGER_FF_H

#include <capture_tools/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Add strobe-tags to float stream
     * \ingroup capture_tools
     *
     * \details
     * One of the inputs is a float stream, the other an enable signal.
     * When the enable signal goes to 1, tags with the key "strobe" is added
     * every "n" samples. An adjustable offset from the beginning can be added.
     *
     * This can be useful for plotting with the QT time sink.
     *
     * It can also be used by the annotated_to_msg_f block to extract packet bits.
     */
    class CAPTURE_TOOLS_API annotate_every_n_samples_from_trigger_ff : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<annotate_every_n_samples_from_trigger_ff> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::annotate_every_n_samples_from_trigger_ff.
       *
       * To avoid accidental use of raw pointers, capture_tools::annotate_every_n_samples_from_trigger_ff's
       * constructor is in a private implementation
       * class. capture_tools::annotate_every_n_samples_from_trigger_ff::make is the public interface for
       * creating new instances.
       */
      static sptr make(int n, int offset);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_ANNOTATE_EVERY_N_SAMPLES_FROM_TRIGGER_FF_H */

