#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2017 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import capture_tools_swig as capture_tools
import numpy
import pmt

class qa_repeat_input_n_times_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        tags = []
        tags.append(gr.tag_utils.python_to_tag((2, pmt.to_pmt("the_key"), pmt.to_pmt("the_value"), pmt.PMT_NIL)))
        tags.append(gr.tag_utils.python_to_tag((3, pmt.to_pmt("hehe"), pmt.to_pmt("the_value"), pmt.PMT_NIL)))
        #tags.append(gr.tag_utils.python_to_tag((4, pmt.to_pmt("end"), pmt.to_pmt("vector_source_c"), pmt.PMT_NIL)))
        self.src = blocks.vector_source_c([1,2,3,4,5], tags=tags)
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(3, 1000)
        #self.repeating.set_tag_propagate_end(False)
        #self.repeating.set_tag_add_repeat(False)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        tags = self.dst.tags()
        #for t in tags:
        #    print("HER: "+str(t.offset))
        self.assertEqual(self.dst.data() , ((1+0j), (2+0j), (3+0j), (4+0j), (5+0j), (1+0j), (2+0j), (3+0j), (4+0j), (5+0j), (1+0j), (2+0j), (3+0j), (4+0j), (5+0j)))
        self.assertEqual(len(tags), 8)
        self.assertEqual(str(tags[0].key), "the_key")
        self.assertEqual(str(tags[0].value), "the_value")
        self.assertEqual(int(tags[0].offset), 2)
        # check data

    def test_002_t (self):
        # set up fg
        tags = []
        tags.append(gr.tag_utils.python_to_tag((4, pmt.to_pmt("end"), pmt.to_pmt("vector_source_c"), pmt.PMT_NIL)))
        self.src = blocks.vector_source_c([1,2,3,4,5], tags=tags)
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(2, 1000)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        #print(str(self.dst.data()))
        self.assertEqual(self.dst.data() , ((1+0j), (2+0j), (3+0j), (4+0j), (5+0j), (1+0j), (2+0j), (3+0j), (4+0j), (5+0j)))
        # check data

    def test_003_t (self):
        # set up fg
        self.src = blocks.vector_source_c([1,2,3,4,5])
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(2, 4)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        #print(str(self.dst.data()))
        self.assertEqual(self.dst.data() , ((1+0j), (2+0j), (3+0j), (4+0j), (1+0j), (2+0j), (3+0j), (4+0j)))
        # check data

    def test_004_t (self):
        # set up fg
        self.src = blocks.vector_source_c([1,2,3,4,5])
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(2, 3)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        #print(str(self.dst.data()))
        self.assertEqual(self.dst.data() , ((1+0j), (2+0j), (3+0j), (1+0j), (2+0j), (3+0j)))
        # check data

    def test_005_t (self):
        # set up fg
        self.src = blocks.vector_source_c([9,8]+list(numpy.ones(100000))+[2,3,4])
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(2, 3)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        #print(str(self.dst.data()))
        self.assertEqual(self.dst.data() , ((9+0j), (8+0j), (1+0j), (9+0j), (8+0j), (1+0j)))
        # check data

    def test_006_t (self):
        # set up fg
        self.src = blocks.vector_source_c([1,2,3,4])
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(2, 3)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        #print(str(self.dst.data()))
        self.assertEqual(self.dst.data() , ((1+0j), (2+0j), (3+0j), (1+0j), (2+0j), (3+0j)))
        # check data

    def test_007_t (self):
        # set up fg
        self.src = blocks.vector_source_c([1,2,3,4])
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(2, 4)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        #print(str(self.dst.data()))
        self.assertEqual(self.dst.data() , ((1+0j), (2+0j), (3+0j), (4+0j), (1+0j), (2+0j), (3+0j), (4+0j)))
        # check data

    def test_008_t (self):
        # set up fg
        tags = []
        tags.append(gr.tag_utils.python_to_tag((3, pmt.to_pmt("end"), pmt.to_pmt("vector_source_c"), pmt.PMT_NIL)))
        self.src = blocks.vector_source_c([1,2,3,4], tags=tags)
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(2, 5)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        #print(str(self.dst.data()))
        self.assertEqual(self.dst.data() , ((1+0j), (2+0j), (3+0j), (4+0j), (1+0j), (2+0j), (3+0j), (4+0j)))
        # check data

    def test_009_t (self):
        # set up fg
        tags = []
        tags.append(gr.tag_utils.python_to_tag((1, pmt.to_pmt("end"), pmt.to_pmt("vector_source_c"), pmt.PMT_NIL)))
        self.src = blocks.vector_source_c([1,2], tags=tags)
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(3, 5)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        #print(str(self.dst.data()))
        self.assertEqual(self.dst.data() , ((1+0j), (2+0j), (1+0j), (2+0j), (1+0j), (2+0j)))
        # check data

    def test_010_t (self):
        # set up fg
        tags = []
        tags.append(gr.tag_utils.python_to_tag((0, pmt.to_pmt("end"), pmt.to_pmt("vector_source_c"), pmt.PMT_NIL)))
        self.src = blocks.vector_source_c([1], tags=tags)
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(3, 5)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        #print(str(self.dst.data()))
        self.assertEqual(self.dst.data() , ((1+0j), (1+0j), (1+0j)))
        # check data

    def test_011_tag1 (self):
        # set up fg
        tags = []
        tags.append(gr.tag_utils.python_to_tag((2, pmt.to_pmt("the_key"), pmt.to_pmt("the_value"), pmt.PMT_NIL)))
        tags.append(gr.tag_utils.python_to_tag((3, pmt.to_pmt("hehe"), pmt.to_pmt("the_value"), pmt.PMT_NIL)))
        tags.append(gr.tag_utils.python_to_tag((4, pmt.to_pmt("end"), pmt.to_pmt("vector_source_c"), pmt.PMT_NIL)))
        self.src = blocks.vector_source_c([1,2,3,4,5], tags=tags)
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(3, 1000)
        self.repeating.set_tag_propagate_end(False)
        self.repeating.set_tag_add_repeat(False)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        tags = self.dst.tags()
        #for t in tags:
        #    print("HER: "+str(t.offset))
        self.assertEqual(self.dst.data() , ((1+0j), (2+0j), (3+0j), (4+0j), (5+0j), (1+0j), (2+0j), (3+0j), (4+0j), (5+0j), (1+0j), (2+0j), (3+0j), (4+0j), (5+0j)))
        self.assertEqual(len(tags), 7)
        self.assertEqual(str(tags[0].key), "the_key")
        self.assertEqual(str(tags[0].value), "the_value")
        self.assertEqual(int(tags[0].offset), 2)
        self.assertEqual(str(tags[2].key), "the_key")
        self.assertEqual(str(tags[2].value), "the_value")
        self.assertEqual(int(tags[2].offset), 7)
        self.assertEqual(str(tags[4].key), "the_key")
        self.assertEqual(str(tags[4].value), "the_value")
        self.assertEqual(int(tags[4].offset), 12)
        self.assertEqual(str(tags[3].key), "hehe")
        self.assertEqual(str(tags[3].value), "the_value")
        self.assertEqual(int(tags[3].offset), 8)
        # check data

    def test_011_tag2 (self):
        # set up fg
        tags = []
        tags.append(gr.tag_utils.python_to_tag((2, pmt.to_pmt("the_key"), pmt.to_pmt("the_value"), pmt.PMT_NIL)))
        tags.append(gr.tag_utils.python_to_tag((3, pmt.to_pmt("hehe"), pmt.to_pmt("the_value"), pmt.PMT_NIL)))
        tags.append(gr.tag_utils.python_to_tag((4, pmt.to_pmt("end"), pmt.to_pmt("vector_source_c"), pmt.PMT_NIL)))
        self.src = blocks.vector_source_c([1,2,3,4,5], tags=tags)
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(3, 1000)
        self.repeating.set_tag_propagate_end(True)
        self.repeating.set_tag_add_repeat(False)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        tags = self.dst.tags()
        #for t in tags:
        #    print("HER: "+str(t.offset))
        self.assertEqual(self.dst.data() , ((1+0j), (2+0j), (3+0j), (4+0j), (5+0j), (1+0j), (2+0j), (3+0j), (4+0j), (5+0j), (1+0j), (2+0j), (3+0j), (4+0j), (5+0j)))
        self.assertEqual(len(tags), 10)
        self.assertEqual(str(tags[0].key), "the_key")
        self.assertEqual(str(tags[0].value), "the_value")
        self.assertEqual(int(tags[0].offset), 2)
        self.assertEqual(str(tags[2].key), "end")
        self.assertEqual(str(tags[2].value), "vector_source_c")
        self.assertEqual(int(tags[2].offset), 4)
        self.assertEqual(str(tags[3].key), "the_key")
        self.assertEqual(str(tags[3].value), "the_value")
        self.assertEqual(int(tags[3].offset), 7)
        self.assertEqual(str(tags[4].key), "hehe")
        self.assertEqual(str(tags[4].value), "the_value")
        self.assertEqual(int(tags[4].offset), 8)
        self.assertEqual(str(tags[9].key), "end")
        self.assertEqual(str(tags[9].value), "repeat_input_n_times_cc")
        self.assertEqual(int(tags[9].offset), 14)
        # check data


    def test_011_tag2 (self):
        # set up fg
        tags = []
        tags.append(gr.tag_utils.python_to_tag((2, pmt.to_pmt("the_key"), pmt.to_pmt("the_value"), pmt.PMT_NIL)))
        tags.append(gr.tag_utils.python_to_tag((3, pmt.to_pmt("hehe"), pmt.to_pmt("the_value"), pmt.PMT_NIL)))
        tags.append(gr.tag_utils.python_to_tag((4, pmt.to_pmt("end"), pmt.to_pmt("vector_source_c"), pmt.PMT_NIL)))
        self.src = blocks.vector_source_c([1,2,3,4,5], tags=tags)
        self.dst = blocks.vector_sink_c()
        self.repeating = capture_tools.repeat_input_n_times_cc(3, 1000)
        self.repeating.set_tag_propagate_end(True)
        self.repeating.set_tag_add_repeat(True)
        self.tb.connect(self.src, self.repeating, self.dst)
        self.tb.run ()
        tags = self.dst.tags()
        #for t in tags:
        #    print("HER: "+str(t.offset))
        self.assertEqual(self.dst.data() , ((1+0j), (2+0j), (3+0j), (4+0j), (5+0j), (1+0j), (2+0j), (3+0j), (4+0j), (5+0j), (1+0j), (2+0j), (3+0j), (4+0j), (5+0j)))
        self.assertEqual(len(tags), 12)
        self.assertEqual(str(tags[0].key), "the_key")
        self.assertEqual(str(tags[0].value), "the_value")
        self.assertEqual(int(tags[0].offset), 2)
        self.assertEqual(str(tags[2].key), "end")
        self.assertEqual(str(tags[2].value), "vector_source_c")
        self.assertEqual(int(tags[2].offset), 4)
        self.assertEqual(str(tags[3].key), "repeat")
        self.assertEqual(str(tags[3].value), "repeat_input_n_times_cc")
        self.assertEqual(int(tags[3].offset), 4)
        self.assertEqual(str(tags[4].key), "the_key")
        self.assertEqual(str(tags[4].value), "the_value")
        self.assertEqual(int(tags[4].offset), 7)
        self.assertEqual(str(tags[5].key), "hehe")
        self.assertEqual(str(tags[5].value), "the_value")
        self.assertEqual(int(tags[5].offset), 8)
        self.assertEqual(str(tags[6].key), "end")
        self.assertEqual(str(tags[6].value), "vector_source_c")
        self.assertEqual(int(tags[6].offset), 9)
        self.assertEqual(str(tags[7].key), "repeat")
        self.assertEqual(str(tags[7].value), "repeat_input_n_times_cc")
        self.assertEqual(int(tags[7].offset), 9)
        self.assertEqual(str(tags[11].key), "end")
        self.assertEqual(str(tags[11].value), "repeat_input_n_times_cc")
        self.assertEqual(int(tags[11].offset), 14)
        # check data

if __name__ == '__main__':
    gr_unittest.run(qa_repeat_input_n_times_cc, "qa_repeat_input_n_times_cc.xml")
