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
#include "keep_middle_in_vector_impl.h"

namespace gr {
  namespace capture_tools {

    keep_middle_in_vector::sptr
    keep_middle_in_vector::make(int width, int keep)
    {
      return gnuradio::get_initial_sptr
        (new keep_middle_in_vector_impl(width, keep));
    }

    /*
     * The private constructor
     */
    keep_middle_in_vector_impl::keep_middle_in_vector_impl(int width, int keep)
      : gr::sync_block("keep_middle_in_vector",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*width),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*width))
    {
        d_width = width;
        d_keep = keep;
    }

    /*
     * Our virtual destructor.
     */
    keep_middle_in_vector_impl::~keep_middle_in_vector_impl()
    {
    }

    int
    keep_middle_in_vector_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        int startpos = (d_width/2) - (d_keep / 2);
        int stoppos = (d_width/2) + (d_keep / 2);
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

        for(int i=0;i<noutput_items;i++) {
            for(int j=0;j<d_width;j++) {
                if(j > startpos && j <= stoppos)
                    out[i*d_width + j] = in[i*d_width + j];
                else
                    out[i*d_width + j] = 0;
            }
        }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

