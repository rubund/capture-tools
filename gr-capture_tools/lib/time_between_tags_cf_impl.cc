/* -*- c++ -*- */
/*
 * Copyright 2022 gr-capture_tools author.
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
#include "time_between_tags_cf_impl.h"

namespace gr {
  namespace capture_tools {

    time_between_tags_cf::sptr
    time_between_tags_cf::make(const std::string &tag_name, int decimate)
    {
      return gnuradio::get_initial_sptr
        (new time_between_tags_cf_impl(tag_name, decimate));
    }


    /*
     * The private constructor
     */
    time_between_tags_cf_impl::time_between_tags_cf_impl(const std::string &tag_name, int decimate)
      : gr::block("time_between_tags_cf",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 1, sizeof(float)))
    {
      d_last_offset = -1;
      d_sample_rate = 4000000;
      d_scaleus = ((double)1.0)/((double)d_sample_rate) / ((double)1e-6);
      d_currentval = -1;
      printf("Scaling is %f\n" , d_scaleus);
    }


    /*
     * Our virtual destructor.
     */
    time_between_tags_cf_impl::~time_between_tags_cf_impl()
    {
    }

    void
    time_between_tags_cf_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int
    time_between_tags_cf_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      float *out = (float *) output_items[0];

      std::vector<tag_t> tags;

      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + ninput_items[0], pmt::mp("address"));
      int produced = 0;
      int until_index = 0;
      int last_index = 0;

      for(int i=0;i<tags.size();i++){
          //if (pmt::equal(tags[i].key, d_tag_key)) {
              if (d_last_offset != -1 &&   d_last_offset !=   tags[i].offset) {
                until_index = tags[i].offset - nitems_read(0);
                uint64_t diff = tags[i].offset - d_last_offset;
                float timeus = ((double)diff) * d_scaleus;
                for(int j=last_index;j<until_index;j++)
                  out[j] = d_currentval;
                d_currentval = timeus;
                last_index = until_index;
                //printf("Number of samples since previous address: %llu\n", diff);
              }
              d_last_offset = tags[i].offset;
          //}
      }

      consume_each (noutput_items);
      
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

