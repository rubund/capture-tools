/* -*- c++ -*- */
/*
 * Copyright 2022 gr-capture_tools author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_CAPTURE_TOOLS_SYNC_TWO_STREAMS_ON_TAG_H
#define INCLUDED_CAPTURE_TOOLS_SYNC_TWO_STREAMS_ON_TAG_H

#include <gnuradio/capture_tools/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief <+description of block+>
     * \ingroup capture_tools
     *
     */
    class CAPTURE_TOOLS_API sync_two_streams_on_tag : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<sync_two_streams_on_tag> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::sync_two_streams_on_tag.
       *
       * To avoid accidental use of raw pointers, capture_tools::sync_two_streams_on_tag's
       * constructor is in a private implementation
       * class. capture_tools::sync_two_streams_on_tag::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string &tag_str);

      virtual void reset_sync()=0;
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_SYNC_TWO_STREAMS_ON_TAG_H */
