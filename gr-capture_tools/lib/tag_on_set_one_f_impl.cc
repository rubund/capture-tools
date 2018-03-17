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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "tag_on_set_one_f_impl.h"

namespace gr {
  namespace capture_tools {

    tag_on_set_one_f::sptr
    tag_on_set_one_f::make(const std::string &tag_str)
    {
      return gnuradio::get_initial_sptr
        (new tag_on_set_one_f_impl(tag_str));
    }

    /*
     * The private constructor
     */
    tag_on_set_one_f_impl::tag_on_set_one_f_impl(const std::string &tag_str)
      : gr::sync_block("tag_on_set_one_f",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(float))),
            d_add_tag_now(false), d_set_now(false)
    {
        d_tag = pmt::intern(tag_str);
        d_tag_val = pmt::intern("");
    }

    /*
     * Our virtual destructor.
     */
    tag_on_set_one_f_impl::~tag_on_set_one_f_impl()
    {
    }

    void
    tag_on_set_one_f_impl::set_value(int val)
    {
        if(val == 1) {
            d_add_tag_now = true;
            d_set_now = true;
        }
        else {
            d_set_now = false;
        }
    }

    int
    tag_on_set_one_f_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      float *out = (float *) output_items[0];

      memset(out, 0, sizeof(float)*noutput_items);

      if (d_add_tag_now) {
        add_item_tag(0, nitems_written(0), d_tag, d_tag_val);
        d_add_tag_now = false;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

