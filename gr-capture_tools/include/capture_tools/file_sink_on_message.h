/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_BLE_FILE_SINK_ON_MESSAGE_H
#define INCLUDED_BLE_FILE_SINK_ON_MESSAGE_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>
#include <gnuradio/msg_queue.h>
#include <gnuradio/blocks/file_sink_base.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Write samples when receiving a message
     * \ingroup ble
     *
     * \details
     * Write "number_buffered" to "number_to_write" samples when receiving a message in the queue.
     * "number_to_write" 0s are appended afterwards.
     */
    class CAPTURE_TOOLS_API file_sink_on_message : virtual public gr::sync_block, virtual public gr::blocks::file_sink_base
    {
     public:
      typedef boost::shared_ptr<file_sink_on_message> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ble::file_sink_on_message.
       *
       * To avoid accidental use of raw pointers, ble::file_sink_on_message's
       * constructor is in a private implementation
       * class. ble::file_sink_on_message::make is the public interface for
       * creating new instances.
       */
      static sptr make(size_t itemsize, const char *filename, int number_buffered, int number_to_write, gr::msg_queue::sptr queue);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_BLE_FILE_SINK_ON_MESSAGE_H */

