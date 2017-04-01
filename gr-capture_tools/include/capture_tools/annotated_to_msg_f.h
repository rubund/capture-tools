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


#ifndef INCLUDED_CAPTURE_TOOLS_ANNOTATED_TO_MSG_F_H
#define INCLUDED_CAPTURE_TOOLS_ANNOTATED_TO_MSG_F_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>
#include <gnuradio/msg_queue.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief <+description of block+>
     * \ingroup capture_tools
     *
     */
    class CAPTURE_TOOLS_API annotated_to_msg_f : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<annotated_to_msg_f> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::annotated_to_msg_f.
       *
       * To avoid accidental use of raw pointers, capture_tools::annotated_to_msg_f's
       * constructor is in a private implementation
       * class. capture_tools::annotated_to_msg_f::make is the public interface for
       * creating new instances.
       */
      static sptr make(gr::msg_queue::sptr packet_queue);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_ANNOTATED_TO_MSG_F_H */

