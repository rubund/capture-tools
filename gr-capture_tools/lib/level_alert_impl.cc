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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "level_alert_impl.h"

namespace gr {
  namespace capture_tools {

    level_alert::sptr
    level_alert::make(float level, gr::msg_queue::sptr alert_queue)
    {
      return gnuradio::get_initial_sptr
        (new level_alert_impl(level, alert_queue));
    }

    /*
     * The private constructor
     */
    level_alert_impl::level_alert_impl(float level, gr::msg_queue::sptr alert_queue)
      : gr::block("level_alert",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 0, sizeof(char))), d_level(level), d_alert_queue(alert_queue)
    {
        for(int i=0;i<AVER;i++) {
            d_magnitudes[i] = 0;
        }
        d_magnitudeindex = 0;

    }

    /*
     * Our virtual destructor.
     */
    level_alert_impl::~level_alert_impl()
    {
    }

    void
    level_alert_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    level_alert_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        float tmp;

        for (int i = 0; i < ninput_items[0]; i++){
              tmp = in[i].real()*in[i].real()+in[i].imag()*in[i].imag();
              d_magnitudes[d_magnitudeindex] = tmp;
              d_magnitudeindex++;
              if(d_magnitudeindex >= AVER-1){
                  float sum=0,average;
                  for(int j=0;j<AVER;j++) {
                      sum += d_magnitudes[j];
                  }
                  average = 10*log10(sum/AVER);	
                  if(average > d_level) {
                      char tmp = 1;
                      gr::message::sptr msg = gr::message::make(0,0,0,1);
                      memcpy(msg->msg(),&tmp,1);
                      d_alert_queue->insert_tail(msg);
                      //printf("Average level for the last %d was %f\n",AVER,average);
                  }
                  d_magnitudeindex = 0;
              }
        }

        // Tell runtime system how many input items we consumed on
        // each input stream.

        consume_each (ninput_items[0]);

        // Tell runtime system how many output items we produced.
        return 0;
    }

  } /* namespace capture_tools */
} /* namespace gr */

