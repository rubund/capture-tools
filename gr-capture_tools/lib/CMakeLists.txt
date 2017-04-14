# Copyright 2011,2012,2016 Free Software Foundation, Inc.
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
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include(GrMiscUtils)
GR_EXPAND_X_CC_H_IMPL(blocks vector_source_X    b s i f c)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND capture_tools_sources
    ${generated_sources}
    file_sink_text_impl.cc
    one_then_zero_impl.cc
    file_sink_on_message_impl.cc
    level_alert_impl.cc
    add_before_and_after_cc_impl.cc
    repeat_input_n_times_cc_impl.cc
    keep_middle_in_vector_impl.cc
    add_rampup_cc_impl.cc
    replace_value_ff_impl.cc
    cut_samples_cc_impl.cc
    annotate_every_n_samples_from_trigger_ff_impl.cc
    annotated_to_msg_f_impl.cc
    collect_to_vector_from_tag_f_impl.cc
    clock_recovery_mm_sync_ff_impl.cc
    add_at_tag_cc_impl.cc
    file_source_tag_end_impl.cc
    concatenate_cc_impl.cc
    head_tag_end_impl.cc
    pfb_clock_sync_sync_fff_impl.cc
    msk_timing_recovery_sync_cc_impl.cc
    bit_sniffer_impl.cc
    freqest_impl.cc
)

set(capture_tools_sources "${capture_tools_sources}" PARENT_SCOPE)
if(NOT capture_tools_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT capture_tools_sources)

add_library(gnuradio-capture_tools SHARED ${capture_tools_sources})
add_dependencies(gnuradio-capture_tools blocks_generated_includes)
target_link_libraries(gnuradio-capture_tools ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
target_link_libraries(gnuradio-capture_tools gnuradio-blocks)
set_target_properties(gnuradio-capture_tools PROPERTIES DEFINE_SYMBOL "gnuradio_capture_tools_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-capture_tools PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-capture_tools RUNTIME_COMPONENT "capture_tools_runtime" DEVEL_COMPONENT "capture_tools_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_capture_tools_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_capture_tools.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_capture_tools.cc
)

add_executable(test-capture_tools ${test_capture_tools_sources})

target_link_libraries(
  test-capture_tools
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-capture_tools
)

GR_ADD_TEST(test_capture_tools test-capture_tools)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

