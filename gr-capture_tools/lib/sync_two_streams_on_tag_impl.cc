/* -*- c++ -*- */
/*
 * Copyright 2022 gr-capture_tools author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "sync_two_streams_on_tag_impl.h"

namespace gr {
  namespace capture_tools {

    using input_type = gr_complex;
    using output_type = gr_complex;
    sync_two_streams_on_tag::sptr
    sync_two_streams_on_tag::make(const std::string &tag_str)
    {
      return gnuradio::make_block_sptr<sync_two_streams_on_tag_impl>(
        tag_str);
    }


    /*
     * The private constructor
     */
    sync_two_streams_on_tag_impl::sync_two_streams_on_tag_impl(const std::string &tag_str)
      : gr::block("sync_two_streams_on_tag",
              gr::io_signature::make(2 /* min inputs */, 2 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(2 /* min outputs */, 2 /*max outputs */, sizeof(output_type)))
    {
      d_tag_str = pmt::intern(tag_str);
      set_tag_propagation_policy(TPP_ONE_TO_ONE);
      d_has_synced = 1;
      d_ind_ch0 = 0;
      d_ind_ch1 = 0;
      d_to_skip = 0;
    }

    /*
     * Our virtual destructor.
     */
    sync_two_streams_on_tag_impl::~sync_two_streams_on_tag_impl()
    {
    }

    void
    sync_two_streams_on_tag_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      int extra = d_to_skip >= 0 ? d_to_skip : -d_to_skip;
      ninput_items_required[0] = noutput_items + extra;
      ninput_items_required[1] = noutput_items + extra;
    }

    void sync_two_streams_on_tag_impl::reset_sync()
    {
      d_has_synced = 0;
      d_ind_ch0 = 0;
      d_ind_ch1 = 0;
      d_to_skip = 0;
    }

    int
    sync_two_streams_on_tag_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      auto in0 = static_cast<const input_type*>(input_items[0]);
      auto out0 = static_cast<output_type*>(output_items[0]);
      auto in1 = static_cast<const input_type*>(input_items[1]);
      auto out1 = static_cast<output_type*>(output_items[1]);

      std::vector<gr::tag_t> tags0;
      get_tags_in_range(tags0,
                        0,
                        nitems_read(0),
                        nitems_read(0) + ninput_items[0],
                        d_tag_str);
      std::vector<gr::tag_t> tags1;
      get_tags_in_range(tags1,
                        1,
                        nitems_read(1),
                        nitems_read(1) + ninput_items[1],
                        d_tag_str);

      if (!d_has_synced) {
        if (d_ind_ch0 == 0) {
          for (const auto& tag : tags0) {
            printf("Synchronized channel0 at %lu\n", tag.offset);
            d_int_ch0 = pmt::to_uint64(pmt::tuple_ref(tag.value, 0));
            d_frac_ch0 = pmt::to_double(pmt::tuple_ref(tag.value, 1));
            if (d_ind_ch1 > 0) {
              double timediff = d_frac_ch0 - d_frac_ch1;
              int nwrong = (int)(round(timediff / (1.0 / 4e6)));
              printf("timediff: %lf\n", timediff);
              d_to_skip = tag.offset - d_ind_ch1 - nwrong;
              d_has_synced = 1;
            }
            d_ind_ch0 = tag.offset;
            break;
          }
        }
        if (d_ind_ch1 == 0) {
          for (const auto& tag : tags1) {
            printf("Synchronized channel1 at %lu\n", tag.offset);
            d_int_ch1 = pmt::to_uint64(pmt::tuple_ref(tag.value, 0));
            d_frac_ch1 = pmt::to_double(pmt::tuple_ref(tag.value, 1));
            if (d_ind_ch0 > 0) {
              double timediff = d_frac_ch1 - d_frac_ch0;
              int nwrong = (int)(round(timediff / (1.0 / 4e6)));
              printf("timediff: %lf\n", timediff);
              d_to_skip = d_ind_ch0 - tag.offset + nwrong;
              d_has_synced = 1;
            }
            d_ind_ch1 = tag.offset;
            break;
          }
        }
      }

      if(d_to_skip != 0) {
        printf("toskip: %d\n", d_to_skip);
        printf("noutput_items: %d\n", noutput_items);
      }
      if (ninput_items[0] < noutput_items || ninput_items[1] < noutput_items) {
        printf("ERROR: not enough data supplued\n");
      }

      memcpy(out0, in0, sizeof(gr_complex)*noutput_items);
      memcpy(out1, in1, sizeof(gr_complex)*noutput_items);

      int64_t skipnow;
      if (d_to_skip > 0){
        if (noutput_items < d_to_skip){
          skipnow = noutput_items;
          d_to_skip = d_to_skip - noutput_items;
        }
        else {
          skipnow = d_to_skip;
          d_to_skip = 0;
        }
        consume (1, noutput_items-skipnow);
        consume (0, noutput_items);
        printf("Consuming %d from first\n", (noutput_items-skipnow));
        printf("Consuming %d from second\n", (noutput_items));
        printf("Producing %d\n", noutput_items);
      }
      else if (d_to_skip < 0) {
        if (noutput_items < (-d_to_skip)){
          skipnow = noutput_items;
          d_to_skip = d_to_skip + noutput_items;
        }
        else {
          skipnow = -d_to_skip;
          d_to_skip = 0;
        }
        consume (1, noutput_items);
        consume (0, noutput_items-skipnow);
        printf("Consuming %d from first\n", (noutput_items));
        printf("Consuming %d from second\n", (noutput_items-skipnow));
        printf("Producing %d\n", noutput_items);
      }
      else {
        consume (0, noutput_items);
        consume (1, noutput_items);
      }

      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */
