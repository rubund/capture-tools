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


#ifndef INCLUDED_CAPTURE_TOOLS_ADD_BEFORE_AND_AFTER_CC_H
#define INCLUDED_CAPTURE_TOOLS_ADD_BEFORE_AND_AFTER_CC_H

#include <capture_tools/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Add constant before and after stream coming in
     * \ingroup capture_tools
     *
     * \details
     * "n_before" samples of value "val_before" are added before the first incoming sample.
     * "n_after" samples of value "val_after" are added after the last incoming sample.
     *
     * The method of detecting the last sample is not bullet proof.
     */
    class CAPTURE_TOOLS_API add_before_and_after_cc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<add_before_and_after_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::add_before_and_after_cc.
       *
       * To avoid accidental use of raw pointers, capture_tools::add_before_and_after_cc's
       * constructor is in a private implementation
       * class. capture_tools::add_before_and_after_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int n_before, int n_after, int val_before=0, int val_after=0);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_ADD_BEFORE_AND_AFTER_CC_H */

