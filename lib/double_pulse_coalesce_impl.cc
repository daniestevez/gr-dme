/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "double_pulse_coalesce_impl.h"
#include <gnuradio/dme/pulse_detector.h>
#include <gnuradio/io_signature.h>
#include <algorithm>
#include <stdexcept>

namespace gr {
namespace dme {

double_pulse_coalesce::sptr
double_pulse_coalesce::make(int min_distance, int max_distance, int delay)
{
    return gnuradio::make_block_sptr<double_pulse_coalesce_impl>(
        min_distance, max_distance, delay);
}

double_pulse_coalesce_impl::double_pulse_coalesce_impl(int min_distance,
                                                       int max_distance,
                                                       int delay)
    : gr::sync_block("double_pulse_coalesce",
                     gr::io_signature::make(1, 1, sizeof(gr_complex)),
                     gr::io_signature::make(1, 1, sizeof(gr_complex))),
      d_min_distance(min_distance),
      d_max_distance(max_distance),
      d_delay(delay)
{
    if (min_distance <= 0) {
        throw std::runtime_error("[double_pulse_coalesce] min_distance must be positive");
    }
    if (max_distance < min_distance) {
        throw std::runtime_error(
            "[double_pulse_coalesce] max_distance cannot be less than min_distance");
    }
    if (delay < 0) {
        throw std::runtime_error("[double_pulse_coalesce] delay cannot be negative");
    }

    set_history(max_distance);
    declare_sample_delay(max_distance - 1);
    set_tag_propagation_policy(TPP_DONT);
}

double_pulse_coalesce_impl::~double_pulse_coalesce_impl() {}

int double_pulse_coalesce_impl::work(int noutput_items,
                                     gr_vector_const_void_star& input_items,
                                     gr_vector_void_star& output_items)
{
    auto in = static_cast<const gr_complex*>(input_items[0]);
    auto out = static_cast<gr_complex*>(output_items[0]);

    const auto nitems = nitems_written(0);
    std::vector<gr::tag_t> tags;
    get_tags_in_range(tags, 0, nitems, nitems + noutput_items);
    const auto _pulse_key = pulse_key();
    for (const auto& tag : tags) {
        if (!pmt::eqv(tag.key, _pulse_key)) {
            // pass non-pulse_key() tag unmodified
            add_item_tag(0, tag);
            continue;
        }

        std::vector<gr::tag_t> second_tags;
        get_tags_in_range(second_tags,
                          0,
                          tag.offset + d_min_distance,
                          tag.offset + d_max_distance,
                          _pulse_key);
        if (!second_tags.empty()) {
            add_item_tag(0, tag.offset + d_delay, tag.key, tag.value);
        }
    }

    std::copy_n(in, noutput_items, out);

    return noutput_items;
}

} /* namespace dme */
} /* namespace gr */
