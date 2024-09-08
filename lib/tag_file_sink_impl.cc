/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tag_file_sink_impl.h"
#include <gnuradio/dme/polyphase_fine_estimate.h>
#include <gnuradio/io_signature.h>

namespace gr {
namespace dme {

tag_file_sink::sptr tag_file_sink::make(const std::string& offset_file,
                                        const std::string& mf_file)
{
    return gnuradio::make_block_sptr<tag_file_sink_impl>(offset_file, mf_file);
}

tag_file_sink_impl::tag_file_sink_impl(const std::string& offset_file,
                                       const std::string& mf_file)
    : gr::sync_block("tag_file_sink",
                     gr::io_signature::make(1, 1, sizeof(gr_complex)),
                     gr::io_signature::make(0, 0, 0))
{
    d_offset_file.open(offset_file);
    d_mf_file.open(mf_file);
}

tag_file_sink_impl::~tag_file_sink_impl() {}

int tag_file_sink_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items)
{
    const auto time_key = fine_time_key();
    const auto mf_key = matched_filter_out_key();

    std::vector<gr::tag_t> tags;
    get_tags_in_window(tags, 0, 0, noutput_items);
    for (const auto& tag : tags) {
        if (pmt::eqv(tag.key, time_key)) {
            const double time = static_cast<double>(tag.offset) +
                                static_cast<double>(pmt::to_float(tag.value));
            d_offset_file.write(reinterpret_cast<const char*>(&time), sizeof(time));
        } else if (pmt::eqv(tag.key, mf_key)) {
            const gr_complex mf{ pmt::to_complex(tag.value) };
            d_mf_file.write(reinterpret_cast<const char*>(&mf), sizeof(mf));
        }
    }

    return noutput_items;
}

} /* namespace dme */
} /* namespace gr */
