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

#include <ctime>
#include <gnuradio/io_signature.h>
#include "bit_sniffer_impl.h"

namespace gr {
  namespace capture_tools {

    bit_sniffer::sptr
    bit_sniffer::make(int fade_out, bool hexadecimal, int offset, int bits_per_word, bool lsb, bool parity, bool ascii, bool binary, int special, bool scroll)
    {
      return gnuradio::get_initial_sptr
        (new bit_sniffer_impl(fade_out, hexadecimal, offset, bits_per_word, lsb, parity, ascii, binary, special, scroll));
    }

    /*
     * The private constructor
     */
    bit_sniffer_impl::bit_sniffer_impl(int fade_out, bool hexadecimal, int offset, int bits_per_word, bool lsb, bool parity, bool ascii, bool binary, int special, bool scroll)
      : gr::sync_block("bit_sniffer",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        d_fade_out(fade_out), d_hexadecimal(hexadecimal), d_offset(offset), d_bits_per_word(bits_per_word), d_lsb(lsb), d_parity(parity),
        d_ascii(ascii), d_binary(binary), d_special(special), d_scroll(scroll), d_invert(false)
    {
        d_cnt = 0;
        d_last_size = 0;
        d_last = NULL;
        d_since_change = NULL;
        d_info = false;
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
        pmt::pmt_t bit_meta = pmt::car(msg);
        size_t packet_length = pmt::length(bit_msg);
        const uint8_t * bits = pmt::u8vector_elements(bit_msg, packet_length);
        float burst_frequency_mhz = pmt::to_float(pmt::dict_ref(bit_meta, pmt::mp("freq"), pmt::PMT_NIL));
        float burst_magnitude = pmt::to_float(pmt::dict_ref(bit_meta, pmt::mp("magnitude"), pmt::PMT_NIL));
        uint64_t burst_id = pmt::to_uint64(pmt::dict_ref(bit_meta, pmt::mp("id"), pmt::PMT_NIL));
        uint64_t offset_addressmatch = pmt::to_uint64(pmt::dict_ref(bit_meta, pmt::mp("offset_addressmatch"), pmt::PMT_NIL));

        // TODO: Add optional manchester decoding here before (manipulate packet_length/bits before code below)

        if (d_last_size < packet_length) {
            if (d_last != NULL) {
                uint8_t *temp_last = new uint8_t[packet_length];
                int * temp = new int[packet_length];
                memset(temp_last, 0, sizeof(uint8_t)*packet_length);
                memset(temp, 0, sizeof(int)*packet_length);
                int minval = std::min(d_last_size, (int)packet_length);
                for(int i=0;i<minval;i++) {
                    temp[i] = d_since_change[i];
                    temp_last[i] = d_last[i];
                }
                delete d_last;
                delete d_since_change;
                d_last = temp_last;
                d_since_change = temp;
            }
            else {
                d_last = new uint8_t[packet_length];
                for(int i=0;i<packet_length;i++) {
                    d_last[i] = 255;
                }
                d_since_change = new int[packet_length];
                memset(d_since_change, 0, sizeof(int)*packet_length);
            }
            d_last_size = packet_length;
        }
        if(!d_scroll)
            printf("\33[2K\r");
        printf("%08d: ", d_cnt);
        uint8_t current_byte;
        int bitcounter = 0;
        bool recent_changed = 0;
        bool just_changed = 0;
        int bits_per_word_practice = d_bits_per_word + (d_parity ? 1 : 0);
        bool do_afterwards = 0;
        std::ostringstream hex_out;
        std::ostringstream ascii_out;
        for(int i=0;i<packet_length;i++) {
            uint8_t current_bit;
            current_bit = d_invert ? 1-bits[i] : bits[i];
            if (i < d_offset){
                bitcounter = 0;
                continue;
            }
            if ((bitcounter % bits_per_word_practice) == 0) {
                current_byte = 0;
                recent_changed = 0;
                just_changed = 0;
            }
            if(!d_parity || (bitcounter % bits_per_word_practice) <= (d_bits_per_word)) {
                if(d_lsb) {
                    current_byte |= current_bit ? (1 << (bitcounter % bits_per_word_practice)) : 0;
                }
                else {
                    current_byte |= current_bit ? (1 << ((bits_per_word_practice - 1) - (bitcounter % bits_per_word_practice))) : 0;
                }
            }
            if (current_bit != d_last[i] && d_last[i] != 255) {
                if(d_binary)
                    printf("\033[91;1m%1d\033[0m", current_bit);
                d_since_change[i] = 0;
                just_changed = 1;
            }
            else {
                if(d_since_change[i] < d_fade_out) {
                    if(d_binary)
                        printf("\033[91;2m%1d\033[0m", current_bit);
                    recent_changed = 1;
                }
                else {
                    if(d_binary)
                        printf("%1d", current_bit);
                }
                d_since_change[i]++;
            }
            d_last[i] = current_bit;
            if ((bitcounter % bits_per_word_practice) == (bits_per_word_practice - 1)) {
                if(d_hexadecimal || d_ascii) {
                    if(just_changed){
                        hex_out << boost::format("\033[91;1m%02x\033[0m") % ((int)current_byte);
                        ascii_out << "\033[91;1m" << printable_char(current_byte) << "\033[0m";
                    }
                    else if(recent_changed){
                        hex_out << boost::format("\033[91;2m%02x\033[0m") % ((int)current_byte);
                        ascii_out << "\033[91;2m" << printable_char(current_byte) << "\033[0m";
                    }
                    else {
                        hex_out << boost::format("%02x") % ((int)current_byte);
                        ascii_out << printable_char(current_byte);
                    }
                }
                if(d_binary) {
                    if (i<(packet_length - 1)) {
                        printf(" ");
                    }
                }
                do_afterwards = 0;
            }
            else {
                do_afterwards = 1;
            }
            bitcounter++;
        }
        if(d_hexadecimal || d_ascii) {
            if (do_afterwards) {
                if(just_changed){
                    hex_out << boost::format("\033[91;1m%02x..\033[0m") % ((int)current_byte);
                    ascii_out << "\033[91;1m" << printable_char(current_byte) << "--\033[0m";
                }
                else if(recent_changed) {
                    hex_out << boost::format("\033[91;2m%02x..\033[0m") % ((int)current_byte);
                    ascii_out << "\033[91;2m" << printable_char(current_byte) << "--\033[0m";
                }
                else {
                    hex_out << boost::format("%02x..") % ((int)current_byte);
                    ascii_out << printable_char(current_byte) << "--";
                }
            }
        }

        if(d_binary && d_hexadecimal)
            std::cout << "   ";
        if(d_hexadecimal)
            std::cout << "[" << hex_out.str() << "]";
        if((d_binary || d_hexadecimal) && d_ascii)
            std::cout << "   ";
        if(d_ascii)
            std::cout << "[" << ascii_out.str() << "]";
        if(d_info) {
            printf(" Freq: %7.3f", burst_frequency_mhz);
            printf(", Mag: %5.2f", burst_magnitude);
            printf(", ID: %4llu", burst_id);
            printf(", offset: %4llu", offset_addressmatch);
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            printf(", Time: %04d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        }

        if(d_scroll)
            printf("\n");
        fflush(stdout);

        d_cnt++;
    }
        char bit_sniffer_impl::printable_char(char ch)
        {
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))
                return ch;
            else
                return '.';
        }

        void bit_sniffer_impl::set_fade_out(int val)
        {
            d_fade_out = val;
        }

        void bit_sniffer_impl::set_hexadecimal(bool val)
        {
            d_hexadecimal = val;
        }

        void bit_sniffer_impl::set_offset(int val)
        {
            d_offset = val;
        }

        void bit_sniffer_impl::set_bits_per_word(int val)
        {
            d_bits_per_word = val;
        }

        void bit_sniffer_impl::set_lsb(bool val)
        {
            d_lsb = val;
        }

        void bit_sniffer_impl::set_parity(bool val)
        {
            d_parity = val;
        }

        void bit_sniffer_impl::set_ascii(bool val)
        {
            d_ascii = val;
        }

        void bit_sniffer_impl::set_binary(bool val)
        {
            d_binary = val;
        }

        void bit_sniffer_impl::set_special(int val)
        {
            d_special = val;
        }

        void bit_sniffer_impl::set_scroll(bool val)
        {
            d_scroll = val;
        }

        void bit_sniffer_impl::set_invert(bool val)
        {
            d_invert = val;
        }

        void bit_sniffer_impl::set_info(bool val)
        {
            d_info = val;
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

