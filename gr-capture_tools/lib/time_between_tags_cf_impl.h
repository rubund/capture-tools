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

#ifndef INCLUDED_CAPTURE_TOOLS_TIME_BETWEEN_TAGS_CF_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_TIME_BETWEEN_TAGS_CF_IMPL_H

#include <capture_tools/time_between_tags_cf.h>

namespace gr {
  namespace capture_tools {

    class time_between_tags_cf_impl : public time_between_tags_cf
    {
     private:
      // Nothing to declare in this block.
      uint64_t d_last_offset;
      int d_sample_rate;
      double d_scaleus;
      float d_currentval;

     public:
      time_between_tags_cf_impl(const std::string &tag_name, int decimate);
      ~time_between_tags_cf_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_TIME_BETWEEN_TAGS_CF_IMPL_H */

