/* -*- c++ -*- */
/* 
 * Copyright 2016 Free Software Foundation, Inc
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


#ifndef INCLUDED_CAPTURE_TOOLS_FFT_BURST_TAGGER_H
#define INCLUDED_CAPTURE_TOOLS_FFT_BURST_TAGGER_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief <+description of block+>
     * \ingroup capture_tools
     *
     */
    class CAPTURE_TOOLS_API fft_burst_tagger : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<fft_burst_tagger> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::fft_burst_tagger.
       *
       * To avoid accidental use of raw pointers, capture_tools::fft_burst_tagger's
       * constructor is in a private implementation
       * class. capture_tools::fft_burst_tagger::make is the public interface for
       * creating new instances.
       */
      static sptr make(float center_frequency, int fft_size, int sample_rate,
                            int burst_pre_len, int burst_post_len,
                            int burst_width, int max_bursts=0, float threshold=7,
                            int history_size=512, bool debug=false);

      virtual uint64_t get_n_tagged_bursts() = 0;

      virtual void set_threshold(float threshold) = 0;
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_FFT_BURST_TAGGER_H */

