/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DME_POLYPHASE_FINE_ESTIMATE_H
#define INCLUDED_DME_POLYPHASE_FINE_ESTIMATE_H

#include <gnuradio/dme/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace dme {

DME_API const pmt::pmt_t matched_filter_out_key();
DME_API const pmt::pmt_t fine_time_key();

/*!
 * \brief Polyphase Fine Estimate
 * \ingroup dme
 *
 * Performs a fine estimate of DME double pulses timing by using a polyphase
 * matched filter and finding the position and phase of the filter that gives
 * maximum power.
 */
class DME_API polyphase_fine_estimate : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<polyphase_fine_estimate> sptr;

    /*!
     * \brief Creates A Polyphase Fine Estimate block
     *
     * \param polyphase_taps Taps of the polyphase matched filter.
     * \param num_branches Number of branches of the polyphase filter.
     * \param window_size The polyphase filter is slided +/-window_size samples
     * around the a-priori location of the double pulse.
     */
    static sptr
    make(const std::vector<float>& polyphase_taps, int num_branches, int window_size);
};

} // namespace dme
} // namespace gr

#endif /* INCLUDED_DME_POLYPHASE_FINE_ESTIMATE_H */
