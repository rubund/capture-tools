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
#include "save_iq_for_failed_impl.h"
#include <list>

namespace gr {
  namespace capture_tools {

    save_iq_for_failed::sptr
    save_iq_for_failed::make(int length_to_save, int max_in_queue, const char *save_path, int history)
    {
      return gnuradio::get_initial_sptr
        (new save_iq_for_failed_impl(length_to_save, max_in_queue, save_path, history));
    }

    /*
     * The private constructor
     */
    save_iq_for_failed_impl::save_iq_for_failed_impl(int length_to_save, int max_in_queue, const char *save_path, int history)
      : gr::sync_block("save_iq_for_failed",
              gr::io_signature::make2(1, 2, sizeof(gr_complex), sizeof(float)),
              gr::io_signature::make(0, 0, 0)),
            d_nhistory(history), d_max_in_queue(max_in_queue), d_length_to_save(length_to_save)
    {
        set_history(history);
        mkdir(save_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        d_save_path = std::string(save_path);
        message_port_register_in(pmt::mp("passfail"));
        set_msg_handler(pmt::mp("passfail"),boost::bind(&save_iq_for_failed_impl::handler, this, _1));

        assert(length_to_save > history);

        d_current_id = -1;
        d_current_chunk = NULL;
        d_saving = false;
        d_saved = 0;
    }

    /*
     * Our virtual destructor.
     */
    save_iq_for_failed_impl::~save_iq_for_failed_impl()
    {
    }

    void
    save_iq_for_failed_impl::set_search_tag(const std::string &tag_str)
    {
      d_search_tag = pmt::intern(tag_str);
    }

    void
    save_iq_for_failed_impl::handler(pmt::pmt_t msg)
    {
        uint64_t packet_id;
        bool passfail;
        if (pmt::dict_has_key(msg, pmt::mp("packet_id")))
            packet_id = pmt::to_uint64(pmt::dict_ref(msg, pmt::mp("packet_id"), pmt::PMT_NIL));
        if (pmt::dict_has_key(msg, pmt::mp("passfail")))
            passfail = pmt::to_bool(pmt::dict_ref(msg, pmt::mp("passfail"), pmt::PMT_NIL));

        std::tuple<uint64_t, bool> *new_tuple = new std::tuple<uint64_t, bool>(packet_id, passfail);
        printf("packet id: %d, passfail: %d\n", packet_id, passfail);

        gr::thread::scoped_lock lock(fp_merge_lock);

        d_passfail.push_back(new_tuple);
        merge_them();
    }

    void
    save_iq_for_failed_impl::complete_save() {
        d_saving = false;
        d_saved = 0;

        gr::thread::scoped_lock lock(fp_merge_lock);

        d_chunks.push_back(d_current_chunk);
        d_indices.push_back(d_current_id);
        //delete d_current_chunk;
        std::list<std::tuple<uint64_t,bool> *>::iterator it;
        for(it=d_passfail.begin(); it != d_passfail.end(); ++it) {
        }
        merge_them();
    }

    void
    save_iq_for_failed_impl::merge_them() {
        std::list<std::tuple<uint64_t,bool> *>::iterator it_passfail;
        std::list<uint64_t>::iterator it_indices;
        std::list<gr_complex *>::iterator it_chunks;

        int location = 0;
        bool didsomething = true;
        while(didsomething) {
            didsomething = false;
            for(it_indices=d_indices.begin(); it_indices != d_indices.end(); ++it_indices) {
                bool found = false;
                bool iscrcok;
                for (it_passfail=d_passfail.begin(); it_passfail != d_passfail.end(); ++it_passfail) {
                    int passfail_index = std::get<0>(**it_passfail);
                    if ((*it_indices) == passfail_index) {
                        iscrcok = std::get<1>(**it_passfail);
                        found = true;
                        delete *it_passfail;
                        d_passfail.remove(*it_passfail);
                        break;
                    }
                }

                if(found) {
                    didsomething = true;
                    int chunk_location = 0;
                    for(it_chunks=d_chunks.begin(); it_chunks != d_chunks.end() ; ++it_chunks) {
                        if(chunk_location == location) {
                            if (!iscrcok)
                                save_chunk_to_file(*it_chunks, *it_indices);
                            delete *it_chunks;
                            d_chunks.remove(*it_chunks);
                            break;
                        }
                        chunk_location++;
                    }
                    d_indices.remove(*it_indices);
                    break;
                }
                location++;
            }
        }

        //std::cout << "Size list d_passfail: " << d_passfail.size() << std::endl;
        //std::cout << "Size list d_chunks:   " << d_chunks.size() << std::endl;
        //std::cout << "Size list d_indices:  " << d_indices.size() << std::endl;
    }

    void
    save_iq_for_failed_impl::save_chunk_to_file(gr_complex * chunk, uint64_t id) {
        std::ostringstream full_path_ss;
        full_path_ss << d_save_path << "/" <<  id << ".cfile";
        std::string full_path = full_path_ss.str();
        FILE *fp = fopen(full_path.c_str(), "w");
        printf("Saving to file: %d\n"), id;
        int written;
        int left_to_write = d_length_to_save;
        while (left_to_write > 0) {
            written = fwrite(chunk,sizeof(gr_complex),left_to_write,fp);
            left_to_write -= written;
        }
        fclose(fp);
    }

    int
    save_iq_for_failed_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      const float *in_f = NULL;

      if (input_items.size() > 1)
          in_f = (const float *) input_items[1];

      int curpos = 0;
      int consumed = 0;

      std::vector<int> tag_positions;
      int next_tag_position_index = -1;

      std::vector<tag_t> tags;
      if (in_f != NULL)
          get_tags_in_range(tags, 1, nitems_read(1), nitems_read(1) + noutput_items, d_search_tag);
      else
          get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items, d_search_tag);
      for(int i=0;i<tags.size();i++) {
          int current;
          current = tags[i].offset - nitems_read(0);
          tag_positions.push_back(current);
          next_tag_position_index = 0;
      }


      if(!d_saving) {
          if(next_tag_position_index >= 0) {
            curpos = tag_positions[next_tag_position_index];
            uint64_t id_packet = pmt::to_uint64(tags[next_tag_position_index].value);
            d_current_id = id_packet;
            consumed += curpos;
            next_tag_position_index++;
            if (next_tag_position_index >= tag_positions.size())
                next_tag_position_index = -1;
            d_saving = true;
            d_current_chunk = new gr_complex[d_length_to_save];
            int tocopy = std::min(d_length_to_save, (noutput_items + d_nhistory - 1 - curpos));
            memcpy(d_current_chunk, in+curpos, tocopy*sizeof(gr_complex));
            consumed += tocopy - d_nhistory + 1;
            d_saved = tocopy;
            if (d_saved >= d_length_to_save) {
                complete_save();
            }
          }
          else {
            consumed = noutput_items;
          }
      }
      else {
            int tocopy = std::min(d_length_to_save - d_saved, noutput_items);
            memcpy(d_current_chunk + d_saved, in + d_nhistory - 1, tocopy*sizeof(gr_complex));
            consumed += tocopy;
            d_saved += tocopy;
            if (d_saved >= d_length_to_save) {
                complete_save();
            }
      }


      // Tell runtime system how many output items we produced.
      return consumed;
    }

  } /* namespace capture_tools */
} /* namespace gr */

