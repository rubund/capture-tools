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
import time
import pmt

class qa_bit_sniffer (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        msg_meta = pmt.make_dict()
        msg_meta = pmt.dict_add(msg_meta, pmt.to_pmt("freq"), pmt.to_pmt("val"))
        vec1 = [0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00];
        msg_vector = pmt.init_u8vector(len(vec1), vec1)
        msg = pmt.cons(msg_meta, msg_vector)
        src = blocks.message_strobe(msg, 1000)
        dut = capture_tools.bit_sniffer()
        self.tb.msg_connect((src, "strobe"), (dut, "packets"))
        self.tb.start ()
        time.sleep(5)
        vec1 = [0x01, 0x00, 0x01, 0x00, 0x01, 0x00];
        msg_vector = pmt.init_u8vector(len(vec1), vec1)
        msg = pmt.cons(msg_meta, msg_vector)
        src.set_msg(msg);
        time.sleep(5)
        vec1 = [0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01];
        msg_vector = pmt.init_u8vector(len(vec1), vec1)
        msg = pmt.cons(msg_meta, msg_vector)
        src.set_msg(msg);
        time.sleep(5)
        self.tb.stop()
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_bit_sniffer, "qa_bit_sniffer.xml")
