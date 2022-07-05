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


#ifndef INCLUDED_CAPTURE_TOOLS_TAG_ON_SET_ONE_B_H
#define INCLUDED_CAPTURE_TOOLS_TAG_ON_SET_ONE_B_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Make a single tag when a set method is called with the value 1 (char)
     * \ingroup capture_tools
     *
     * \details
     * The set method can be connected to a button in gnuradio-companion. A click
     * on the button then injects a tag into the stream.
     * The stream is otherwise copied from the input to the output.
     */
    class CAPTURE_TOOLS_API tag_on_set_one_b : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<tag_on_set_one_b> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::tag_on_set_one_b.
       *
       * To avoid accidental use of raw pointers, capture_tools::tag_on_set_one_b's
       * constructor is in a private implementation
       * class. capture_tools::tag_on_set_one_b::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string &tag_str);

      virtual void set_value(int val) = 0;
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_TAG_ON_SET_ONE_B_H */

