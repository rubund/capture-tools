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
#include "annotate_every_n_samples_from_trigger_ff_impl.h"

namespace gr {
  namespace capture_tools {

    annotate_every_n_samples_from_trigger_ff::sptr
    annotate_every_n_samples_from_trigger_ff::make(int n, int offset)
    {
      return gnuradio::get_initial_sptr
        (new annotate_every_n_samples_from_trigger_ff_impl(n, offset));
    }

    /*
     * The private constructor
     */
    annotate_every_n_samples_from_trigger_ff_impl::annotate_every_n_samples_from_trigger_ff_impl(int n, int offset)
      : gr::block("annotate_every_n_samples_from_trigger_ff",
              gr::io_signature::make(2, 2, sizeof(float)),
              gr::io_signature::make(1, 2, sizeof(float)))
    {
        d_n = n;
        d_offset = offset;
        d_cnt = 0;
        d_state = 0;
        d_valnotstrobe = -1000;
    }

    /*
     * Our virtual destructor.
     */
    annotate_every_n_samples_from_trigger_ff_impl::~annotate_every_n_samples_from_trigger_ff_impl()
    {
    }

    void
    annotate_every_n_samples_from_trigger_ff_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
        ninput_items_required[1] = noutput_items;
    }

    int
    annotate_every_n_samples_from_trigger_ff_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      const float *in_threshold = (const float *) input_items[1];
      float *out = (float *) output_items[0];
      float *out_strobe = (float *) output_items[1];
        char output2connected;
        if (output_items.size() == 2){
            output2connected = 1;
        }
        else {
            output2connected = 0;
        }

        for(int i=0;i<noutput_items;i++) {
            if (d_state == 0) {
                if (in_threshold[i] == 1) {
                    d_state = 1;
                    d_cnt = 0;
                }
                if(output2connected)
                    out_strobe[i] = d_valnotstrobe;
            }
            else if (d_state == 1) {
                d_cnt++;
                if (d_cnt == d_offset) {
                    d_state = 2;
                    d_cnt = 0;
                }
            }
            else if (d_state == 2) {
                if ((d_cnt % d_n) == 0) {
                    add_item_tag(0, nitems_written(0) + i, pmt::intern("strobe"), pmt::intern(""), pmt::intern(""));
                    if(output2connected)
                        out_strobe[i] = 1;
                }
                else {
                    if(output2connected)
                        out_strobe[i] = d_valnotstrobe;
                }
                d_cnt++;
                if (in_threshold[i] == 0) {
                    d_state = 0;
                    d_cnt = 0;
                }
            }
        }
        memcpy(out, in, sizeof(float)*noutput_items);


      consume_each (noutput_items);


      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

