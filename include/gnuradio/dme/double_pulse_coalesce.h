/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DME_DOUBLE_PULSE_COALESCE_H
#define INCLUDED_DME_DOUBLE_PULSE_COALESCE_H

#include <gnuradio/dme/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace dme {

/*!
 * \brief Double Pulse Coalesce
 * \ingroup dme
 *
 * Coalesces two pulse detection tags into one, as long as the two pulse
 * detection tags are present.
 *
 * For each pulse tag received by this block, it checks if there is another
 * pulse tag between min_distance and max_distance (both included) samples away
 * in the future. If there is such a tag, then a pulse tag is inserted at the
 * output at the position of the first tag, delayed by 'delay' samples. If there
 * is no second tag, then the input tag is dropped.
 */
class DME_API double_pulse_coalesce : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<double_pulse_coalesce> sptr;

    /*!
     * \brief Creates a Double Pulse Coalesce
     *
     * \param min_distance Minimum distance for second pulse search.
     * \param max_distance Maximum distance for second pulse search.
     * \param delay Delay to apply to the output tag.
     */
    static sptr make(int min_distance, int max_distance, int delay);
};

} // namespace dme
} // namespace gr

#endif /* INCLUDED_DME_DOUBLE_PULSE_COALESCE_H */
