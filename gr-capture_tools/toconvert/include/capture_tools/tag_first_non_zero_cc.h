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


#ifndef INCLUDED_CAPTURE_TOOLS_TAG_FIRST_NON_ZERO_CC_H
#define INCLUDED_CAPTURE_TOOLS_TAG_FIRST_NON_ZERO_CC_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief First sample after a row of zeros is tagged.
     * \ingroup capture_tools
     *
     * \details
     * Complex input and output.
     * If there are 20 or more zeros after each other, the
     * first sample afterwards will be tagged with a configurable
     * tag.
     */
    class CAPTURE_TOOLS_API tag_first_non_zero_cc : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<tag_first_non_zero_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::tag_first_non_zero_cc.
       *
       * To avoid accidental use of raw pointers, capture_tools::tag_first_non_zero_cc's
       * constructor is in a private implementation
       * class. capture_tools::tag_first_non_zero_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string &tag_str);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_TAG_FIRST_NON_ZERO_CC_H */

