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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <iostream>
#include <ctime>
#include <string>
#include "file_sink_on_tag_impl.h"

namespace gr {
  namespace capture_tools {

    file_sink_on_tag::sptr
    file_sink_on_tag::make(size_t itemsize, const char *filename, int number_buffered, int number_to_write, const std::string &tag_str)
    {
      return gnuradio::get_initial_sptr
        (new file_sink_on_tag_impl(itemsize, filename, number_buffered, number_to_write, tag_str));
    }

    /*
     * The private constructor
     */
    file_sink_on_tag_impl::file_sink_on_tag_impl(size_t itemsize, const char *filename, int number_buffered, int number_to_write, const std::string &tag_str)
      : gr::sync_block("file_sink_on_tag",
              gr::io_signature::make(1, 1, itemsize),
              gr::io_signature::make(0, 0, 0)),
        gr::blocks::file_sink_base(filename, true, 0),
    d_itemsize(itemsize), d_number_buffered(number_buffered), d_number_to_write(number_to_write)
    {
       d_search_tag = pmt::intern(tag_str);

       if (!open(filename))
          throw std::runtime_error ("can't open file");
       d_buffered = new char[d_number_buffered*itemsize];
       memset(d_buffered,0,d_number_buffered*itemsize);
       d_index_buffered = 0;
       d_is_writing = 0;
       d_numbers_written = 0;
       d_null_to_write = new char[itemsize];
       d_metadata_fp = NULL;
       d_metadata_enabled = false;
       d_capture_cnt = 1;
       d_extra_burst_output = false;
       memset(d_null_to_write, 0, itemsize);
           printf("%d\n",number_to_write);
    }

    /*
     * Our virtual destructor.
     */
    file_sink_on_tag_impl::~file_sink_on_tag_impl()
    {
        delete d_buffered;
        if (d_metadata_fp != NULL) {
            fclose(d_metadata_fp);
            d_metadata_fp = NULL;
        }
    }

    void
    file_sink_on_tag_impl::set_metadata_output(bool enable, const char *filename) {
        d_metadata_enabled = enable;
        if (enable) {
            d_metadata_fp = fopen(filename, "w");
        }
        else {
            if (d_metadata_fp != NULL) {
                fclose(d_metadata_fp);
                d_metadata_fp = NULL;
            }
        }
    }

    void
    file_sink_on_tag_impl::set_burst_extra_data_output(bool enable) {
        d_extra_burst_output = enable;
    }


    int
    file_sink_on_tag_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        char *inbuf = (char *) input_items[0];
        int  nread = 0;
        int status;
        std::vector<tag_t> tags;
        std::vector<int> tag_positions;
        std::vector<std::string> tag_data;
        int next_tag_position = -1;
        int next_tag_position_index = -1;
		std::string next_tag_data = "";
        bool start_now = false;

        get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items, d_search_tag);
        for(int i=0;i<tags.size();i++) {
            int current;
            current = tags[i].offset - nitems_read(0);
            tag_positions.push_back(current);
			if (d_extra_burst_output) {
				uint64_t id = pmt::to_uint64(pmt::dict_ref(tags[i].value, pmt::mp("id"), pmt::PMT_NIL));
				float magnitude = pmt::to_float(pmt::dict_ref(tags[i].value, pmt::mp("magnitude"), pmt::PMT_NIL));
				float center_frequency = pmt::to_float(pmt::dict_ref(tags[i].value, pmt::mp("center_frequency"), pmt::PMT_NIL));
				float sample_rate = pmt::to_float(pmt::dict_ref(tags[i].value, pmt::mp("sample_rate"), pmt::PMT_NIL));
				float relative_frequency = pmt::to_float(pmt::dict_ref(tags[i].value, pmt::mp("relative_frequency"), pmt::PMT_NIL));
				float burst_freq;
				burst_freq = center_frequency + relative_frequency * ((float)sample_rate);
				std::ostringstream make_string;
				make_string << "Mag: " << magnitude << ", RelFreq: " << relative_frequency << ", BurstFreq: " << burst_freq;
				tag_data.push_back(make_string.str());
			}
			else tag_data.push_back("");

            next_tag_position_index = 0;
        }
        if(next_tag_position_index >= 0) {
            next_tag_position = tag_positions[next_tag_position_index];
        }

        do_update();				// update d_fp is reqd

        if (!d_fp)
          return noutput_items;		// drop output on the floor

        while (nread < noutput_items){
          int count = 0;
          if(d_is_writing) {
              count = fwrite (inbuf, d_itemsize, 1, d_fp);
              if (count == 0)	// FIXME add error handling
                break;
              nread += count;
              inbuf += count * d_itemsize;
              //inbuf += count;
              d_numbers_written += count;
              if(d_numbers_written >= d_number_to_write){
                  int n_zeros = (d_number_to_write / 4); // Fill a fourth of the length with zeros between groups
                  for(int i=0;i<n_zeros;i++) {
                      status = fwrite(d_null_to_write,d_itemsize,1,d_fp);
                      if(status == 0) break;
                  }
                  d_is_writing = 0;
                  d_numbers_written = 0;
                  //d_queue->flush();
                  if (d_unbuffered)
                    fflush (d_fp);
                  memset(d_buffered,0,d_number_buffered*d_itemsize);
              }
          }
          else {
              memcpy (d_buffered+(d_index_buffered*d_itemsize), inbuf, d_itemsize);
              d_index_buffered++;
              if(d_index_buffered >= d_number_buffered-1){
                  d_index_buffered = 0;
              }
              nread += 1;
              inbuf += d_itemsize;
              //msg_in = d_queue->delete_head_nowait();	
              while(nread > next_tag_position && next_tag_position != -1) {
                next_tag_position_index++;
                if (next_tag_position_index == tag_positions.size()) {
                  next_tag_position_index = -1;
                  next_tag_position = -1;
                  break;
                }
                else {
                  next_tag_position = tag_positions[next_tag_position_index];
                }
              }
              if(next_tag_position == nread) {
                if(d_metadata_fp != NULL) {
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    fprintf(d_metadata_fp, "Capture: %llu, Time: %04d-%02d-%02d %02d:%02d:%02d: Data: %s\n", d_capture_cnt, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, tag_data[next_tag_position_index].c_str());
                    fflush(d_metadata_fp);
                    d_capture_cnt++;
                }
                start_now = true;
                next_tag_position_index++;
                if (next_tag_position_index == tag_positions.size()) {
                  next_tag_position_index = -1;
                  next_tag_position = -1;
                }
                else {
                  next_tag_position = tag_positions[next_tag_position_index];
                }
              }
              else start_now = false;
              if(start_now) {
                  printf("w");
                  fflush(stdout);
                  for(int i=d_index_buffered;i<d_number_buffered;i++){
                      status = fwrite(d_buffered+i*d_itemsize,d_itemsize,1,d_fp);
                      if(status == 0) break;
                  }
                  for(int i=0;i<d_index_buffered;i++){
                      status = fwrite(d_buffered+i*d_itemsize,d_itemsize,1,d_fp);
                      if(status == 0) break;
                  }
                  d_is_writing = 1;
              }
          }
        }



        if (d_unbuffered)
            fflush (d_fp);

        //return nwritten;
        return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */

