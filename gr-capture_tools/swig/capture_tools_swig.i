/* -*- c++ -*- */

#define CAPTURE_TOOLS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "capture_tools_swig_doc.i"

%{
#include "capture_tools/file_sink_text.h"
#include "capture_tools/one_then_zero.h"
#include "capture_tools/file_sink_on_message.h"
#include "capture_tools/level_alert.h"
#include "capture_tools/add_before_and_after_cc.h"
#include "capture_tools/repeat_input_n_times_cc.h"
%}


%include "capture_tools/file_sink_text.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, file_sink_text);
%include "capture_tools/one_then_zero.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, one_then_zero);
%include "capture_tools/file_sink_on_message.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, file_sink_on_message);
%include "capture_tools/level_alert.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, level_alert);
%include "capture_tools/add_before_and_after_cc.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, add_before_and_after_cc);
%include "capture_tools/repeat_input_n_times_cc.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, repeat_input_n_times_cc);
