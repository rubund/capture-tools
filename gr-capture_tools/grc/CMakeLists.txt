# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

install(FILES
    capture_tools_file_sink_text.xml
    capture_tools_one_then_zero.xml
    capture_tools_file_sink_on_message.xml
    capture_tools_level_alert.xml
    capture_tools_add_before_and_after_cc.xml
    capture_tools_repeat_input_n_times_cc.xml
    capture_tools_keep_middle_in_vector.xml
    capture_tools_add_rampup_cc.xml
    capture_tools_replace_value_ff.xml
    capture_tools_cut_samples_cc.xml
    capture_tools_annotate_every_n_samples_from_trigger_ff.xml
    capture_tools_annotated_to_msg_f.xml
    capture_tools_collect_to_vector_from_tag_f.xml
    capture_tools_clock_recovery_mm_sync_ff.xml
    capture_tools_add_at_tag_cc.xml
    capture_tools_file_source_tag_end.xml
    capture_tools_concatenate_cc.xml
    capture_tools_vector_source_x.xml
    capture_tools_head_tag_end.xml
    capture_tools_pfb_clock_sync_sync_fff.xml
    capture_tools_msk_timing_recovery_sync_cc.xml
    capture_tools_bit_sniffer.xml
    capture_tools_freqest.xml
    capture_tools_repeat_input_n_times_cscs.xml
    capture_tools_threshold_tag_other_stream_cc.xml
    capture_tools_file_sink_on_tag.xml
    capture_tools_msg_to_gfsk_c.xml
    capture_tools_file_source_on_tag.xml
    capture_tools_tag_on_set_one_f.xml
    capture_tools_tag_on_set_one_b.xml
    capture_tools_tag_first_non_zero_cc.xml
    capture_tools_burst_msg_source_c.xml
    capture_tools_fft_burst_tagger.xml
    capture_tools_tagged_burst_to_pdu.xml DESTINATION share/gnuradio/grc/blocks
)
