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
#include "add_at_tag_cc_impl.h"

namespace gr {
  namespace capture_tools {

    add_at_tag_cc::sptr
    add_at_tag_cc::make(int n_min, int n_max, gr_complex val, const char *tag_key)
    {
      return gnuradio::get_initial_sptr
        (new add_at_tag_cc_impl(n_min, n_max, val, tag_key));
    }

    /*
     * The private constructor
     */
    add_at_tag_cc_impl::add_at_tag_cc_impl(int n_min, int n_max, gr_complex val, const char *tag_key)
      : gr::block("add_at_tag_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        d_n_min = n_min;
        d_n_max = n_max;
        d_val = val;
        d_tag_key =  pmt::string_to_symbol(tag_key);
        d_state = 0;
        d_cnt_added = 0;
        set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    add_at_tag_cc_impl::~add_at_tag_cc_impl()
    {
    }

    void
    add_at_tag_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int
    add_at_tag_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        std::vector<tag_t> tags;
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
        int add_index = -1;
        int consumed = 0;
        int produced = 0;

        get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + ninput_items[0]);
        for(int i=0;i<tags.size();i++){
            if (pmt::equal(tags[i].key, d_tag_key)) {
                add_index = tags[i].offset - nitems_read(0);
                break;
            }
        }

        if (d_state == 0) {
            int minval = std::min(noutput_items, ninput_items[0]);
            for(int i=0; i<minval; i++) {
                out[i] = in[i];
                consumed++;
                produced++;
                if (i == add_index) {
                    d_state = 1;
                    d_cnt_added = 0;
                    int randval = rand() % (d_n_max - d_n_min + 1);
                    d_n = d_n_min + randval;
                    break;
                }
            }
            for(int i=0; i<tags.size(); i++) {
                if ((tags[i].offset - nitems_read(0)) < consumed)
                    add_item_tag(0, nitems_written(0) + (tags[i].offset-nitems_read(0)), tags[i].key, tags[i].value, tags[i].srcid);
            }
        }
        else if (d_state == 1){
            for(int i=0; i<noutput_items; i++) {
                out[i] = d_val;
                produced++;
                d_cnt_added++;
                if(d_cnt_added >= d_n){
                    d_state = 0;
                    break;
                }
            }
        }

      consume_each (consumed);

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace capture_tools */
} /* namespace gr */

