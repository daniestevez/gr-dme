/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DME_PULSE_DETECTOR_IMPL_H
#define INCLUDED_DME_PULSE_DETECTOR_IMPL_H

#include <gnuradio/dme/pulse_detector.h>

namespace gr {
namespace dme {

class pulse_detector_impl : public pulse_detector
{
private:
    const int d_window_size;
    float d_threshold;
    float d_noise_alpha;
    float d_noise_pwr_estimate = 0.0f;
    int d_window_remain = 0; // used to avoid multiple detections in the same window

public:
    pulse_detector_impl(int window_size, float threshold, float noise_alpha);
    ~pulse_detector_impl() override;

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;

    float threshold() const override { return d_threshold; }

    float noise_alpha() const override { return d_noise_alpha; }

    void set_threshold(float threshold) override { d_threshold = threshold; }

    void set_noise_alpha(float noise_alpha) override { d_noise_alpha = noise_alpha; }
};

} // namespace dme
} // namespace gr

#endif /* INCLUDED_DME_PULSE_DETECTOR_IMPL_H */
