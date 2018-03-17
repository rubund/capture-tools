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

#include <vector>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <gnuradio/io_signature.h>
#include <gnuradio/filter/firdes.h>
#include "msg_to_gfsk_c_impl.h"

namespace gr {
  namespace capture_tools {

    msg_to_gfsk_c::sptr
    msg_to_gfsk_c::make(int samples_per_symbol, float sensitivity, float bt)
    {
      return gnuradio::get_initial_sptr
        (new msg_to_gfsk_c_impl(samples_per_symbol, sensitivity, bt));
    }

    /*
     * The private constructor
     */
    msg_to_gfsk_c_impl::msg_to_gfsk_c_impl(int samples_per_symbol, float sensitivity, float bt)
      : gr::block("msg_to_gfsk_c",
              gr::io_signature::make(0, 1, sizeof(char)),
              gr::io_signature::make2(1, 2, sizeof(gr_complex), sizeof(float))), d_samples_per_symbol(samples_per_symbol),
      d_sensitivity(sensitivity), d_bt(bt), d_fixed_delay(10000), d_delay_counter(0),
      d_ready_to_transmit(1), d_current_packet(NULL), d_bitcounter(0), d_upsample_counter(0), d_wait_settle(80),
      d_rampup_delay(40), d_rampdown_delay(40), d_max_queue_length(1000), d_state(0), d_tag_set(1), d_filt_input_cnt(0),
      d_rampup_alpha(0.2), d_rampdown_alpha(0.2), d_phase(0), d_value_between(0), d_fixed_delay_range(1000), d_current_delay(0),
      d_reset_phase(1), d_repeat_packet(0), d_level_off(0), d_rampup_tag_set(0), d_randomize_phase(0), d_sensitivity_range(0),
      d_current_sensitivity(0), d_packets_requested(0)
    {
      srand(time(NULL));
      message_port_register_in(pmt::mp("packet"));
      set_msg_handler(pmt::mp("packet"),boost::bind(&msg_to_gfsk_c_impl::add_packet_to_queue, this, _1));
      message_port_register_out(pmt::mp("ready"));

      d_filt_input = NULL;
      init_filter();

      set_tag_propagation_policy(TPP_DONT);
      //d_fir = new gr::filter::kernel::fir_filter_fff(1, d_filter_taps);
    }

    /*
     * Our virtual destructor.
     */
    msg_to_gfsk_c_impl::~msg_to_gfsk_c_impl()
    {
      //delete d_fir;
      if(d_filt_input != NULL)
        delete d_filt_input;
    }

    void
    msg_to_gfsk_c_impl::init_filter()
    {
      gr::thread::scoped_lock lock(common_mutex);

      std::vector<float> gaussian;
      d_filter_taps = gr::filter::firdes::gaussian(1.0, d_samples_per_symbol, d_bt, 4*d_samples_per_symbol);

      if(d_filt_input != NULL)
        delete d_filt_input;
      d_filt_input = new float[d_filter_taps.size()];
      memset(d_filt_input, 0, sizeof(float)*d_filter_taps.size());
    }

    void
    msg_to_gfsk_c_impl::set_sensitivity(float val)
    {
      d_sensitivity = val;
    }

    void
    msg_to_gfsk_c_impl::set_bt(float val)
    {
      d_bt = val;
      init_filter();
    }

    void
    msg_to_gfsk_c_impl::set_fixed_delay(int val)
    {
      d_fixed_delay = val;
    }

    void
    msg_to_gfsk_c_impl::set_fixed_delay_range(int val)
    {
      d_fixed_delay_range = val;
    }

    void
    msg_to_gfsk_c_impl::set_reset_phase(bool val)
    {
      d_reset_phase = val;
    }

    void
    msg_to_gfsk_c_impl::set_repeat_packet(bool val)
    {
      d_repeat_packet = val;
    }

    void
    msg_to_gfsk_c_impl::set_wait_settle(int val)
    {
      d_wait_settle = val;
    }

    void
    msg_to_gfsk_c_impl::set_rampup_delay(int val)
    {
      d_rampup_delay = val;
    }

    void
    msg_to_gfsk_c_impl::set_rampdown_delay(int val)
    {
      d_rampdown_delay = val;
    }

    void
    msg_to_gfsk_c_impl::set_max_queue_length(int val)
    {
      d_max_queue_length = val;
    }

    void
    msg_to_gfsk_c_impl::set_rampup_alpha(float val)
    {
      d_rampup_alpha = val;
    }

    void
    msg_to_gfsk_c_impl::set_rampdown_alpha(float val)
    {
      d_rampdown_alpha = val;
    }

    void
    msg_to_gfsk_c_impl::set_value_between(float val)
    {
      d_value_between = val;
    }

    void
    msg_to_gfsk_c_impl::set_level_off(float val)
    {
      d_level_off = val;
    }

    void
    msg_to_gfsk_c_impl::set_randomize_phase(bool val)
    {
      d_randomize_phase = val;
    }

    void
    msg_to_gfsk_c_impl::set_sensitivity_range(float val)
    {
      d_sensitivity_range = val;
    }

    void
    msg_to_gfsk_c_impl::add_packet_to_queue(pmt::pmt_t msg)
    {
      gr::thread::scoped_lock lock(common_mutex);
      //printf("Adding packet to queue\n");
      std::string bits = pmt::symbol_to_string(msg);
      std::vector<char> *bitvector;
      bitvector = new std::vector<char>();
      for(int i=0;i<bits.length();i++) {
        int t;
        t = bits.c_str()[i] - '0';
        bitvector->push_back((char)t);
      }
      d_packets.push_back(bitvector);
    }

    void
    msg_to_gfsk_c_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      if(ninput_items_required.size() > 0) {
        ninput_items_required[0] = noutput_items;
      }
      else {
        ninput_items_required[0] = 0;
      }
    }

    void
    msg_to_gfsk_c_impl::set_current_delay()
    {
      if(d_fixed_delay_range > 0)
        d_current_delay = d_fixed_delay + (std::rand() % d_fixed_delay_range) - (int)(d_fixed_delay_range/2);
      else
        d_current_delay = d_fixed_delay;
    }

    void
    msg_to_gfsk_c_impl::unset_current_packet()
    {
      if(!d_repeat_packet) {
        delete d_current_packet;
      }
      d_current_packet = NULL;
    }

    int
    msg_to_gfsk_c_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const char *in;
      if (ninput_items.size() > 0) {
        in = (const char *) input_items[0];
      }
      else {
        in = NULL;
      }
      gr_complex *out = (gr_complex *) output_items[0];

      float *mod_out;
      if(output_items.size() >= 2)
        mod_out = (float *) output_items[1];
      else
        mod_out = NULL;

      float modulation;

      int ntaps = d_filter_taps.size();
      float intofilt;
      float scale;

      gr::thread::scoped_lock lock(common_mutex);

      int available_in_queue;
      //pmt::pmt_t ready_msg = pmt::from_long(1);
      pmt::pmt_t ready_msg = pmt::mp("0111100001111000011110000111100001111000011110000111100001111000011110000111100000100111001010110111010010010100101001000011110100101101011101011"); // FIXME: Just for testing

      if(d_repeat_packet) {
        if (d_packets.size() == 0) {
          message_port_pub(pmt::mp("ready"), ready_msg);
          //printf("Sending ready\n");
        }
      }
      else {
        if(noutput_items != 0) {
          available_in_queue = d_max_queue_length - d_packets_requested;
          for(int i=0;i<available_in_queue;i++) {
            message_port_pub(pmt::mp("ready"), ready_msg);
            d_packets_requested++;
            //printf("Sending ready\n");
          }
        }
      }

      for(int i=0;i<noutput_items;i++) {

        intofilt = 0;
        scale = d_level_off;
        // Check if any packets available in queue. In that case, set d_current_packet to current packet
        if(d_current_packet == NULL && d_ready_to_transmit) {
          if(d_packets.size() > 0) {
            d_current_packet = d_packets.front();
            if(!d_repeat_packet)
              d_packets.pop_front();
            if(d_packets_requested > 0)
              d_packets_requested--;
            if(d_rampup_delay != 0)
              d_state = 1;
            else
              d_state = 2;
            d_ready_to_transmit = 0;
            d_tag_set = 0;
            if(d_reset_phase)
              d_phase = 0;
            if(d_randomize_phase) {
              d_phase = (float)(rand() % 1000000)/1000000.0 * 2*M_PI;
            }
            if(d_sensitivity_range > 0.000001) {
              d_current_sensitivity = d_sensitivity + ((float)(rand() % 1000000)/1000000.0 * d_sensitivity_range) - (d_sensitivity_range / 2.0);
            }
            else {
              d_current_sensitivity = d_sensitivity;
            }
            available_in_queue = d_max_queue_length - d_packets_requested;
            if((!d_repeat_packet) && available_in_queue > 0) {
              message_port_pub(pmt::mp("ready"), ready_msg);
              d_packets_requested++;
              //printf("Sending ready\n");
            }
          }
        }

        // If d_current_packet != NULL, we have three different states: rampup, packet, rampdown
        if(d_current_packet) {
          if(d_state == 1) { // Rampup
            if(!d_tag_set) {
              add_item_tag(0, nitems_written(0) + i, pmt::intern("ramp_up"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));
              if(mod_out != NULL)
                add_item_tag(1, nitems_written(0) + i, pmt::intern("ramp_up"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));
              d_tag_set = 1;
              d_rampup_tag_set = 1;
            }
            scale = 1 - (1-d_level_off)*exp(-d_delay_counter*d_rampup_alpha);
            intofilt = d_value_between;
            d_delay_counter++;
            if(d_delay_counter >= d_rampup_delay) {
              d_delay_counter = 0;
              d_state = 2;
              d_tag_set = 0;
            }
          }
          else if(d_state == 2) { // Packet itself
            if(!d_tag_set) {
              add_item_tag(0, nitems_written(0) + i, pmt::intern("packet"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));
              if(mod_out != NULL)
                add_item_tag(1, nitems_written(0) + i, pmt::intern("packet"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));

              if(!d_rampup_tag_set) { // If in a mode where we never enter the rampup state. Add the tag ramp_up at the same sample as packet. This simplifies QT frequency plot triggering
                add_item_tag(0, nitems_written(0) + i, pmt::intern("ramp_up"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));
                if(mod_out != NULL)
                  add_item_tag(1, nitems_written(0) + i, pmt::intern("ramp_up"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));
              }
              d_tag_set = 1;
            }
            scale = 1;
            intofilt = d_current_packet->at(d_bitcounter) ? 1 : -1;
            d_upsample_counter++;
            if(d_upsample_counter == d_samples_per_symbol) {
              d_upsample_counter = 0;
              d_bitcounter++;
              if(d_bitcounter == d_current_packet->size()) {
                if(d_wait_settle != 0)
                  d_state = 3;
                else if(d_rampdown_delay != 0)
                  d_state = 4;
                else {
                  set_current_delay();
                  unset_current_packet();
                  d_state = 0;
                  if(d_current_delay == 0) {
                    d_ready_to_transmit = 1;
                  }
                }
                d_bitcounter = 0;
                d_tag_set = 0;
                d_rampup_tag_set = 0;
              }
            }
          }
          else if(d_state == 3) { // Wait to settle
            if(!d_tag_set) {
              add_item_tag(0, nitems_written(0) + i, pmt::intern("wait_to_settle"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));
              if(mod_out != NULL)
                add_item_tag(1, nitems_written(0) + i, pmt::intern("wait_to_settle"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));
              d_tag_set = 1;
            }
            scale = 1;
            intofilt = d_value_between;
            d_delay_counter++;
            if(d_delay_counter >= d_wait_settle) {
              d_delay_counter = 0;
              if(d_rampdown_delay != 0)
                d_state = 4;
              else {
                set_current_delay();
                unset_current_packet();
                d_state = 0;
                if(d_current_delay == 0) {
                  d_ready_to_transmit = 1;
                }
              }
              d_tag_set = 0;
            }
          }
          else if(d_state == 4) { // Rampdown
            if(!d_tag_set) {
              add_item_tag(0, nitems_written(0) + i, pmt::intern("ramp_down"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));
              if(mod_out != NULL)
                add_item_tag(1, nitems_written(0) + i, pmt::intern("ramp_down"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));
              d_tag_set = 1;
            }
            scale = d_level_off + (1-d_level_off)*exp(-d_delay_counter*d_rampdown_alpha);
            intofilt = d_value_between;
            d_delay_counter++;
            if(d_delay_counter >= d_rampdown_delay) {
              unset_current_packet();
              d_delay_counter = 0;
              d_state = 0;
              d_tag_set = 0;
              set_current_delay();
            }
          }

        }
        // If d_current_packet == NULL, we will count d_fixed_delay until opening up d_ready_to_transmit
        else {
          if(!d_tag_set) {
            add_item_tag(0, nitems_written(0) + i, pmt::intern("complete"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));
            if(mod_out != NULL)
              add_item_tag(1, nitems_written(0) + i, pmt::intern("complete"), pmt::intern(""), pmt::intern("msg_to_gfsk_c"));
            d_tag_set = 1;
          }
          intofilt = d_value_between;
          if (!d_ready_to_transmit) {
            d_delay_counter++;
            if(d_delay_counter >= d_current_delay) {
              d_delay_counter = 0;
              d_ready_to_transmit = 1;
            }
          }
        }

        d_filt_input[d_filt_input_cnt] = intofilt;
        float sum = 0;
        for(int j=0;j<ntaps;j++) {
          int wraparound = (d_filt_input_cnt-j);
          if(wraparound < 0) {
            sum += d_filt_input[ntaps+wraparound] * d_filter_taps[j];
          }
          else {
            sum += d_filt_input[d_filt_input_cnt-j] * d_filter_taps[j];
          }
        }
        d_filt_input_cnt++;
        if(d_filt_input_cnt == ntaps)
          d_filt_input_cnt = 0;
        modulation = sum;

        d_phase += modulation*d_current_sensitivity;
        if(d_phase > M_PI)
          d_phase -= 2*M_PI;
        else if (d_phase < -M_PI)
          d_phase += 2*M_PI;
        out[i] = gr_complex(scale*cos(d_phase),scale*sin(d_phase));

        if(mod_out != NULL)
          mod_out[i] = modulation;
      }

      if(in != NULL)
        consume_each (noutput_items);

      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

