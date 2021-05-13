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
#include "zwave_msg_source_impl.h"

namespace gr {
  namespace capture_tools {

    zwave_msg_source::sptr
    zwave_msg_source::make()
    {
      return gnuradio::get_initial_sptr
        (new zwave_msg_source_impl());
    }


    /*
     * The private constructor
     */
    zwave_msg_source_impl::zwave_msg_source_impl()
      : gr::block("zwave_msg_source",
              gr::io_signature::make(0, 0, 1),
              gr::io_signature::make(0, 0, 1))
    {
        message_port_register_out(pmt::mp("packet"));
        message_port_register_in(pmt::mp("request"));
        set_msg_handler(pmt::mp("request"),boost::bind(&zwave_msg_source_impl::request_packet, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    zwave_msg_source_impl::~zwave_msg_source_impl()
    {
    }

    void
    zwave_msg_source_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    void
    zwave_msg_source_impl::request_packet(pmt::pmt_t msg)
    {
        std::ostringstream packet_bits_stream;
        std::string packet_bytes = "5555555555555555555555555555555555555555555555555555555555555555555555f0d66dec2c01410a0e042501ff9c8900000000";
        char current_byte_hex[3];
        //current_byte_hex[0] = "0";
        //current_byte_hex[1] = "x";
        for(int i=0;i<packet_bytes.length();i+=2) {
            current_byte_hex[0] = packet_bytes[i];
            current_byte_hex[1] = packet_bytes[i+1];
            current_byte_hex[2] = 0;
            unsigned char current = strtoul(current_byte_hex, NULL, 16);
            //printf("%02x",current);
            for(int j=0;j<8;j++) {
                char currentbit = ((current) >> (7-j)) & 0x01;
                if (currentbit == 1)
                    packet_bits_stream << "0"; // Inverted
                else
                    packet_bits_stream << "1"; // Inverted
            }
        }
        //std::cout << std::endl;
        std::string packet_bits = packet_bits_stream.str();
        //std::cout << packet_bits << std::endl;
        pmt::pmt_t ready_msg = pmt::mp(packet_bits.c_str());
        message_port_pub(pmt::mp("packet"), ready_msg);
    }

    int
    zwave_msg_source_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      //const <+ITYPE+> *in = (const <+ITYPE+> *) input_items[0];
      //<+OTYPE+> *out = (<+OTYPE+> *) output_items[0];

      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

