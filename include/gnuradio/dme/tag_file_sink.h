/* -*- c++ -*- */
/*
 * Copyright 2024 Daniel Estevez <daniel@destevez>.
 *
 * This file is part of gr-dme
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DME_TAG_FILE_SINK_H
#define INCLUDED_DME_TAG_FILE_SINK_H

#include <gnuradio/dme/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace dme {

/*!
 * \brief Tag File Sink
 * \ingroup dme
 *
 * Writes tags produced by the DME receiver to files.
 */
class DME_API tag_file_sink : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<tag_file_sink> sptr;

    /*!
     * \brief Creates a Tag File Sink
     *
     * \param offset_file Path for the offset file (double format)
     * \param mf_file Path for the matched filter file (gr_complex format)
     */
    static sptr make(const std::string& offset_file, const std::string& mf_file);
};

} // namespace dme
} // namespace gr

#endif /* INCLUDED_DME_TAG_FILE_SINK_H */
