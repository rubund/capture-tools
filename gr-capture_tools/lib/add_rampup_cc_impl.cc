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
#include "add_rampup_cc_impl.h"
#include <vector>

namespace gr {
  namespace capture_tools {

    add_rampup_cc::sptr
    add_rampup_cc::make(float alpha, int constant_start, int constant_stop, int constant_length)
    {
      return gnuradio::get_initial_sptr
        (new add_rampup_cc_impl(alpha, constant_start, constant_stop, constant_length));
    }

    /*
     * The private constructor
     */
    add_rampup_cc_impl::add_rampup_cc_impl(float alpha, int constant_start, int constant_stop, int constant_length)
      : gr::sync_block("add_rampup_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        d_alpha = alpha;
        d_scale = 0;
        d_state = 0;
    }

    /*
     * Our virtual destructor.
     */
    add_rampup_cc_impl::~add_rampup_cc_impl()
    {
    }

    int
    add_rampup_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        std::vector<int> rampup_start_pos;
        std::vector<int> rampdown_start_pos;
        int startposindex_rampup = -1;
        int startposindex_rampdown = -1;
        std::vector<tag_t> tags;
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

        get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items);
        for(int i=0; i<tags.size(); i++) {
            if (pmt::equal(tags[i].key, pmt::intern("ramp_up"))) {
                rampup_start_pos.push_back(tags[i].offset - nitems_read(0));
                startposindex_rampup = 0;
            }
            else if (pmt::equal(tags[i].key, pmt::intern("ramp_down"))) {
                rampdown_start_pos.push_back(tags[i].offset - nitems_read(0));
                startposindex_rampdown = 0;
            }
        }

        for(int i=0;i<noutput_items;i++) {
            if (startposindex_rampup != -1 && i == rampup_start_pos[startposindex_rampup] && d_state == 0) {
                d_state = 1;
                d_rampcounter = 0;
                startposindex_rampup++;
                if (startposindex_rampup == rampup_start_pos.size()) {
                    startposindex_rampup = -1;
                }
            }
            if (startposindex_rampdown != -1 && i == rampdown_start_pos[startposindex_rampdown] && d_state == 0) {
                d_state = 2;
                d_rampcounter = 0;
                startposindex_rampdown++;
                if (startposindex_rampdown == rampdown_start_pos.size()) {
                    startposindex_rampdown = -1;
                }
            }
            if(d_state == 1) {
                d_scale = 1 - exp(-d_rampcounter*d_alpha);
                d_rampcounter++;
                if(d_scale > 0.995){
                    d_scale = 1;
                    d_state = 0;
                }
            }
            else if(d_state == 2) {
                d_scale = exp(-d_rampcounter*d_alpha);
                d_rampcounter++;
                if(d_scale < 0.005){
                    d_scale = 0;
                    d_state = 0;
                }
            }
            out[i] = in[i] * d_scale;
        }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

