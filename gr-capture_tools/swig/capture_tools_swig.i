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
#include "capture_tools/add_at_tag_cc.h"
#include "capture_tools/file_source_tag_end.h"
#include "capture_tools/vector_source.h"
#include "capture_tools/head_tag_end.h"
#include "capture_tools/pfb_clock_sync_sync_fff.h"
#include "capture_tools/msk_timing_recovery_sync_cc.h"
#include "capture_tools/bit_sniffer.h"
#include "capture_tools/freqest.h"
#include "capture_tools/repeat_input_n_times_cscs.h"
#include "capture_tools/threshold_tag_other_stream_cc.h"
#include "capture_tools/file_sink_on_tag.h"
#include "capture_tools/msg_to_gfsk_c.h"
#include "capture_tools/file_source_on_tag.h"
#include "capture_tools/tag_on_set_one_f.h"
#include "capture_tools/tag_on_set_one_b.h"
#include "capture_tools/tag_first_non_zero_cc.h"
#include "capture_tools/burst_msg_source_c.h"
#include "capture_tools/fft_burst_tagger.h"
#include "capture_tools/tagged_burst_to_pdu.h"
#include "capture_tools/sync_and_strobe.h"
#include "capture_tools/xrun_monitor_ff.h"
#include "capture_tools/xrun_monitor_cc.h"
#include "capture_tools/save_iq_for_failed.h"
#include "capture_tools/print_tags_c.h"
#include "capture_tools/file_sink_store_tags.h"
#include "capture_tools/zwave_field_parser.h"

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
%include "capture_tools/add_at_tag_cc.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, add_at_tag_cc);
%include "capture_tools/file_source_tag_end.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, file_source_tag_end);


%include "capture_tools/vector_source.h"

GR_SWIG_BLOCK_MAGIC2_TMPL(capture_tools, vector_source_b, vector_source<std::uint8_t>);
GR_SWIG_BLOCK_MAGIC2_TMPL(capture_tools, vector_source_s, vector_source<std::int16_t>);
GR_SWIG_BLOCK_MAGIC2_TMPL(capture_tools, vector_source_i, vector_source<std::int32_t>);
GR_SWIG_BLOCK_MAGIC2_TMPL(capture_tools, vector_source_f, vector_source<float>);
GR_SWIG_BLOCK_MAGIC2_TMPL(capture_tools, vector_source_c, vector_source<gr_complex>);


%include "capture_tools/head_tag_end.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, head_tag_end);
%include "capture_tools/pfb_clock_sync_sync_fff.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, pfb_clock_sync_sync_fff);
%include "capture_tools/msk_timing_recovery_sync_cc.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, msk_timing_recovery_sync_cc);
%include "capture_tools/bit_sniffer.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, bit_sniffer);
%include "capture_tools/freqest.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, freqest);
%include "capture_tools/repeat_input_n_times_cscs.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, repeat_input_n_times_cscs);
%include "capture_tools/threshold_tag_other_stream_cc.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, threshold_tag_other_stream_cc);
%include "capture_tools/file_sink_on_tag.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, file_sink_on_tag);
%include "capture_tools/msg_to_gfsk_c.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, msg_to_gfsk_c);
%include "capture_tools/file_source_on_tag.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, file_source_on_tag);
%include "capture_tools/tag_on_set_one_f.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, tag_on_set_one_f);
%include "capture_tools/tag_on_set_one_b.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, tag_on_set_one_b);
%include "capture_tools/tag_first_non_zero_cc.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, tag_first_non_zero_cc);
%include "capture_tools/burst_msg_source_c.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, burst_msg_source_c);
%include "capture_tools/fft_burst_tagger.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, fft_burst_tagger);
%include "capture_tools/tagged_burst_to_pdu.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, tagged_burst_to_pdu);
%include "capture_tools/sync_and_strobe.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, sync_and_strobe);
%include "capture_tools/xrun_monitor_ff.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, xrun_monitor_ff);
%include "capture_tools/xrun_monitor_cc.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, xrun_monitor_cc);
%include "capture_tools/save_iq_for_failed.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, save_iq_for_failed);
%include "capture_tools/print_tags_c.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, print_tags_c);
%include "capture_tools/file_sink_store_tags.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, file_sink_store_tags);
%include "capture_tools/zwave_field_parser.h"
GR_SWIG_BLOCK_MAGIC2(capture_tools, zwave_field_parser);
