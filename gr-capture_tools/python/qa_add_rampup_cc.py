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
import pmt

class qa_add_rampup_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        tag1 = gr.tag_utils.python_to_tag((0, pmt.to_pmt("ramp_up"), pmt.to_pmt("the_value"), pmt.PMT_NIL))
        tag2 = gr.tag_utils.python_to_tag((25, pmt.to_pmt("ramp_down"), pmt.to_pmt("the_value"), pmt.PMT_NIL))
        self.src = blocks.vector_source_c([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1], tags=[tag1, tag2])
        self.dut = capture_tools.add_rampup_cc(0.4)
        self.dst = blocks.vector_sink_c()
        self.tb.connect(self.src, self.dut, self.dst)
        self.tb.run ()
        print(self.dst.data())
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_add_rampup_cc, "qa_add_rampup_cc.xml")
