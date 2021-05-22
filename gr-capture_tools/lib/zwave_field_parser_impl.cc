/* -*- c++ -*- */
/*
 * Copyright 2021 gr-capture_tools author.
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
#include "zwave_field_parser_impl.h"

namespace gr {
  namespace capture_tools {

    zwave_field_parser::sptr
    zwave_field_parser::make()
    {
      return gnuradio::get_initial_sptr
        (new zwave_field_parser_impl());
    }


    /*
     * The private constructor
     */
    zwave_field_parser_impl::zwave_field_parser_impl()
      : gr::sync_block("zwave_field_parser",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        message_port_register_in(pmt::mp("formatted_packets"));
        set_msg_handler(pmt::mp("formatted_packets"), boost::bind(&zwave_field_parser_impl::handler, this, _1));
    }

    void zwave_field_parser_impl::handler(pmt::pmt_t msg)
    {
        pmt::pmt_t bit_msg = pmt::cdr(msg);
        pmt::pmt_t bit_meta = pmt::car(msg);
        size_t packet_length = pmt::length(bit_msg);
        const uint8_t * bytes = pmt::u8vector_elements(bit_msg, packet_length);
        std::ostringstream string_out;
        //for(int i=0;i<packet_length;i++)
        //  string_out << boost::format("%02x") % ((int)bytes[i]);
        unsigned int home_id = (bytes[3] << 0) | (bytes[2] << 8) | (bytes[1] << 16) | (bytes[0] << 24);
        string_out << "Home ID: " << boost::format("%08x") % (home_id);

        fprintf(stdout, "Packet received: [%s]\n", string_out.str().c_str());
        fflush(stdout);
    }

    /*
     * Our virtual destructor.
     */
    zwave_field_parser_impl::~zwave_field_parser_impl()
    {
    }

    int
    zwave_field_parser_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {

      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

