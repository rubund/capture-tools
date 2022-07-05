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

#ifndef INCLUDED_CAPTURE_TOOLS_FILE_SINK_ON_TAG_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_FILE_SINK_ON_TAG_IMPL_H

#include <capture_tools/file_sink_on_tag.h>

namespace gr {
  namespace capture_tools {

    class file_sink_on_tag_impl : public file_sink_on_tag
    {
     private:
      size_t	d_itemsize;
      char *d_buffered;
      int d_index_buffered;
      int d_number_buffered;
      int d_number_to_write;
      int d_numbers_written;
      bool d_is_writing;
      pmt::pmt_t d_search_tag;
      char *d_null_to_write;
      FILE *d_metadata_fp;
      bool d_metadata_enabled;
      uint64_t d_capture_cnt;
      bool d_extra_burst_output;

     public:
      file_sink_on_tag_impl(size_t itemsize, const char *filename, int number_buffered, int number_to_write, const std::string &tag_str);
      ~file_sink_on_tag_impl();

      void set_metadata_output(bool enable, const char *filename);
      void set_burst_extra_data_output(bool enable);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_FILE_SINK_ON_TAG_IMPL_H */

