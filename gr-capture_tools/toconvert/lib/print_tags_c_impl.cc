/* -*- c++ -*- */
/*
 * Copyright 2021 gr-capture_tools author.
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
#include "print_tags_c_impl.h"

namespace gr {
  namespace capture_tools {

    print_tags_c::sptr
    print_tags_c::make()
    {
      return gnuradio::get_initial_sptr
        (new print_tags_c_impl());
    }


    /*
     * The private constructor
     */
    print_tags_c_impl::print_tags_c_impl()
      : gr::sync_block("print_tags_c",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 0, 0))
    {}

    /*
     * Our virtual destructor.
     */
    print_tags_c_impl::~print_tags_c_impl()
    {
    }

    int
    print_tags_c_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      std::vector<tag_t> tags;

      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items);
      for(int i=0;i<tags.size();i++) {
        std::cout << "Found tag: " << pmt::symbol_to_string(tags[i].key);
        std::cout << " (index: " << tags[i].offset << ")" << std::endl;
      }


      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

