/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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
#include "burst_msg_source_c_impl.h"

namespace gr {
  namespace capture_tools {

    burst_msg_source_c::sptr
    burst_msg_source_c::make(int n)
    {
      return gnuradio::get_initial_sptr
        (new burst_msg_source_c_impl(n));
    }

    /*
     * The private constructor
     */
    burst_msg_source_c_impl::burst_msg_source_c_impl(int n)
      : gr::sync_block("burst_msg_source_c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
		char name[100];
        if (n == 1) {
            message_port_register_in(pmt::mp("burst"));
            set_msg_handler(pmt::mp("burst"),boost::bind(&burst_msg_source_c_impl::new_burst, this, _1));
            message_port_register_out(pmt::mp("handled"));
        }
        else {
            for(int i=0;i<n;i++) {
                snprintf(name, 99, "burst%d", i);
                message_port_register_in(pmt::mp(name));
                set_msg_handler(pmt::mp(name),boost::bind(&burst_msg_source_c_impl::new_burst, this, _1));
                snprintf(name, 99, "handled%d", i);
		        message_port_register_out(pmt::mp(name));
            }
        }
	}

    /*
     * Our virtual destructor.
     */
    burst_msg_source_c_impl::~burst_msg_source_c_impl()
    {
    }


	void
    burst_msg_source_c_impl::new_burst(pmt::pmt_t msg)
    {
		gr::thread::scoped_lock lock(common_mutex);
		d_bursts.push_back(msg);
    }


    int
    burst_msg_source_c_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *out = (gr_complex *) output_items[0];
		int produced = 0;
		pmt::pmt_t current_burst;

		gr::thread::scoped_lock lock(common_mutex);
		if(d_bursts.size() == 0) {
			usleep(100);
		}
		else {
			current_burst = d_bursts.front();
			d_bursts.pop_front();
			for(int i = 0;i<noutput_items;i++) {
				out[i] = 1;
				produced++;
			}
			pmt::pmt_t handled_msg = pmt::mp("handled");  // FIXME
            message_port_pub(pmt::mp("handled"), handled_msg);
		}

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace capture_tools */
} /* namespace gr */

