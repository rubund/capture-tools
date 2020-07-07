# Copyright 2011,2012 Free Software Foundation, Inc.
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


########################################################################
# Install public header files
########################################################################
install(FILES
    ${generated_includes}
    api.h
    file_sink_text.h
    one_then_zero.h
    file_sink_on_message.h
    level_alert.h
    add_before_and_after_cc.h
    repeat_input_n_times_cc.h
    keep_middle_in_vector.h
    add_rampup_cc.h
    replace_value_ff.h
    cut_samples_cc.h
    annotate_every_n_samples_from_trigger_ff.h
    annotated_to_msg_f.h
    collect_to_vector_from_tag_f.h
    clock_recovery_mm_sync_ff.h
    add_at_tag_cc.h
    file_source_tag_end.h
    head_tag_end.h
    pfb_clock_sync_sync_fff.h
    msk_timing_recovery_sync_cc.h
    bit_sniffer.h
    freqest.h
    repeat_input_n_times_cscs.h
    threshold_tag_other_stream_cc.h
    file_sink_on_tag.h
    msg_to_gfsk_c.h
    file_source_on_tag.h
    tag_on_set_one_f.h
    tag_on_set_one_b.h
    tag_first_non_zero_cc.h
    burst_msg_source_c.h
    fft_burst_tagger.h
    tagged_burst_to_pdu.h
    sync_and_strobe.h
    xrun_monitor_ff.h
    xrun_monitor_cc.h
    save_iq_for_failed.h DESTINATION include/capture_tools
)
