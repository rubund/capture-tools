/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_CAPTURE_TOOLS_ONE_THEN_ZERO_H
#define INCLUDED_CAPTURE_TOOLS_ONE_THEN_ZERO_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Generate a 1, and then only 0s
     * \ingroup capture_tools
     *
     * \details
     * Works with both char and float. The size is set as parameter.
     */
    class CAPTURE_TOOLS_API one_then_zero : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<one_then_zero> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::one_then_zero.
       *
       * To avoid accidental use of raw pointers, capture_tools::one_then_zero's
       * constructor is in a private implementation
       * class. capture_tools::one_then_zero::make is the public interface for
       * creating new instances.
       */
      static sptr make(int size);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_ONE_THEN_ZERO_H */

