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

########################################################################
# Include python install macros
########################################################################
include(GrPython)
if(NOT PYTHONINTERP_FOUND)
    return()
endif()

########################################################################
# Install python sources
########################################################################
GR_PYTHON_INSTALL(
    FILES
    __init__.py
    DESTINATION ${GR_PYTHON_DIR}/capture_tools
)

########################################################################
# Handle the unit tests
########################################################################
include(GrTest)

set(GR_TEST_TARGET_DEPS gnuradio-capture_tools)
set(GR_TEST_PYTHON_DIRS ${CMAKE_BINARY_DIR}/swig)
GR_ADD_TEST(qa_file_sink_on_message ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_file_sink_on_message.py)
GR_ADD_TEST(qa_level_alert ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_level_alert.py)
GR_ADD_TEST(qa_add_before_and_after_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_add_before_and_after_cc.py)
GR_ADD_TEST(qa_repeat_input_n_times_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_repeat_input_n_times_cc.py)
GR_ADD_TEST(qa_keep_middle_in_vector ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_keep_middle_in_vector.py)
GR_ADD_TEST(qa_add_rampup_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_add_rampup_cc.py)
GR_ADD_TEST(qa_replace_value_ff ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_replace_value_ff.py)
GR_ADD_TEST(qa_cut_samples_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_cut_samples_cc.py)
GR_ADD_TEST(qa_annotate_every_n_samples_from_trigger_ff ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_annotate_every_n_samples_from_trigger_ff.py)
GR_ADD_TEST(qa_annotated_to_msg_f ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_annotated_to_msg_f.py)
GR_ADD_TEST(qa_collect_to_vector_from_tag_f ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_collect_to_vector_from_tag_f.py)
GR_ADD_TEST(qa_clock_recovery_mm_sync_ff ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_clock_recovery_mm_sync_ff.py)
GR_ADD_TEST(qa_add_at_tag_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_add_at_tag_cc.py)
GR_ADD_TEST(qa_file_source_tag_end ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_file_source_tag_end.py)
GR_ADD_TEST(qa_concatenate_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_concatenate_cc.py)
GR_ADD_TEST(qa_vector_source ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_vector_source.py)
GR_ADD_TEST(qa_head_tag_end ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_head_tag_end.py)
GR_ADD_TEST(qa_pfb_clock_sync_sync_fff ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_pfb_clock_sync_sync_fff.py)
GR_ADD_TEST(qa_msk_timing_recovery_sync_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_msk_timing_recovery_sync_cc.py)
GR_ADD_TEST(qa_bit_sniffer ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_bit_sniffer.py)
GR_ADD_TEST(qa_freqest ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_freqest.py)
GR_ADD_TEST(qa_repeat_input_n_times_cscs ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_repeat_input_n_times_cscs.py)
GR_ADD_TEST(qa_threshold_tag_other_stream_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_threshold_tag_other_stream_cc.py)
GR_ADD_TEST(qa_file_sink_on_tag ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_file_sink_on_tag.py)
GR_ADD_TEST(qa_msg_to_gfsk_c ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_msg_to_gfsk_c.py)
GR_ADD_TEST(qa_file_source_on_tag ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_file_source_on_tag.py)
GR_ADD_TEST(qa_tag_on_set_one_f ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_tag_on_set_one_f.py)
GR_ADD_TEST(qa_tag_on_set_one_b ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_tag_on_set_one_b.py)
GR_ADD_TEST(qa_tag_first_non_zero_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_tag_first_non_zero_cc.py)
GR_ADD_TEST(qa_burst_msg_source_c ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_burst_msg_source_c.py)
GR_ADD_TEST(qa_fft_burst_tagger ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_fft_burst_tagger.py)
