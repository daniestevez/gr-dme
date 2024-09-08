/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DME_POLYPHASE_FINE_ESTIMATE_IMPL_H
#define INCLUDED_DME_POLYPHASE_FINE_ESTIMATE_IMPL_H

#include <gnuradio/dme/polyphase_fine_estimate.h>
#include <volk/volk_alloc.hh>

namespace gr {
namespace dme {

class polyphase_fine_estimate_impl : public polyphase_fine_estimate
{
private:
    const int d_window_size;
    std::vector<volk::vector<float>> d_taps;

public:
    polyphase_fine_estimate_impl(const std::vector<float>& polyphase_taps,
                                 int num_branches,
                                 int window_size);
    ~polyphase_fine_estimate_impl() override;

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace dme
} // namespace gr

#endif /* INCLUDED_DME_POLYPHASE_FINE_ESTIMATE_IMPL_H */
