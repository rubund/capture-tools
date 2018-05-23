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

#include <ctime>
#include <gnuradio/io_signature.h>
#include "burst_msg_source_c_impl.h"

namespace gr {
  namespace capture_tools {

    burst_msg_source_c::sptr
    burst_msg_source_c::make(int n)
    {
      return gnuradio::get_initial_sptr
        (new burst_msg_source_c_impl(n));
    }

    /*
     * The private constructor
     */
    burst_msg_source_c_impl::burst_msg_source_c_impl(int n)
      : gr::sync_block("burst_msg_source_c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
		char name[100];
        if (n == 1) {
            message_port_register_in(pmt::mp("burst"));
            set_msg_handler(pmt::mp("burst"),boost::bind(&burst_msg_source_c_impl::new_burst, this, _1));
            message_port_register_out(pmt::mp("handled"));
        }
        else {
            for(int i=0;i<n;i++) {
                snprintf(name, 99, "burst%d", i);
                message_port_register_in(pmt::mp(name));
                set_msg_handler(pmt::mp(name),boost::bind(&burst_msg_source_c_impl::new_burst, this, _1));
                snprintf(name, 99, "handled%d", i);
		        message_port_register_out(pmt::mp(name));
            }
        }
        d_in_burst = false;
        d_current_burst_pos = 0;
        d_current_inc = 0;
        d_current_phase = 0;
        d_repeat = false;
        d_round_factor = 1;
        d_n_zeros = 0;
        d_add_zeros_now = false;
        d_mag_threshold = -999999;
        d_get_one = false;
        d_save_now = false;
        d_saving_now = false;
        d_max_freq = 1e12;
        d_min_freq = -1;
	}

    /*
     * Our virtual destructor.
     */
    burst_msg_source_c_impl::~burst_msg_source_c_impl()
    {
    }


	void
    burst_msg_source_c_impl::new_burst(pmt::pmt_t msg)
    {
		gr::thread::scoped_lock lock(common_mutex);
		d_bursts.push_back(msg);
    }

	void
    burst_msg_source_c_impl::set_repeat(bool val)
    {
        d_repeat = val;
    }

	void
    burst_msg_source_c_impl::set_round_factor(float val)
    {
        d_round_factor = val;
    }

	void
    burst_msg_source_c_impl::set_n_zeros(int val)
    {
        d_n_zeros = val;
    }

    void
    burst_msg_source_c_impl::set_stop(int val)
    {
        if(val == 1)
            d_running = false;
    }

    void
    burst_msg_source_c_impl::set_start(int val)
    {
        if(val == 1)
            d_running = true;
    }

    void
    burst_msg_source_c_impl::set_get_next(int val)
    {
        if(val == 1 && !d_running)
            d_get_one = true;
    }

    void
    burst_msg_source_c_impl::set_save_current(int val)
    {
        if(val == 1)
            d_save_now = true;
    }

    void
    burst_msg_source_c_impl::set_max_freq(float val)
    {
        d_max_freq = val;
    }

    void
    burst_msg_source_c_impl::set_min_freq(float val)
    {
        d_min_freq = val;
    }

    void
    burst_msg_source_c_impl::set_mag_threshold(float val)
    {
        d_mag_threshold = val;
    }

    int
    burst_msg_source_c_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *out = (gr_complex *) output_items[0];
		int produced = 0;
		pmt::pmt_t current_burst;

		gr::thread::scoped_lock lock(common_mutex);
        if(d_add_zeros_now) {
            int remaining = d_n_zeros - d_zeros_cnt;
            int tomake = std::min(remaining, noutput_items);

            d_zeros_cnt += tomake;
            produced += tomake;
            memset(out, 0, tomake*sizeof(gr_complex));
            if(tomake == remaining) {
                d_add_zeros_now = false;
            }
        }
		else if(d_bursts.size() == 0) {
            if(d_repeat) {
                memset(out, 0, noutput_items*sizeof(gr_complex));
                produced = noutput_items; // If repeat, we output continuously zeros before the first message. If not repeat, we just wait a little bit
            }
            else {
                usleep(100000);
            }
		}
		else {
            int lnow = d_bursts.size();
            for(int i=0;i<lnow;i++) {
		        current_burst = d_bursts.front();
                pmt::pmt_t samples = pmt::cdr(current_burst);
                size_t current_burst_length = pmt::length(samples);
                int remaining = noutput_items - produced;
                const gr_complex *burst = (const gr_complex*) pmt::c32vector_elements(samples, current_burst_length);
                int remaining_in_current;
                if (d_in_burst) {
                    remaining_in_current = current_burst_length - d_current_burst_pos;
                }
                else {
                    remaining_in_current = current_burst_length;

                    pmt::pmt_t key = pmt::string_to_symbol("new_burst");

                    pmt::pmt_t meta = pmt::car(current_burst);
                    float relative_frequency = pmt::to_float(pmt::dict_ref(meta, pmt::mp("relative_frequency"), pmt::PMT_NIL));
                    float center_frequency = pmt::to_float(pmt::dict_ref(meta, pmt::mp("center_frequency"), pmt::PMT_NIL));
                    float sample_rate = pmt::to_float(pmt::dict_ref(meta, pmt::mp("sample_rate"), pmt::PMT_NIL));
                    uint64_t id = pmt::to_uint64(pmt::dict_ref(meta, pmt::mp("id"), pmt::PMT_NIL));
                    d_current_id = id;
                    uint64_t offset = pmt::to_uint64(pmt::dict_ref(meta, pmt::mp("offset"), pmt::PMT_NIL));
                    float magnitude = pmt::to_float(pmt::dict_ref(meta, pmt::mp("magnitude"), pmt::PMT_NIL));
                    float burst_shift_freq;
                    burst_shift_freq = relative_frequency * ((float)sample_rate);


                    // Round to every 50 kHz:
                    float factor = d_round_factor;
                    float divided = burst_shift_freq / factor;
                    float rounded = round(divided);
                    burst_shift_freq = rounded * factor;
                    //

                    float burst_freq;
                    burst_freq = center_frequency + burst_shift_freq;

                    // Filtering out bursts
                    if (burst_freq < d_min_freq || burst_freq > d_max_freq) remaining_in_current = 0;
                    if (magnitude < d_mag_threshold) remaining_in_current = 0;
                    //

                    if (d_save_now && !d_saving_now && remaining_in_current != 0) {
                        d_fp = fopen("/tmp/burst.cfile","wb");
                        d_saving_now = true;
                        d_save_now = false;
                    }

                    d_current_inc = 2*M_PI* burst_shift_freq / sample_rate;
                    d_current_phase = 0;

                    pmt::pmt_t value = pmt::make_dict();
                    value = pmt::dict_add(value, pmt::mp("id"), pmt::from_uint64(id));
                    value = pmt::dict_add(value, pmt::mp("burst_frequency_mhz"), pmt::from_float(burst_freq/1e6));
                    value = pmt::dict_add(value, pmt::mp("burst_frequency"), pmt::from_float(burst_freq));
                    value = pmt::dict_add(value, pmt::mp("relative_frequency"), pmt::from_float(relative_frequency));
                    value = pmt::dict_add(value, pmt::mp("center_frequency"), pmt::from_float(center_frequency));
                    value = pmt::dict_add(value, pmt::mp("magnitude"), pmt::from_float(magnitude));
                    value = pmt::dict_add(value, pmt::mp("offset"), pmt::from_uint64(offset));
                    value = pmt::dict_add(value, pmt::mp("sample_rate"), pmt::from_float(sample_rate));

                    if(remaining_in_current > 0) {
                        //fprintf(stdout, "Received: %5llu", id);
                        //std::cout << ", Freq: " << (burst_freq/1e6) << " MHz, Magnitude: " << magnitude;
                        //time_t t = time(NULL);
                        //struct tm tm = *localtime(&t);
                        //fprintf(stdout, ", Time: %04d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                        add_item_tag(0, nitems_written(0) + produced, key, value);
                    }
                }

                
                int toproduce = std::min(remaining, remaining_in_current);

                for(int j = 0; j<toproduce; j++) {
                    out[produced] = burst[d_current_burst_pos+j] * gr_complex(cos(d_current_phase), -sin(d_current_phase));
                    d_current_phase = d_current_phase + d_current_inc;
                    if      (d_current_phase > M_PI)  d_current_phase -= 2*M_PI;
                    else if (d_current_phase < -M_PI) d_current_phase += 2*M_PI;
                    produced++;
                    if(d_saving_now)
                        fwrite(out+produced, sizeof(gr_complex), 1, d_fp);
                }
                if (toproduce < remaining_in_current) {
                    d_in_burst = true;
                    d_current_burst_pos += toproduce;
                    break;
                }
                else {
                    if (toproduce > 0) {
                        pmt::pmt_t key = pmt::string_to_symbol("end_burst");
                        add_item_tag(0, nitems_written(0) + produced, key, pmt::PMT_NIL);

                        if(d_saving_now) {
                            d_saving_now = false;
                            fclose(d_fp);
                            printf("Done saving to file /tmp/burst.cfile\n");
                        }
                    }
                    d_in_burst = false;
                    d_current_burst_pos = 0;
                    bool running_get_one = d_running || d_get_one;
                    d_get_one = false;
                    if((remaining_in_current == 0) || (!d_repeat && running_get_one) || (((lnow - i) > 1) && running_get_one)) {
                        d_bursts.pop_front();
                        pmt::pmt_t handled_msg = pmt::from_uint64(d_current_id);
                        message_port_pub(pmt::mp("handled"), handled_msg);
                    }
                    if ((!running_get_one || (d_repeat && (lnow - i) <= 1)) && (remaining_in_current > 0)) {
                        usleep(100000); // We throttle down if there is nothing more to produce for the current burst AND we have temporarily stopped or we are in repeat mode and there are no more packets in the queue.
                    }
                    if (toproduce > 0) {
                        if (d_n_zeros > 0) {
                            d_add_zeros_now = true;
                            d_zeros_cnt = 0;
                            break;
                        }
                    }
                }

            }
		}

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace capture_tools */
} /* namespace gr */

