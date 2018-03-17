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

#ifndef INCLUDED_CAPTURE_TOOLS_FILE_SOURCE_ON_TAG_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_FILE_SOURCE_ON_TAG_IMPL_H

#include <capture_tools/file_source_on_tag.h>
#include <boost/thread/mutex.hpp>

namespace gr {
  namespace capture_tools {

    class file_source_on_tag_impl : public file_source_on_tag
    {
     private:
      size_t d_itemsize;
      pmt::pmt_t d_search_tag;
      bool d_filename_in_message;
      pmt::pmt_t d_tag_start;
      pmt::pmt_t d_tag_stop;
      pmt::pmt_t d_tag_val;

      FILE *d_fp;
      FILE *d_new_fp;
      bool d_repeat;
      bool d_updated;
      boost::mutex fp_mutex;

      bool d_is_in_file;

      void do_update();

     public:
      file_source_on_tag_impl(size_t itemsize, const char *filename, const std::string &tag_str, bool filename_in_message);
      ~file_source_on_tag_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);


      void open(const char *filename, bool repeat);
      bool seek(long seek_point, int whence);
      void close();

    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_FILE_SOURCE_ON_TAG_IMPL_H */

