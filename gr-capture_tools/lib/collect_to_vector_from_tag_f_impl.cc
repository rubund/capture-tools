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
#include "collect_to_vector_from_tag_f_impl.h"

namespace gr {
  namespace capture_tools {

    collect_to_vector_from_tag_f::sptr
    collect_to_vector_from_tag_f::make(int len)
    {
      return gnuradio::get_initial_sptr
        (new collect_to_vector_from_tag_f_impl(len));
    }

    /*
     * The private constructor
     */
    collect_to_vector_from_tag_f_impl::collect_to_vector_from_tag_f_impl(int len)
      : gr::block("collect_to_vector_from_tag_f",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)*len))
    {
        d_len = len;
        d_state = 0;
    }

    /*
     * Our virtual destructor.
     */
    collect_to_vector_from_tag_f_impl::~collect_to_vector_from_tag_f_impl()
    {
    }

    void
    collect_to_vector_from_tag_f_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items * d_len;
    }

    int
    collect_to_vector_from_tag_f_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        std::vector<tag_t> tags;
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];
        int next_start = -1;
        int tagnum = -1;

        if(d_state == 0) {
            get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items);
            for(int i=0;i<tags.size();i++) {
                if(pmt::equal(tags[i].key, pmt::intern("burst")) && pmt::equal(tags[i].value, pmt::from_bool(1))) {
                    printf("Found burst tag True\n");
                    int index = tags[i].offset - nitems_read(0);
                    d_state = 1;
                    d_cnt = 0;
                    if (index > 0) {
                        consume_each(index);
                        return 0; // Align so that first input sample is at burst tag
                    }
                    break;
                }
            }
        }

        if (d_state == 1) {
            memcpy(out, in, sizeof(float)*d_len);
            d_cnt += d_len;
            d_state = 0;
            consume_each(d_len);
            return 1;
        }


      consume_each (ninput_items[0]);

      // Tell runtime system how many output items we produced.
      return 0;
    }

  } /* namespace capture_tools */
} /* namespace gr */

