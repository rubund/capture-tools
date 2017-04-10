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
#include "concatenate_cc_impl.h"

namespace gr {
  namespace capture_tools {

    concatenate_cc::sptr
    concatenate_cc::make(int inputs)
    {
      return gnuradio::get_initial_sptr
        (new concatenate_cc_impl(inputs));
    }

    /*
     * The private constructor
     */
    concatenate_cc_impl::concatenate_cc_impl(int inputs)
      : gr::block("concatenate_cc",
              gr::io_signature::make(1, -1, sizeof(gr_complex)),
              gr::io_signature::make(1,  1, sizeof(gr_complex)))
    {
        d_in = new gr_complex*[inputs];
        d_inputs = inputs;
        d_current = 0;
        set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    concatenate_cc_impl::~concatenate_cc_impl()
    {
    }

    void
    concatenate_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        for(int i=0;i<d_inputs;i++) {
          if(i == d_current)
              ninput_items_required[i] = noutput_items;
          else
              ninput_items_required[i] = 0;
        }
    }

    int
    concatenate_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        int consumed = 0;
        int produced = 0;
        std::vector<tag_t> tags;
        int end_pos = -1;
        for(int i=0;i<d_inputs;i++) {
          d_in[i] = (gr_complex *) input_items[i];
        }
      gr_complex *out = (gr_complex *) output_items[0];
        if (d_current == -1)
            return -1;

        get_tags_in_range(tags, d_current, nitems_read(d_current), nitems_read(d_current) + ninput_items[d_current]);
        for(int i=0;i<tags.size();i++){
            if (pmt::equal(tags[i].key, pmt::intern("end"))) {
                end_pos = tags[i].offset - nitems_read(d_current);
                break;
            }
        }
        int minval2;
        int minval;
        if (end_pos != -1)
            minval2 = std::min(end_pos + 1, ninput_items[d_current]);
        else
            minval2 = ninput_items[d_current];
        minval = std::min(minval2, noutput_items);
        memcpy(out, d_in[d_current], sizeof(gr_complex) * minval);
        consumed += minval;
        produced += minval;


      consume (d_current, consumed);

      if((end_pos+1) == minval){
        d_current++;
        if(d_current >= d_inputs) {
            add_item_tag(0, nitems_written(0) + end_pos - 1, pmt::intern("end"), pmt::intern("concatenate_cc"), pmt::intern(""));
            d_current = -1;
        }
      }

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace capture_tools */
} /* namespace gr */

