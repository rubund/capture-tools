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
#include "clock_recovery_mm_sync_ff_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <stdexcept>


namespace gr {
  namespace capture_tools {

    clock_recovery_mm_sync_ff::sptr
    clock_recovery_mm_sync_ff::make(float omega, float gain_omega, float mu, float gain_mu, float omega_relative_limit)
    {
      return gnuradio::get_initial_sptr
        (new clock_recovery_mm_sync_ff_impl(omega, gain_omega, mu, gain_mu, omega_relative_limit));
    }

    /*
     * The private constructor
     */
    clock_recovery_mm_sync_ff_impl::clock_recovery_mm_sync_ff_impl(float omega, float gain_omega, float mu, float gain_mu, float omega_relative_limit)
      : gr::sync_block("clock_recovery_mm_sync_ff",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float))),
    d_mu(mu), d_gain_mu(gain_mu), d_gain_omega(gain_omega),
    d_omega_relative_limit(omega_relative_limit),
    d_last_sample(0), d_interp(new filter::mmse_fir_interpolator_ff())
    {
      if(omega <  1)
    throw std::out_of_range("clock rate must be > 0");
      if(gain_mu <  0  || gain_omega < 0)
    throw std::out_of_range("Gains must be non-negative");

      set_omega(omega);         // also sets min and max omega
        d_mid = floor(d_omega / 2.0);
      //set_relative_rate (1.0 / omega);
      //enable_update_rate(true);  // fixes tag propagation through variable rate block
    }

    /*
     * Our virtual destructor.
     */
    clock_recovery_mm_sync_ff_impl::~clock_recovery_mm_sync_ff_impl()
    {
        delete d_interp;
    }

    static inline float
    slice(float x)
    {
      return x < 0 ? -1.0F : 1.0F;
    }

    void
    clock_recovery_mm_sync_ff_impl::set_omega (float omega)
    {
      d_omega = omega;
      d_omega_mid = omega;
      d_omega_lim = d_omega_mid * d_omega_relative_limit;
  }


    int
    clock_recovery_mm_sync_ff_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        float tmpout;
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      int ii = 0; // input index
      int oo = 0; // output index
      int ni = noutput_items - d_interp->ntaps(); // don't use more input than this
      float mm_val;

      while(oo < noutput_items && ii < ni ) {
    // produce output sample
    tmpout = d_interp->interpolate(&in[ii], d_mu);
    
    mm_val = slice(d_last_sample) * tmpout - slice(tmpout) * d_last_sample;
    d_last_sample = tmpout;

    d_omega = d_omega + d_gain_omega * mm_val;
    d_omega = d_omega_mid + gr::branchless_clip(d_omega-d_omega_mid, d_omega_lim);
    d_mu = d_mu + d_omega + d_gain_mu * mm_val;

    add_item_tag(0, nitems_written(0) + ii + d_mid, pmt::intern("strobe"), pmt::intern(""), pmt::intern(""));
    for(int j=0;j<((int)floor(d_mu));j++) {
        out[ii+j] = tmpout;
    }
    ii += (int)floor(d_mu);
    d_mu = d_mu - floor(d_mu);
    oo++;
      }

      //consume_each(ii);
      return ii;
    } 


  } /* namespace capture_tools */
} /* namespace gr */

