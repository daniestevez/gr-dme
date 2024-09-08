/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DME_PULSE_DETECTOR_H
#define INCLUDED_DME_PULSE_DETECTOR_H

#include <gnuradio/dme/api.h>
#include <gnuradio/sync_block.h>
#include <pmt/pmt.h>

namespace gr {
namespace dme {

DME_API const pmt::pmt_t pulse_key();

/*!
 * \brief Pulse Detector
 * \ingroup dme
 *
 * Detects pulses by comparing looking at the output of a matched filter and
 * comparing the amplitude with a noise amplitude obtained with a 1-pole IIR
 * filter on the matched filter amplitude.
 *
 * The block has two input ports, one for the original complex signal, and
 * another one for the power of a matched filter output (both need to be aligned
 * in time apropriately by delaying the original signal). It keeps a running
 * estimate of the noise floor power by filtering the power of the matched
 * filter output with a 1-pole IIR filter. When the matched filter output power
 * is greater than 'threshold' times the noise estimate, a detection is
 * declared. For each detection, the sample that has the maximum amplitude in a
 * window of 'window_size' samples starting from the sample where the detection
 * was declared is considered the pulse peak. A tag is attached to the pulse
 * peak in the output. The output samples are equal to the original signal
 * input.
 */
class DME_API pulse_detector : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<pulse_detector> sptr;

    /*!
     * \brief Creates a Pulse Detector.
     *
     * \param window_size The size of the window for peak searching when a
     * detection happens.
     * \param threshold Detection threshold.
     * \param noise_alpha IIR filter coefficient for the noise floor power
     * estimation.
     */
    static sptr make(int window_size, float threshold, float noise_alpha);

    virtual float threshold() const = 0;
    virtual float noise_alpha() const = 0;
    virtual void set_threshold(float threshold) = 0;
    virtual void set_noise_alpha(float noise_alpha) = 0;
};

} // namespace dme
} // namespace gr

#endif /* INCLUDED_DME_PULSE_DETECTOR_H */
