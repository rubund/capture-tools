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

include(GrMiscUtils)
GR_EXPAND_X_H(blocks vector_source_X    b s i f c)

add_custom_target(blocks_generated_includes DEPENDS
    ${generated_includes}
)


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
    concatenate_cc.h
    head_tag_end.h DESTINATION include/capture_tools
)
