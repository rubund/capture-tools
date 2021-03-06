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

#ifndef INCLUDED_BLOCKS_file_source_tag_end_H
#define INCLUDED_BLOCKS_file_source_tag_end_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace capture_tools {

    /*!
     * \brief Add a tag to the last sample of the file
     * \ingroup capture_tools
     *
     * \details
     * Same as file_source, but a tag 'end' is added to the last sample in the file.
     */
    class CAPTURE_TOOLS_API file_source_tag_end : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<file_source_tag_end> sptr;

      /*!
       * \brief Create a file source.
       *
       * Opens \p filename as a source of items into a flowgraph. The
       * data is expected to be in binary format, item after item. The
       * \p itemsize of the block determines the conversion from bits
       * to items.
       *
       * If \p repeat is turned on, the file will repeat the file after
       * it's reached the end.
       *
       * \param itemsize	the size of each item in the file, in bytes
       * \param filename	name of the file to source from
       * \param repeat	repeat file from start
       */
      static sptr make(size_t itemsize, const char *filename, bool repeat = false);

      /*!
       * \brief seek file to \p seek_point relative to \p whence
       *
       * \param seek_point	sample offset in file
       * \param whence	one of SEEK_SET, SEEK_CUR, SEEK_END (man fseek)
       */
      virtual bool seek(long seek_point, int whence) = 0;

      /*!
       * \brief Opens a new file.
       *
       * \param filename	name of the file to source from
       * \param repeat	repeat file from start
       */
      virtual void open(const char *filename, bool repeat) = 0;

      /*!
       * \brief Close the file handle.
       */
      virtual void close() = 0;
    };

  } /* namespace capture_tools */
} /* namespace gr */

#endif /* INCLUDED_BLOCKS_file_source_tag_end_H */
