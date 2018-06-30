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
#include "xrun_monitor_ff_impl.h"

namespace gr {
  namespace capture_tools {

    xrun_monitor_ff::sptr
    xrun_monitor_ff::make()
    {
      return gnuradio::get_initial_sptr
        (new xrun_monitor_ff_impl());
    }

    /*
     * The private constructor
     */
    xrun_monitor_ff_impl::xrun_monitor_ff_impl()
      : gr::block("xrun_monitor_ff",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
        d_n = 0;
    }

    /*
     * Our virtual destructor.
     */
    xrun_monitor_ff_impl::~xrun_monitor_ff_impl()
    {
    }

    int
    xrun_monitor_ff_impl::general_work(int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      d_n += noutput_items;

        if (d_n > 100000) {
            printf("ninput_items: %d, noutput_items: %d\n", ninput_items[0], noutput_items);
            d_n = 0;
        }
      // Do <+signal processing+>
      memcpy(out, in, sizeof(float) * noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

