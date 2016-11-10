/* -*- c++ -*- */

#define CAPTURE_TOOLS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "capture_tools_swig_doc.i"

%{
#include "capture_tools/file_sink_text.h"
#include "capture_tools/one_then_zero.h"
%}


%include "capture_tools/file_sink_text.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, file_sink_text);
%include "capture_tools/one_then_zero.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, one_then_zero);
