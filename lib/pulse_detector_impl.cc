/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "pulse_detector_impl.h"
#include <gnuradio/io_signature.h>
#include <algorithm>
#include <stdexcept>

namespace gr {
namespace dme {

const pmt::pmt_t pulse_key()
{
    static const auto key = pmt::string_to_symbol("pulse");
    return key;
}

pulse_detector::sptr
pulse_detector::make(int window_size, float threshold, float noise_alpha)
{
    return gnuradio::make_block_sptr<pulse_detector_impl>(
        window_size, threshold, noise_alpha);
}

pulse_detector_impl::pulse_detector_impl(int window_size,
                                         float threshold,
                                         float noise_alpha)
    : gr::sync_block("pulse_detector",
                     gr::io_signature::makev(
                         2, 2, std::vector<int>{ sizeof(gr_complex), sizeof(float) }),
                     gr::io_signature::make(1, 1, sizeof(gr_complex))),
      d_window_size(window_size),
      d_threshold(threshold),
      d_noise_alpha(noise_alpha)
{
    if (window_size <= 0) {
        throw std::runtime_error("[pulse_detector] window_size must be positive");
    }
    set_history(window_size);
    declare_sample_delay(window_size - 1);
}

pulse_detector_impl::~pulse_detector_impl() {}

int pulse_detector_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    auto in_complex = static_cast<const gr_complex*>(input_items[0]);
    auto in_mf_power = static_cast<const float*>(input_items[1]);
    auto out = static_cast<gr_complex*>(output_items[0]);

    for (int j = 0; j < noutput_items; ++j) {
        d_noise_pwr_estimate =
            d_noise_alpha * in_mf_power[j] + (1 - d_noise_alpha) * d_noise_pwr_estimate;

        if (d_window_remain == 0 &&
            in_mf_power[j] >= d_threshold * d_noise_pwr_estimate) {
            // detection happened: look ahead in the window to find maximum
            const int peak_location =
                std::max_element(&in_mf_power[j], &in_mf_power[j + d_window_size]) -
                &in_mf_power[0];
            add_item_tag(0, nitems_written(0) + peak_location, pulse_key(), pmt::PMT_NIL);
            d_window_remain = d_window_size;
        }

        if (d_window_remain > 0) {
            --d_window_remain;
        }
    }

    std::copy_n(in_complex, noutput_items, out);

    return noutput_items;
}

} /* namespace dme */
} /* namespace gr */
