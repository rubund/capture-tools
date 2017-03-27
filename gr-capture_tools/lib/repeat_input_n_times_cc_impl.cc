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
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        d_memory = new gr_complex[max_samples];
        d_memory_cnt = 0;
        d_max_samples = max_samples;
        d_n_times = n_times;
        d_state = 0;
        d_playback_cnt = 0;
        d_playback_time = 0;
        d_done_input = 0;
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
            if (d_done_input)
                ninput_items_required[0] = 0;
            else
                ninput_items_required[0] = noutput_items;
        }
        else if (d_state == 1) {
            ninput_items_required[0] = 0;
        }
        printf("A: To produce: %d\n", noutput_items);
        printf("A: Required items: %d\n", ninput_items_required[0]);
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
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

        printf("B: Input items: %d\n", ninput_items[0]);
        printf("B: Output items: %d\n", noutput_items);
        produced = 0;
        consumed = 0;

        if (d_state == 0){
            if(ninput_items[0] == 1){
                d_done_input = 1;
                d_state = 1;
            }
            int minelem = std::min(noutput_items, ninput_items[0]);
            if((d_memory_cnt + minelem) < d_max_samples) {
                memcpy(d_memory+(d_memory_cnt), in, sizeof(gr_complex) * (minelem));
                memcpy(out, in, sizeof(gr_complex) * (minelem));
                d_memory_cnt += minelem;
                printf("d_memory_cnt: %d\n", d_memory_cnt);
                for(int j=0;j<d_memory_cnt;j++) {
                    std::cout << d_memory[j] << " ";
                }
                std::cout << std::endl;
                produced = minelem;
                consumed = minelem;
            }
            else {
                memcpy(d_memory+(d_memory_cnt), in, sizeof(gr_complex) * (d_max_samples - d_memory_cnt));
                memcpy(out, in, sizeof(gr_complex) * (d_max_samples - d_memory_cnt));
                d_memory_cnt = d_max_samples;
                produced = d_max_samples - d_memory_cnt;
                consumed = d_max_samples - d_memory_cnt;
            }
        }
        else if (d_state == 1){
            printf("in state 1\n");
            if((d_playback_cnt + noutput_items) < d_memory_cnt) {
                memcpy(out, d_memory+(d_playback_cnt), sizeof(gr_complex) * noutput_items);
                d_playback_cnt += noutput_items;
                produced = noutput_items;
                consumed = 0;
            }
            else {
                printf("difference: %d\n", (d_memory_cnt - d_playback_cnt));
                memcpy(out, d_memory+(d_playback_cnt), sizeof(gr_complex) * (d_memory_cnt - d_playback_cnt));
                produced = d_memory_cnt - d_playback_cnt;
                consumed = 0;
                d_playback_cnt = 0;
                d_playback_time++;
                if(d_playback_time >= (d_n_times - 1)){
                    d_state = 2;
                }
                //else {
                //    memcpy(out+((d_memory_cnt - d_playback_cnt)), d_memory+sizeof(gr_complex)*(d_playback_cnt), sizeof(gr_complex) * (noutput_items-(d_memory_cnt - d_playback_cnt)));
                //    d_playback_cnt += noutput_items;
                //    produced = noutput_items;
                //    consumed = 0;
                //}
            }
        }
        else if (d_state == 2) {
            consumed = 0;
            produced = -1;
        }

        printf("Consumed: %d\n", consumed);
        printf("Produced: %d\n", produced);

      consume_each (consumed);

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace capture_tools */
} /* namespace gr */

