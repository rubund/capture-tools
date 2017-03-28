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
#include "add_before_and_after_cc_impl.h"

namespace gr {
  namespace capture_tools {

    add_before_and_after_cc::sptr
    add_before_and_after_cc::make(int n_before, int n_after, int val_before, int val_after)
    {
      return gnuradio::get_initial_sptr
        (new add_before_and_after_cc_impl(n_before, n_after, val_before, val_after));
    }

    /*
     * The private constructor
     */
    add_before_and_after_cc_impl::add_before_and_after_cc_impl(int n_before, int n_after, int val_before, int val_after)
      : gr::block("add_before_and_after_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        d_n_before = n_before;
        d_n_after = n_after;
        d_val_before = val_before;
        d_val_after = val_after;
        d_cnt = 0;
        d_state = 0;
        set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    add_before_and_after_cc_impl::~add_before_and_after_cc_impl()
    {
    }

    void
    add_before_and_after_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
        if (d_state == 2)
            ninput_items_required[0] = 0;
        else
            ninput_items_required[0] = noutput_items;
    }

    int
    add_before_and_after_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        int consumed;
        int produced;
        std::vector<tag_t> tags;
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

        if(d_state == 0) {
            int minval = std::min((d_n_before-d_cnt), noutput_items);
            for(int i=0;i<minval;i++) {
                out[i] = d_val_before;
                d_cnt++;
            }
            if (d_cnt >= d_n_before) {
                d_state = 1;
                d_cnt = 0;
            }
            consumed = 0;
            produced = minval;
        }
        else if (d_state == 1) {
            memcpy(out, in, sizeof(gr_complex) * noutput_items);
            get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items);
            for(int i=0; i<tags.size(); i++) {
                add_item_tag(0, nitems_written(0) + (tags[i].offset-nitems_read(0)), tags[i].key, tags[i].value, tags[i].srcid);
            }
            consumed = noutput_items;
            produced = noutput_items;
            if (ninput_items[0] == 1) {
                d_state = 2;
            }
        }
        else if (d_state == 2) {
            int minval = std::min((d_n_after-d_cnt), noutput_items);
            for(int i=0;i<minval;i++) {
                out[i] = d_val_after;
                d_cnt++;
            }
            if (d_cnt >= d_n_after) {
                d_state = 3;

            }
            consumed = 0;
            produced = minval;
        }
        else if (d_state == 3) {
            consumed = 0;
            produced = -1;
        }

      consume_each (consumed);

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace capture_tools */
} /* namespace gr */

