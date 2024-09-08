/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DME_DOUBLE_PULSE_COALESCE_IMPL_H
#define INCLUDED_DME_DOUBLE_PULSE_COALESCE_IMPL_H

#include <gnuradio/dme/double_pulse_coalesce.h>

namespace gr {
namespace dme {

class double_pulse_coalesce_impl : public double_pulse_coalesce
{
private:
    const int d_min_distance;
    const int d_max_distance;
    const int d_delay;

public:
    double_pulse_coalesce_impl(int min_distance, int max_distance, int delay);
    ~double_pulse_coalesce_impl() override;

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace dme
} // namespace gr

#endif /* INCLUDED_DME_DOUBLE_PULSE_COALESCE_IMPL_H */
