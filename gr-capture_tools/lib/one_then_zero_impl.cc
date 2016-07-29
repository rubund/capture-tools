/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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
#include "one_then_zero_impl.h"

namespace gr {
  namespace capture_tools {

    one_then_zero::sptr
    one_then_zero::make(int size)
    {
      return gnuradio::get_initial_sptr
        (new one_then_zero_impl(size));
    }

    /*
     * The private constructor
     */
    one_then_zero_impl::one_then_zero_impl(int size)
      : gr::sync_block("one_then_zero",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, size))
    {
		d_started = 0;
		d_size = size;
	}

    /*
     * Our virtual destructor.
     */
    one_then_zero_impl::~one_then_zero_impl()
    {
    }

    int
    one_then_zero_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      char *bout = (char *) output_items[0];
      float *fout = (float *) output_items[0];

		for(int i = 0; i < noutput_items; i++){	
			if (d_started) {
				if(d_size == 1)
					bout[i] = 0;
				else if(d_size == 4)
					fout[i] = 0;
			}
			else {
				if(d_size == 1)
					bout[i] = 1;
				else if(d_size == 4)
					fout[i] = 1;
				d_started = 1;
			}
		}

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

