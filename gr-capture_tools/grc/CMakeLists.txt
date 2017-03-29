# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

install(FILES
    capture_tools_file_sink_text.xml
    capture_tools_one_then_zero.xml
    capture_tools_file_sink_on_message.xml
    capture_tools_level_alert.xml
    capture_tools_add_before_and_after_cc.xml
    capture_tools_repeat_input_n_times_cc.xml
    capture_tools_keep_middle_in_vector.xml DESTINATION share/gnuradio/grc/blocks
)
