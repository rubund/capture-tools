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
#include "tag_first_non_zero_cc_impl.h"

namespace gr {
  namespace capture_tools {

    tag_first_non_zero_cc::sptr
    tag_first_non_zero_cc::make(const std::string &tag_str)
    {
      return gnuradio::get_initial_sptr
        (new tag_first_non_zero_cc_impl(tag_str));
    }

    /*
     * The private constructor
     */
    tag_first_non_zero_cc_impl::tag_first_non_zero_cc_impl(const std::string &tag_str)
      : gr::sync_block("tag_first_non_zero_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
			d_last(gr_complex(0,0)), d_nzeros(0)
    {
       d_tag = pmt::intern(tag_str);
	}

    /*
     * Our virtual destructor.
     */
    tag_first_non_zero_cc_impl::~tag_first_non_zero_cc_impl()
    {
    }

    int
    tag_first_non_zero_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

		for(int i=0;i<noutput_items;i++) {
			if(in[i] == gr_complex(0,0)) {
				if(d_nzeros < 65000000)
					d_nzeros++;
			}
			else {
				if(d_last == gr_complex(0,0) && d_nzeros > 20) {
                    add_item_tag(0, nitems_written(0) + i, d_tag, pmt::from_long(d_nzeros), pmt::intern(""));
				}
				d_nzeros = 0;
			}
			d_last = in[i];
			out[i] = in[i];
		}


      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

