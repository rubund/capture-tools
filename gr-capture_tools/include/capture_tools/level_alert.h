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


#ifndef INCLUDED_BLE_LEVEL_ALERT_H
#define INCLUDED_BLE_LEVEL_ALERT_H

#include <capture_tools/api.h>
#include <gnuradio/block.h>
#include <gnuradio/msg_queue.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Generate message on queue when magnitude of input is above a threshold
     * \ingroup ble
     *
     * \details
     * 1000 samples are averaged. When the average is above "level" in dB,
     * a message is being produced. This message can be used to trigger file
     * logging for file_sink_on_message.
     */
    class CAPTURE_TOOLS_API level_alert : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<level_alert> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ble::level_alert.
       *
       * To avoid accidental use of raw pointers, ble::level_alert's
       * constructor is in a private implementation
       * class. ble::level_alert::make is the public interface for
       * creating new instances.
       */
      static sptr make(float level, gr::msg_queue::sptr alert_queue);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_BLE_LEVEL_ALERT_H */

