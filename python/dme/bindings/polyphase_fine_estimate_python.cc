/*
 * Copyright 2024 Free Software Foundation, Inc.
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
/* BINDTOOL_HEADER_FILE(polyphase_fine_estimate.h) */
/* BINDTOOL_HEADER_FILE_HASH(43a2993004cf1f01a214317e4b1cb4f2)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/dme/polyphase_fine_estimate.h>
// pydoc.h is automatically generated in the build directory
#include <polyphase_fine_estimate_pydoc.h>

void bind_polyphase_fine_estimate(py::module& m)
{

    using polyphase_fine_estimate = ::gr::dme::polyphase_fine_estimate;


    py::class_<polyphase_fine_estimate,
               gr::sync_block,
               gr::block,
               gr::basic_block,
               std::shared_ptr<polyphase_fine_estimate>>(
        m, "polyphase_fine_estimate", D(polyphase_fine_estimate))

        .def(py::init(&polyphase_fine_estimate::make),
             py::arg("polyphase_taps"),
             py::arg("num_branches"),
             py::arg("window_size"),
             D(polyphase_fine_estimate, make))


        ;


    m.def("matched_filter_out_key",
          &::gr::dme::matched_filter_out_key,
          D(matched_filter_out_key));


    m.def("fine_time_key", &::gr::dme::fine_time_key, D(fine_time_key));
}
