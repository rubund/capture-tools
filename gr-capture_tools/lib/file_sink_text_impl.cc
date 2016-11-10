/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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
#include "file_sink_text_impl.h"

namespace gr {
  namespace capture_tools {

    file_sink_text::sptr
    file_sink_text::make(int size, int nelem, std::string filename)
    {
      return gnuradio::get_initial_sptr
        (new file_sink_text_impl(size, nelem, filename));
    }

    /*
     * The private constructor
     */
    file_sink_text_impl::file_sink_text_impl(int size, int nelem, std::string filename)
      : gr::sync_block("file_sink_text",
              gr::io_signature::make(1, 1, size*nelem),
              gr::io_signature::make(0, 0, 0))
    {
		d_size = size;
		d_nelem = nelem;
		d_filename = filename;
		d_counter = 1;
	}

    /*
     * Our virtual destructor.
     */
    file_sink_text_impl::~file_sink_text_impl()
    {
    }

    int
    file_sink_text_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const char  *bin = (const char *) input_items[0];
      const float *fin = (const float *) input_items[0];
      const gr_complex *cin = (const gr_complex *) input_items[0];

		
		for(int i=0; i<noutput_items; i++){
			if (d_nelem != 1) {
				std::cout << d_counter << ":" << std::endl;
				std::cout << "[";
				for(int j=0; j<d_nelem; j++){
					if(d_size == 1)
						std::cout << ((int)(bin[i*d_nelem+j]));
					if(d_size == 4)
						std::cout << (fin[i*d_nelem+j]);
					if(d_size == 8)
						std::cout << (cin[i*d_nelem+j]);
					if (j != d_nelem-1)
						std::cout << ", ";
				}
				std::cout << "]" << std::endl << std::endl;
			}
			else {
				if(d_size == 1){
					std::cout << ((int)bin[i]) << std::endl;
				}
				else if(d_size == 4) {
					std::cout << fin[i] << std::endl;
				}
				else if(d_size == 8)
					std::cout << cin[i] << std::endl;
			}
			d_counter++;
		}

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

