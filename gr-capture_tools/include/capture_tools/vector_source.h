/* -*- c++ -*- */
/*
 * Copyright 2004,2008,2012-2013,2018 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef VECTOR_SOURCE_H
#define VECTOR_SOURCE_H

#include <capture_tools/api.h>
#include <gnuradio/sync_block.h>
#include <cstdint>

namespace gr {
namespace capture_tools {

/*!
 * \brief Source that streams T items based on the input \p data vector.
 * \ingroup misc_blk
 *
 * \details
 * This block produces a stream of samples based on an input
 * vector. In C++, this is a std::vector<T>, and in Python,
 * this is either a list or tuple. The data can repeat infinitely
 * until the flowgraph is terminated by some other event or, the
 * default, run the data once and stop.
 *
 * The vector source can also produce stream tags with the
 * data. Pass in a vector of gr::tag_t objects and they will be
 * emitted based on the specified offset of the tag.
 *
 * GNU Radio provides a utility Python module in gr.tag_utils to
 * convert between tags and Python objects:
 * gr.tag_utils.python_to_tag.
 *
 * We can create tags as Python lists (or tuples) using the list
 * structure [int offset, pmt key, pmt value, pmt srcid]. It is
 * important to define the list/tuple with the values in the
 * correct order and with the correct data type. A python
 * dictionary can also be used using the keys: "offset", "key",
 * "value", and "srcid" with the same data types as for the lists.
 *
 * When given a list of tags, the vector source will emit the tags
 * repeatedly by updating the offset relative to the vector stream
 * length. That is, if the vector has 500 items and a tag has an
 * offset of 0, that tag will be placed on item 0, 500, 1000,
 * 1500, etc.
 */
template <class T>
class CAPTURE_TOOLS_API vector_source : virtual public sync_block
{
public:
    // gr::blocks::vector_source::sptr
    typedef boost::shared_ptr<vector_source<T>> sptr;

    static sptr make(const std::vector<T>& data,
                     bool repeat = false,
                     unsigned int vlen = 1,
                     const std::vector<tag_t>& tags = std::vector<tag_t>());

    virtual void rewind() = 0;
    virtual void set_data(const std::vector<T>& data,
                          const std::vector<tag_t>& tags = std::vector<tag_t>()) = 0;
    virtual void set_repeat(bool repeat) = 0;
};

typedef vector_source<std::uint8_t> vector_source_b;
typedef vector_source<std::int16_t> vector_source_s;
typedef vector_source<std::int32_t> vector_source_i;
typedef vector_source<float> vector_source_f;
typedef vector_source<gr_complex> vector_source_c;
} /* namespace capture_tools */
} /* namespace gr */

#endif /* VECTOR_SOURCE_H */
