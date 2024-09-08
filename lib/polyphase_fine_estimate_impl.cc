/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "polyphase_fine_estimate_impl.h"
#include <gnuradio/dme/pulse_detector.h>
#include <gnuradio/io_signature.h>
#include <volk/volk.h>
#include <algorithm>
#include <stdexcept>

namespace gr {
namespace dme {

const pmt::pmt_t matched_filter_out_key()
{
    static const auto key = pmt::string_to_symbol("matched_filter_out");
    return key;
}

const pmt::pmt_t fine_time_key()
{
    static const auto key = pmt::string_to_symbol("fine_time");
    return key;
}

polyphase_fine_estimate::sptr polyphase_fine_estimate::make(
    const std::vector<float>& polyphase_taps, int num_branches, int window_size)
{
    return gnuradio::make_block_sptr<polyphase_fine_estimate_impl>(
        polyphase_taps, num_branches, window_size);
}

polyphase_fine_estimate_impl::polyphase_fine_estimate_impl(
    const std::vector<float>& polyphase_taps, int num_branches, int window_size)
    : gr::sync_block("polyphase_fine_estimate",
                     gr::io_signature::make(1, 1, sizeof(gr_complex)),
                     gr::io_signature::make(1, 1, sizeof(gr_complex))),
      d_window_size(window_size)
{
    if (window_size < 0) {
        throw std::runtime_error(
            "[polyphase_fine_estimate] window_size cannot be negative");
    }

    if (polyphase_taps.size() % num_branches != 0) {
        throw std::runtime_error("[polyphase_fine_estimate] the length of polyphase_taps "
                                 "must be divisible by num_branches");
    }

    const int taps_per_branch = polyphase_taps.size() / num_branches;
    for (int j = 0; j < num_branches; ++j) {
        d_taps.emplace_back(taps_per_branch);
        for (int k = 0; k < taps_per_branch; ++k) {
            d_taps[j][k] = polyphase_taps[j + num_branches * k];
        }
    }

    set_history(2 * window_size + taps_per_branch);
    declare_sample_delay(0, 2 * window_size + taps_per_branch - 1);
    set_tag_propagation_policy(TPP_DONT);
}

polyphase_fine_estimate_impl::~polyphase_fine_estimate_impl() {}

int polyphase_fine_estimate_impl::work(int noutput_items,
                                       gr_vector_const_void_star& input_items,
                                       gr_vector_void_star& output_items)
{
    auto in = static_cast<const gr_complex*>(input_items[0]);
    auto out = static_cast<gr_complex*>(output_items[0]);

    const int num_branches = d_taps.size();
    const int taps_per_branch = d_taps[0].size();

    const auto nitems = nitems_written(0);
    const auto tags_begin = nitems + d_window_size + taps_per_branch / 2;
    const auto tags_end = tags_begin + noutput_items;
    std::vector<gr::tag_t> tags;
    get_tags_in_range(tags, 0, tags_begin, tags_end);
    const auto _pulse_key = pulse_key();
    for (const auto& tag : tags) {
        if (!pmt::eqv(tag.key, _pulse_key)) {
            // pass non-pulse_key() tag unmodified
            add_item_tag(0, tag);
            continue;
        }

        const int idx = tag.offset - nitems;
        int best_w = 0;
        int best_branch = 0;
        float best_pwr = 0.0f;
        gr_complex best_mf = {};
        for (int w = -d_window_size; w <= d_window_size; ++w) {
            for (int branch = 0; branch < num_branches; ++branch) {
                gr_complex mf_out;
                volk_32fc_32f_dot_prod_32fc(&mf_out,
                                            &in[idx - taps_per_branch / 2 + w],
                                            d_taps[branch].data(),
                                            taps_per_branch);
                const float pwr =
                    mf_out.real() * mf_out.real() + mf_out.imag() * mf_out.imag();
                if (pwr > best_pwr) {
                    best_w = w;
                    best_branch = branch;
                    best_pwr = pwr;
                    best_mf = mf_out;
                }
            }
        }
        const auto offset = tag.offset + best_w;
        add_item_tag(0, offset, tag.key, tag.value);
        add_item_tag(0, offset, matched_filter_out_key(), pmt::from_complex(best_mf));
        const float fine_time = static_cast<float>(num_branches / 2 - best_branch) /
                                static_cast<float>(num_branches);
        add_item_tag(0, offset, fine_time_key(), pmt::from_float(fine_time));
    }

    std::copy_n(in, noutput_items, out);

    return noutput_items;
}

} /* namespace dme */
} /* namespace gr */
