/* -*- c++ -*- */
/*
 * Copyright 2022 gr-capture_tools author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_CAPTURE_TOOLS_SYNC_TWO_STREAMS_ON_TAG_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_SYNC_TWO_STREAMS_ON_TAG_IMPL_H

#include <gnuradio/capture_tools/sync_two_streams_on_tag.h>

namespace gr {
  namespace capture_tools {

    class sync_two_streams_on_tag_impl : public sync_two_streams_on_tag
    {
     private:
      // Nothing to declare in this block.
      pmt::pmt_t d_tag_str;
      bool d_has_synced;
      uint64_t d_ind_ch0;
      uint64_t d_ind_ch1;
      int64_t d_to_skip;
      double d_frac_ch0;
      double d_frac_ch1;
      uint64_t d_int_ch0;
      uint64_t d_int_ch1;

     public:
      sync_two_streams_on_tag_impl(const std::string &tag_str);
      ~sync_two_streams_on_tag_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

      void reset_sync();
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_SYNC_TWO_STREAMS_ON_TAG_IMPL_H */
