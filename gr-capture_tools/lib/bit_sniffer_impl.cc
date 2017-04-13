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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "bit_sniffer_impl.h"

namespace gr {
  namespace capture_tools {

    bit_sniffer::sptr
    bit_sniffer::make()
    {
      return gnuradio::get_initial_sptr
        (new bit_sniffer_impl());
    }

    /*
     * The private constructor
     */
    bit_sniffer_impl::bit_sniffer_impl()
      : gr::sync_block("bit_sniffer",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        d_cnt = 0;
        d_last_size = 0;
        d_last = NULL;
        d_since_change = NULL;
        d_fade_out = 200;
        message_port_register_in(pmt::mp("packets"));
        set_msg_handler(pmt::mp("packets"), boost::bind(&bit_sniffer_impl::handler, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    bit_sniffer_impl::~bit_sniffer_impl()
    {
        if (d_last != NULL) {
            delete d_last;
            delete d_since_change;
        }
    }

    void bit_sniffer_impl::handler(pmt::pmt_t msg)
    {
        pmt::pmt_t bit_msg = pmt::cdr(msg);
        size_t packet_length = pmt::length(bit_msg);
        const uint8_t * bits = pmt::u8vector_elements(bit_msg, packet_length);
        if (d_last_size < packet_length) {
            if (d_last != NULL) {
                delete d_last;
                int * temp = new int[packet_length];
                for(int i=0;i<d_last_size;i++)
                    temp[i] = d_since_change[i];
                delete d_since_change;
                d_since_change = temp;
            }
            else {
                d_since_change = new int[packet_length];
            }
            d_last = new uint8_t[packet_length];
            d_last_size = packet_length;
        }
        for(int i=0;i<packet_length;i++) {
            if (bits[i] != d_last[i]) {
                printf("\033[91;1m%1d\033[0m", bits[i]);
                d_since_change[i] = 0;
            }
            else {
                if(d_since_change[i] < d_fade_out) {
                    printf("\033[91;2m%1d\033[0m", bits[i]);
                }
                else {
                    printf("%1d", bits[i]);
                }
                d_since_change[i]++;
            }
            d_last[i] = bits[i];
        }
        printf("\r");
        fflush(stdout);

        d_cnt++;
    }

    int
    bit_sniffer_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

