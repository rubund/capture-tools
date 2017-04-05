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

class qa_vector_source (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        tags= []
        tags.append(gr.tag_utils.python_to_tag((2, pmt.to_pmt("add_here"), pmt.to_pmt("the_value"), pmt.PMT_NIL)))
        tags.append(gr.tag_utils.python_to_tag((5, pmt.to_pmt("add_here"), pmt.to_pmt("the_value"), pmt.PMT_NIL)))
        src = capture_tools.vector_source_c([1,2,3,4,5,6,7,8,9], tags=tags)
        dut = capture_tools.add_at_tag_cc(10,10,0,"add_here")
        dst = blocks.vector_sink_c()
        self.tb.connect(src,dut,dst)
        self.tb.run ()
        print(dst.data())
        print("Tags:")
        for t in dst.tags():
            print(str(t.offset)+": "+str(t.key)+"")
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_vector_source, "qa_vector_source.xml")
