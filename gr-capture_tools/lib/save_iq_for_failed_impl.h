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

#ifndef INCLUDED_CAPTURE_TOOLS_SAVE_IQ_FOR_FAILED_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_SAVE_IQ_FOR_FAILED_IMPL_H

#include <capture_tools/save_iq_for_failed.h>

#include <list>

namespace gr {
  namespace capture_tools {

    class save_iq_for_failed_impl : public save_iq_for_failed
    {
     private:
      // Nothing to declare in this block.
        int d_nhistory;
      pmt::pmt_t d_search_tag;
        int d_max_in_queue;
        std::string d_save_path;
        int d_length_to_save;

        std::list<gr_complex *> d_chunks;

        gr_complex *d_current_chunk;
        int d_saved;
        bool d_saving;

        void handler(pmt::pmt_t msg);
        void complete_save();

     public:
      save_iq_for_failed_impl(int length_to_save, int max_in_queue, const char *save_path, int history);
      ~save_iq_for_failed_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

       
      void set_search_tag(const std::string &);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_SAVE_IQ_FOR_FAILED_IMPL_H */

