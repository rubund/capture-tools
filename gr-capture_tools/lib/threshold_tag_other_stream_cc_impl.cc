/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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


#include <gnuradio/io_signature.h>
#include "threshold_tag_other_stream_cc_impl.h"

namespace gr {
  namespace capture_tools {

    threshold_tag_other_stream_cc::sptr
    threshold_tag_other_stream_cc::make(float low_thresh, float high_thresh, int vlen)
    {
      return gnuradio::make_block_sptr<threshold_tag_other_stream_cc_impl>(
        low_thresh, high_thresh, vlen);
    }

    /*
     * The private constructor
     */
    threshold_tag_other_stream_cc_impl::threshold_tag_other_stream_cc_impl(float low_thresh, float high_thresh, int vlen)
      : gr::sync_block("threshold_tag_other_stream_cc",
              gr::io_signature::make2(2, 2, sizeof(gr_complex) * vlen, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(gr_complex) * vlen)),
            d_low_thresh(low_thresh), d_high_thresh(high_thresh), d_is_above(false),
            d_just_started(true)
    {
        d_vlen = vlen;
        d_going_high_tag = pmt::intern("going_high");
        d_going_low_tag = pmt::intern("going_low");
    }

    /*
     * Our virtual destructor.
     */
    threshold_tag_other_stream_cc_impl::~threshold_tag_other_stream_cc_impl()
    {
    }

    void
    threshold_tag_other_stream_cc_impl::set_going_high_tag(const std::string &tag_str)
    {
      d_going_high_tag = pmt::intern(tag_str);
    }

    void
    threshold_tag_other_stream_cc_impl::set_going_low_tag(const std::string &tag_str)
    {
      d_going_low_tag = pmt::intern(tag_str);
    }

    void
    threshold_tag_other_stream_cc_impl::set_low_thresh(float val)
    {
      d_low_thresh = val;
    }

    void
    threshold_tag_other_stream_cc_impl::set_high_thresh(float val)
    {
      d_high_thresh = val;
    }

    int
    threshold_tag_other_stream_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      const float *in_float = (const float *) input_items[1];
      gr_complex *out = (gr_complex *) output_items[0];

      for(int i=0;i<noutput_items;i++) {
        if(d_just_started) {
          if (in_float[i] > d_high_thresh) {
            d_is_above = true;
            d_just_started = false;
          }
          if (in_float[i] < d_low_thresh) {
            d_is_above = false;
            d_just_started = false;
          }
        }
        else {
          if (d_is_above) {
            if(in_float[i] < d_low_thresh) {
              d_is_above = false;
              add_item_tag(0, nitems_written(0) + i, d_going_low_tag, pmt::intern(""), pmt::intern(""));
            }
          }
          else {
            if(in_float[i] > d_high_thresh) {
              d_is_above = true;
              add_item_tag(0, nitems_written(0) + i, d_going_high_tag, pmt::intern(""), pmt::intern(""));
            }
          }
        }
      }
      memcpy(out, in, sizeof(gr_complex)*d_vlen*noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

