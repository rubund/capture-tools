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


#ifndef INCLUDED_CAPTURE_TOOLS_ADD_AT_TAG_CC_H
#define INCLUDED_CAPTURE_TOOLS_ADD_AT_TAG_CC_H

#include <capture_tools/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Add a random number of a constant value for every tag "tag_key"
     * \ingroup capture_tools
     *
     * \details
     * Whenever the tag with key "tag_key" is encountered, a number from and
     * including "n_min" to and including "n_max" of values "val" are
     * added. The samples are added at the position AFTER the sample containing
     * the tag.
     *
     * n_min == 0 is unsupported.
     */
    class CAPTURE_TOOLS_API add_at_tag_cc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<add_at_tag_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::add_at_tag_cc.
       *
       * To avoid accidental use of raw pointers, capture_tools::add_at_tag_cc's
       * constructor is in a private implementation
       * class. capture_tools::add_at_tag_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int n_min, int n_max, gr_complex val, const char *tag_key);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_ADD_AT_TAG_CC_H */

