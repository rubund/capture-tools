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

#ifndef INCLUDED_BLE_LEVEL_ALERT_IMPL_H
#define INCLUDED_BLE_LEVEL_ALERT_IMPL_H

#include <capture_tools/level_alert.h>

namespace gr {
  namespace capture_tools {

    #define AVER 1000

    class level_alert_impl : public level_alert
    {
     private:
      // Nothing to declare in this block.
	  float d_magnitudes[AVER];
	  int d_magnitudeindex;
	  float d_level;
	  gr::msg_queue::sptr d_alert_queue;

     public:
      level_alert_impl(float level, gr::msg_queue::sptr alert_queue);
      ~level_alert_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_BLE_LEVEL_ALERT_IMPL_H */

