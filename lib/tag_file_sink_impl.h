/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DME_TAG_FILE_SINK_IMPL_H
#define INCLUDED_DME_TAG_FILE_SINK_IMPL_H

#include <gnuradio/dme/tag_file_sink.h>
#include <fstream>

namespace gr {
namespace dme {

class tag_file_sink_impl : public tag_file_sink
{
private:
    std::ofstream d_offset_file;
    std::ofstream d_mf_file;

public:
    tag_file_sink_impl(const std::string& offset_file, const std::string& mf_file);
    ~tag_file_sink_impl() override;

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace dme
} // namespace gr

#endif /* INCLUDED_DME_TAG_FILE_SINK_IMPL_H */
