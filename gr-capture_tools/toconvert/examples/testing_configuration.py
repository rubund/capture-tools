#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Fri Apr 14 09:05:37 2017
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import capture_tools
import pmt
import sys


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.scroll = scroll = False
        self.samp_rate = samp_rate = 32000
        self.offset = offset = 0
        self.hex_output = hex_output = False
        self.bits_per_word = bits_per_word = 8
        self.binary_output = binary_output = True
        self.ascii_output = ascii_output = False

        ##################################################
        # Blocks
        ##################################################
        _scroll_check_box = Qt.QCheckBox("scroll")
        self._scroll_choices = {True: True, False: False}
        self._scroll_choices_inv = dict((v,k) for k,v in self._scroll_choices.iteritems())
        self._scroll_callback = lambda i: Qt.QMetaObject.invokeMethod(_scroll_check_box, "setChecked", Qt.Q_ARG("bool", self._scroll_choices_inv[i]))
        self._scroll_callback(self.scroll)
        _scroll_check_box.stateChanged.connect(lambda i: self.set_scroll(self._scroll_choices[bool(i)]))
        self.top_layout.addWidget(_scroll_check_box)
        self._offset_range = Range(0, 7, 1, 0, 200)
        self._offset_win = RangeWidget(self._offset_range, self.set_offset, "offset", "counter_slider", int)
        self.top_layout.addWidget(self._offset_win)
        _hex_output_check_box = Qt.QCheckBox("hex_output")
        self._hex_output_choices = {True: True, False: False}
        self._hex_output_choices_inv = dict((v,k) for k,v in self._hex_output_choices.iteritems())
        self._hex_output_callback = lambda i: Qt.QMetaObject.invokeMethod(_hex_output_check_box, "setChecked", Qt.Q_ARG("bool", self._hex_output_choices_inv[i]))
        self._hex_output_callback(self.hex_output)
        _hex_output_check_box.stateChanged.connect(lambda i: self.set_hex_output(self._hex_output_choices[bool(i)]))
        self.top_layout.addWidget(_hex_output_check_box)
        self._bits_per_word_range = Range(2, 8, 1, 8, 200)
        self._bits_per_word_win = RangeWidget(self._bits_per_word_range, self.set_bits_per_word, "bits_per_word", "counter_slider", int)
        self.top_layout.addWidget(self._bits_per_word_win)
        _binary_output_check_box = Qt.QCheckBox("binary_output")
        self._binary_output_choices = {True: True, False: False}
        self._binary_output_choices_inv = dict((v,k) for k,v in self._binary_output_choices.iteritems())
        self._binary_output_callback = lambda i: Qt.QMetaObject.invokeMethod(_binary_output_check_box, "setChecked", Qt.Q_ARG("bool", self._binary_output_choices_inv[i]))
        self._binary_output_callback(self.binary_output)
        _binary_output_check_box.stateChanged.connect(lambda i: self.set_binary_output(self._binary_output_choices[bool(i)]))
        self.top_layout.addWidget(_binary_output_check_box)
        _ascii_output_check_box = Qt.QCheckBox("ascii_output")
        self._ascii_output_choices = {True: True, False: False}
        self._ascii_output_choices_inv = dict((v,k) for k,v in self._ascii_output_choices.iteritems())
        self._ascii_output_callback = lambda i: Qt.QMetaObject.invokeMethod(_ascii_output_check_box, "setChecked", Qt.Q_ARG("bool", self._ascii_output_choices_inv[i]))
        self._ascii_output_callback(self.ascii_output)
        _ascii_output_check_box.stateChanged.connect(lambda i: self.set_ascii_output(self._ascii_output_choices[bool(i)]))
        self.top_layout.addWidget(_ascii_output_check_box)
        self.capture_tools_bit_sniffer_0 = capture_tools.bit_sniffer(200, hex_output, offset, bits_per_word, False, False, ascii_output, binary_output, 0, scroll)
        msg_meta = pmt.make_dict()
        msg_meta = pmt.dict_add(msg_meta, pmt.to_pmt("freq"), pmt.to_pmt("val"))
        vec1 = [0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00];
        msg_vector = pmt.init_u8vector(len(vec1), vec1)
        msg = pmt.cons(msg_meta, msg_vector)
        self.blocks_message_strobe_0 = blocks.message_strobe(msg, 10)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.blocks_message_strobe_0, 'strobe'), (self.capture_tools_bit_sniffer_0, 'packets'))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_scroll(self):
        return self.scroll

    def set_scroll(self, scroll):
        self.scroll = scroll
        self._scroll_callback(self.scroll)
        self.capture_tools_bit_sniffer_0.set_scroll(self.scroll)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate

    def get_offset(self):
        return self.offset

    def set_offset(self, offset):
        self.offset = offset
        self.capture_tools_bit_sniffer_0.set_offset(self.offset)

    def get_hex_output(self):
        return self.hex_output

    def set_hex_output(self, hex_output):
        self.hex_output = hex_output
        self._hex_output_callback(self.hex_output)
        self.capture_tools_bit_sniffer_0.set_hexadecimal(self.hex_output)

    def get_bits_per_word(self):
        return self.bits_per_word

    def set_bits_per_word(self, bits_per_word):
        self.bits_per_word = bits_per_word
        self.capture_tools_bit_sniffer_0.set_bits_per_word(self.bits_per_word)

    def get_binary_output(self):
        return self.binary_output

    def set_binary_output(self, binary_output):
        self.binary_output = binary_output
        self._binary_output_callback(self.binary_output)
        self.capture_tools_bit_sniffer_0.set_binary(self.binary_output)

    def get_ascii_output(self):
        return self.ascii_output

    def set_ascii_output(self, ascii_output):
        self.ascii_output = ascii_output
        self._ascii_output_callback(self.ascii_output)
        self.capture_tools_bit_sniffer_0.set_ascii(self.ascii_output)


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
