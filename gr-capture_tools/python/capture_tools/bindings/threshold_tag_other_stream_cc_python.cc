/*
 * Copyright 2022 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(threshold_tag_other_stream_cc.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(ab502dd868e5fbb5afe1a304b912be8f)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/capture_tools/threshold_tag_other_stream_cc.h>
// pydoc.h is automatically generated in the build directory
#include <threshold_tag_other_stream_cc_pydoc.h>

void bind_threshold_tag_other_stream_cc(py::module& m)
{

    using threshold_tag_other_stream_cc    = ::gr::capture_tools::threshold_tag_other_stream_cc;


    py::class_<threshold_tag_other_stream_cc, gr::sync_block, gr::block, gr::basic_block,
        std::shared_ptr<threshold_tag_other_stream_cc>>(m, "threshold_tag_other_stream_cc", D(threshold_tag_other_stream_cc))

        .def(py::init(&threshold_tag_other_stream_cc::make),
           py::arg("low_thres"),
           py::arg("high_thres"),
           py::arg("vlen") = 1,
           D(threshold_tag_other_stream_cc,make)
        )
        




        
        .def("set_low_thresh",&threshold_tag_other_stream_cc::set_low_thresh,       
            py::arg("arg0"),
            D(threshold_tag_other_stream_cc,set_low_thresh)
        )


        
        .def("set_high_thresh",&threshold_tag_other_stream_cc::set_high_thresh,       
            py::arg("arg0"),
            D(threshold_tag_other_stream_cc,set_high_thresh)
        )


        
        .def("set_going_high_tag",&threshold_tag_other_stream_cc::set_going_high_tag,       
            py::arg("tag_str"),
            D(threshold_tag_other_stream_cc,set_going_high_tag)
        )


        
        .def("set_going_low_tag",&threshold_tag_other_stream_cc::set_going_low_tag,       
            py::arg("tag_str"),
            D(threshold_tag_other_stream_cc,set_going_low_tag)
        )

        ;




}







