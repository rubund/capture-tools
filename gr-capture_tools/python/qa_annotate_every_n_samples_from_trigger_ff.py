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

class qa_annotate_every_n_samples_from_trigger_ff (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        src = blocks.vector_source_f([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1])
        src_th = blocks.vector_source_f([0,0,0,1,1,1,1,1,1,1,1,1,1,1,1])
        dut = capture_tools.annotate_every_n_samples_from_trigger_ff(3, 2)
        dst = blocks.vector_sink_f()
        self.tb.connect(src,(dut, 0))
        self.tb.connect(src_th,(dut, 1))
        self.tb.connect(dut, dst)
        self.tb.run ()
        print(dst.data())
        print(dst.tags())
        for t in dst.tags():
            print(t.key)
            print(t.offset)
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_annotate_every_n_samples_from_trigger_ff, "qa_annotate_every_n_samples_from_trigger_ff.xml")
