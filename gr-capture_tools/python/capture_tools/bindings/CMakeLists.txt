# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT capture_tools_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT capture_tools_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND capture_tools_python_files
    msg_to_gfsk_c_python.cc
    threshold_tag_other_stream_cc_python.cc
    sync_and_strobe_python.cc
    bit_sniffer_python.cc
    annotated_to_msg_f_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(capture_tools
   ../../..
   gr::capture_tools
   "${capture_tools_python_files}")

# copy in bindings .so file for use in QA test module
add_custom_target(
  copy_bindings_for_tests ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/*.so"
    ${CMAKE_BINARY_DIR}/test_modules/gnuradio/capture_tools/
  DEPENDS capture_tools_python)

install(TARGETS capture_tools_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/capture_tools COMPONENT pythonapi)
