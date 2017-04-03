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
#include "capture_tools/keep_middle_in_vector.h"
#include "capture_tools/add_rampup_cc.h"
#include "capture_tools/replace_value_ff.h"
#include "capture_tools/cut_samples_cc.h"
#include "capture_tools/annotate_every_n_samples_from_trigger_ff.h"
#include "capture_tools/annotated_to_msg_f.h"
#include "capture_tools/collect_to_vector_from_tag_f.h"
#include "capture_tools/clock_recovery_mm_sync_ff.h"
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
%include "capture_tools/keep_middle_in_vector.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, keep_middle_in_vector);
%include "capture_tools/add_rampup_cc.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, add_rampup_cc);
%include "capture_tools/replace_value_ff.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, replace_value_ff);
%include "capture_tools/cut_samples_cc.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, cut_samples_cc);
%include "capture_tools/annotate_every_n_samples_from_trigger_ff.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, annotate_every_n_samples_from_trigger_ff);
%include "capture_tools/annotated_to_msg_f.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, annotated_to_msg_f);
%include "capture_tools/collect_to_vector_from_tag_f.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, collect_to_vector_from_tag_f);
%include "capture_tools/clock_recovery_mm_sync_ff.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, clock_recovery_mm_sync_ff);
