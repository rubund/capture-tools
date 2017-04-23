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

class qa_concatenate_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        src1 = blocks.vector_source_c([1,2,3,4])
        src2 = blocks.vector_source_c([5,3,3,3])
        dut = capture_tools.concatenate_cc(2)
        dst = blocks.vector_sink_c()
        self.tb.connect(src1,(dut,0))
        self.tb.connect(src2,(dut,1))
        self.tb.connect(dut, dst)
        self.tb.run ()
        print(dst.data())
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_concatenate_cc, "qa_concatenate_cc.xml")
