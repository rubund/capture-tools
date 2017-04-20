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
#include "repeat_input_n_times_cc_impl.h"

namespace gr {
  namespace capture_tools {

    repeat_input_n_times_cc::sptr
    repeat_input_n_times_cc::make(int n_times, int max_samples)
    {
      return gnuradio::get_initial_sptr
        (new repeat_input_n_times_cc_impl(n_times, max_samples));
    }

    /*
     * The private constructor
     */
    repeat_input_n_times_cc_impl::repeat_input_n_times_cc_impl(int n_times, int max_samples)
      : gr::block("repeat_input_n_times_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))), d_fromtag(0)
    {
        d_memory = new gr_complex[max_samples];
        d_memory_cnt = 0;
        d_max_samples = max_samples;
        d_n_times = n_times;
        d_state = 0;
        d_playback_cnt = 0;
        d_playback_time = 0;
        set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    repeat_input_n_times_cc_impl::~repeat_input_n_times_cc_impl()
    {
        delete d_memory;
    }

    void
    repeat_input_n_times_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        if (d_state == 0) {
            ninput_items_required[0] = noutput_items;
        }
        else if (d_state == 1 || d_state == 2) {
            ninput_items_required[0] = 0;
        }
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    repeat_input_n_times_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        int produced;
        int consumed;
        int end_pos = -1;
        std::vector<tag_t> tags;
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

        produced = 0;
        consumed = 0;

        if (d_state == 0){
            int minelem = std::min(noutput_items, ninput_items[0]);
            get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + minelem);
            for(int i=0;i<tags.size();i++){
                if (pmt::equal(tags[i].key, pmt::intern("end"))) {
                    end_pos = tags[i].offset - nitems_read(0) + 1;
                    break;
                }
            }
            int minelem3;
            if (end_pos != -1)
                minelem3 = std::min(minelem, end_pos);
            else
                minelem3 = minelem;
            if((d_memory_cnt + minelem3) < d_max_samples) {
                memcpy(d_memory+(d_memory_cnt), in, sizeof(gr_complex) * (minelem3));
                memcpy(out, in, sizeof(gr_complex) * (minelem3));
                get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + minelem3);
                for(int i=0; i<tags.size(); i++) {
                    add_item_tag(0, nitems_written(0) + (tags[i].offset-nitems_read(0)), tags[i].key, tags[i].value, tags[i].srcid);
                    d_all_tags.push_back(tags[i]);
                }
                d_memory_cnt += minelem3;
                produced = minelem3;
                consumed = minelem3;
            }
            else {
                memcpy(d_memory+(d_memory_cnt), in, sizeof(gr_complex) * (d_max_samples - d_memory_cnt));
                int minelem2 = std::min((d_max_samples - d_memory_cnt), minelem3);
                memcpy(out, in, sizeof(gr_complex) * minelem2);
                get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + minelem2);
                for(int i=0; i<tags.size(); i++) {
                    add_item_tag(0, nitems_written(0) + (tags[i].offset-nitems_read(0)), tags[i].key, tags[i].value, tags[i].srcid);
                    d_all_tags.push_back(tags[i]);
                }
                produced = minelem2;
                d_memory_cnt = d_max_samples;
                consumed = ninput_items[0];
                d_state = 1;
            }
            if (minelem3 == end_pos)
                d_state = 1;
        }
        else if (d_state == 1){
            if((d_playback_cnt + noutput_items) < d_memory_cnt) {
                for(int i=d_fromtag;i<d_all_tags.size(); i++){
                    if(d_all_tags[i].offset < d_playback_cnt) {
                        d_fromtag = i;
                    }
                    else if (d_all_tags[i].offset >= (d_playback_cnt + noutput_items)) {
                        break;
                    }
                    else {
                        add_item_tag(0, nitems_written(0) + (d_all_tags[i].offset-d_playback_cnt), d_all_tags[i].key, d_all_tags[i].value, d_all_tags[i].srcid);
                    }
                }
                memcpy(out, d_memory+(d_playback_cnt), sizeof(gr_complex) * noutput_items);
                d_playback_cnt += noutput_items;
                produced = noutput_items;
                consumed = 0;
            }
            else {
                for(int i=d_fromtag;i<d_all_tags.size(); i++){
                    if(d_all_tags[i].offset < d_playback_cnt) {
                        d_fromtag = i;
                    }
                    else if (d_all_tags[i].offset >= d_memory_cnt) {
                        break;
                    }
                    else {
                        add_item_tag(0, nitems_written(0) + (d_all_tags[i].offset-d_playback_cnt), d_all_tags[i].key, d_all_tags[i].value, d_all_tags[i].srcid);
                    }
                }
                int postag = nitems_written(0) + d_memory_cnt - d_playback_cnt - 1;
                memcpy(out, d_memory+(d_playback_cnt), sizeof(gr_complex) * (d_memory_cnt - d_playback_cnt));
                produced = d_memory_cnt - d_playback_cnt;
                consumed = 0;
                d_playback_cnt = 0;
                d_playback_time++;
                d_fromtag = 0;
                if(d_n_times != 0 && (d_playback_time >= (d_n_times - 1))){
                    d_state = 2;
                    add_item_tag(0, postag, pmt::intern("end"), pmt::intern("repeat_input_n_times_cc"), pmt::intern(""));
                }
                else {
                    add_item_tag(0, postag, pmt::intern("repeat"), pmt::intern("repeat_input_n_times_cc"), pmt::intern(""));
                }
            }
        }
        else if (d_state == 2) {
            consumed = 0;
            produced = -1;
        }


      consume_each (consumed);

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace capture_tools */
} /* namespace gr */

