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


#ifndef INCLUDED_CAPTURE_TOOLS_CONCATENATE_CC_H
#define INCLUDED_CAPTURE_TOOLS_CONCATENATE_CC_H

#include <capture_tools/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Concatenates the inputs to a longer stream
     * \ingroup capture_tools
     *
     * \details
     * Accepts 'inputs' number of complex input streams of finite length.
     * At first the first stream is copied to the output. When ended,
     * it will continue with the second and so on.
     */
    class CAPTURE_TOOLS_API concatenate_cc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<concatenate_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::concatenate_cc.
       *
       * To avoid accidental use of raw pointers, capture_tools::concatenate_cc's
       * constructor is in a private implementation
       * class. capture_tools::concatenate_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int inputs);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_CONCATENATE_CC_H */

