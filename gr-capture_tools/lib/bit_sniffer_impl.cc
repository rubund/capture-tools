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
        message_port_register_in(pmt::mp("packets"));
        set_msg_handler(pmt::mp("packets"), boost::bind(&bit_sniffer_impl::handler, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    bit_sniffer_impl::~bit_sniffer_impl()
    {
    }

    void bit_sniffer_impl::handler(pmt::pmt_t msg)
    {
        pmt::pmt_t bit_msg = pmt::cdr(msg);
        size_t packet_length = pmt::length(bit_msg);
        const uint8_t * bits = pmt::u8vector_elements(bit_msg, packet_length);
        //std::cout << packet_length << std::endl;
        //std::cout << ((int)bits[0]) << std::endl;
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

