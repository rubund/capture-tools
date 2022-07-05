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
#include "replace_value_ff_impl.h"

namespace gr {
  namespace capture_tools {

    replace_value_ff::sptr
    replace_value_ff::make(float search, float replace)
    {
      return gnuradio::get_initial_sptr
        (new replace_value_ff_impl(search, replace));
    }

    /*
     * The private constructor
     */
    replace_value_ff_impl::replace_value_ff_impl(float search, float replace)
      : gr::sync_block("replace_value_ff",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
        d_search = search;
        d_replace = replace;
    }

    /*
     * Our virtual destructor.
     */
    replace_value_ff_impl::~replace_value_ff_impl()
    {
    }

    int
    replace_value_ff_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

        for(int i=0;i<noutput_items;i++){
            if ((in[i] > (d_search-0.00000000001) && (in[i] < (d_search+0.00000000001)))) {
                out[i] = d_replace;
            }
            else {
                out[i] = in[i];
            }
        }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

