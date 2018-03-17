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
#include "threshold_tag_other_stream_cc_impl.h"

namespace gr {
  namespace capture_tools {

    threshold_tag_other_stream_cc::sptr
    threshold_tag_other_stream_cc::make()
    {
      return gnuradio::get_initial_sptr
        (new threshold_tag_other_stream_cc_impl());
    }

    /*
     * The private constructor
     */
    threshold_tag_other_stream_cc_impl::threshold_tag_other_stream_cc_impl()
      : gr::sync_block("threshold_tag_other_stream_cc",
              gr::io_signature::make2(2, 2, sizeof(gr_complex), sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {}

    /*
     * Our virtual destructor.
     */
    threshold_tag_other_stream_cc_impl::~threshold_tag_other_stream_cc_impl()
    {
    }

    int
    threshold_tag_other_stream_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      const float *in_float = (const float *) input_items[1];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

