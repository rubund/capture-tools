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


#ifndef INCLUDED_CAPTURE_TOOLS_FILE_SOURCE_ON_TAG_H
#define INCLUDED_CAPTURE_TOOLS_FILE_SOURCE_ON_TAG_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>
#include <gnuradio/blocks/file_source.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Output samples from file when tag is present in input
     * \ingroup capture_tools
     *
     * \details
     * The input signal is used to sync. Its values have no significance.
     * However, a tag 'tag_str' at the input causes the file to be sent
     * to the output.
     *
     * The tags 'file_begin' and 'file_end' are also added to the output stream.
     */
    class CAPTURE_TOOLS_API file_source_on_tag : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<file_source_on_tag> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of capture_tools::file_source_on_tag.
       *
       * To avoid accidental use of raw pointers, capture_tools::file_source_on_tag's
       * constructor is in a private implementation
       * class. capture_tools::file_source_on_tag::make is the public interface for
       * creating new instances.
       */
      static sptr make(size_t itemsize, const char *filename, const std::string &tag_str, bool filename_in_message=false);

      virtual bool seek(long seek_point, int whence) = 0;
      virtual void open(const char *filename, bool repeat) = 0;
      virtual void close() = 0;

    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_FILE_SOURCE_ON_TAG_H */

