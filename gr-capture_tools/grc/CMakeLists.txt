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
    capture_tools_file_sink_text.block.yml
    capture_tools_one_then_zero.block.yml
    capture_tools_file_sink_on_message.block.yml
    capture_tools_level_alert.block.yml
    capture_tools_add_before_and_after_cc.block.yml
    capture_tools_repeat_input_n_times_cc.block.yml
    capture_tools_keep_middle_in_vector.block.yml
    capture_tools_add_rampup_cc.block.yml
    capture_tools_replace_value_ff.block.yml
    capture_tools_cut_samples_cc.block.yml
    capture_tools_annotate_every_n_samples_from_trigger_ff.block.yml
    capture_tools_annotated_to_msg_f.block.yml
    capture_tools_collect_to_vector_from_tag_f.block.yml
    capture_tools_clock_recovery_mm_sync_ff.block.yml
    capture_tools_add_at_tag_cc.block.yml
    capture_tools_file_source_tag_end.block.yml
    capture_tools_vector_source_x.block.yml
    capture_tools_head_tag_end.block.yml
    capture_tools_pfb_clock_sync_sync_fff.block.yml
    capture_tools_msk_timing_recovery_sync_cc.block.yml
    capture_tools_bit_sniffer.block.yml
    capture_tools_freqest.block.yml
    capture_tools_repeat_input_n_times_cscs.block.yml
    capture_tools_threshold_tag_other_stream_cc.block.yml
    capture_tools_file_sink_on_tag.block.yml
    capture_tools_msg_to_gfsk_c.block.yml
    capture_tools_file_source_on_tag.block.yml
    capture_tools_tag_on_set_one_f.block.yml
    capture_tools_tag_on_set_one_b.block.yml
    capture_tools_tag_first_non_zero_cc.block.yml
    capture_tools_burst_msg_source_c.block.yml
    capture_tools_fft_burst_tagger.block.yml
    capture_tools_tagged_burst_to_pdu.block.yml
    capture_tools_sync_and_strobe.block.yml
    capture_tools_xrun_monitor_ff.block.yml
    capture_tools_save_iq_for_failed.block.yml
    capture_tools_print_tags_c.block.yml
    capture_tools_file_sink_store_tags.block.yml
    capture_tools_zwave_field_parser.block.yml DESTINATION share/gnuradio/grc/blocks
)
