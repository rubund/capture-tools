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
#include "cut_samples_cc_impl.h"

namespace gr {
  namespace capture_tools {

    cut_samples_cc::sptr
    cut_samples_cc::make(int n)
    {
      return gnuradio::get_initial_sptr
        (new cut_samples_cc_impl(n));
    }

    /*
     * The private constructor
     */
    cut_samples_cc_impl::cut_samples_cc_impl(int n)
      : gr::block("cut_samples_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        d_n = n;
        d_cnt = 0;
        d_done = 0;
    }

    /*
     * Our virtual destructor.
     */
    cut_samples_cc_impl::~cut_samples_cc_impl()
    {
    }

    void
    cut_samples_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int
    cut_samples_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        int produced;
        int consumed;
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

        produced = 0;
        consumed = 0;

        if(!d_done) {
        for(int i=0;i<noutput_items;i++) {
            out[i] = in[i];
            d_cnt++;
            produced++;
            if (d_cnt >= d_n) {
                d_done = 1;
                break;
            }
        }
            consumed = produced;
        }
        else {
            produced = -1;
            consumed = ninput_items[0];
        }

      consume_each (consumed);

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace capture_tools */
} /* namespace gr */

