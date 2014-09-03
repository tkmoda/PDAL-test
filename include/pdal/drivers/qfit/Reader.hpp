/******************************************************************************
* Copyright (c) 2011, Howard Butler, hobu.inc@gmail.com
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#pragma once

#include <vector>

#include <boost/detail/endian.hpp>

#include <pdal/Reader.hpp>
#include <pdal/StageIterator.hpp>
#include <pdal/Options.hpp>

#ifdef BOOST_LITTLE_ENDIAN
# define QFIT_SWAP_BE_TO_LE(p) \
    do { \
        char* first = static_cast<char*>(static_cast<void*>(&p)); \
        char* last = first + sizeof(p) - 1; \
        for(; first < last; ++first, --last) { \
            char const x = *last; \
            *last = *first; \
            *first = x; \
        }} while(false)

# define QFIT_SWAP_BE_TO_LE_N(p, n) \
    do { \
        char* first = static_cast<char*>(static_cast<void*>(&p)); \
        char* last = first + n - 1; \
        for(; first < last; ++first, --last) { \
            char const x = *last; \
            *last = *first; \
            *first = x; \
        }} while(false)
#endif

namespace pdal
{
namespace drivers
{
namespace qfit
{

namespace iterators
{
namespace sequential
{
class Reader;
} //namespace sequential
} //namespace iterators

enum QFIT_Format_Type
{
    QFIT_Format_10 = 10,
    QFIT_Format_12 = 12,
    QFIT_Format_14 = 14,
    QFIT_Format_Unknown = 128
};

class qfit_error : public pdal_error
{
public:

    qfit_error(std::string const& msg)
        : pdal_error(msg)
    {}
};

// supported options:
//   <uint32>id
//   <bool>debug
//   <uint32>verbose
//   <string>filename  [required]
//
class PDAL_DLL Reader : public pdal::Reader
{
    friend class iterators::sequential::Reader;
public:
    SET_STAGE_NAME("drivers.qfit.reader", "QFIT Reader")
    SET_STAGE_LINK("http://pdal.io/stages/drivers.qfit.reader.html")
    SET_STAGE_ENABLED(true)

    Reader(const Options& options);

    static Options getDefaultOptions();
    static Dimension::IdList getDefaultDimensions();

    std::string getFileName() const;

    StageSequentialIterator *createSequentialIterator() const;

    std::size_t getPointDataOffset() const
        { return m_offset; }
    boost::uint32_t getPointDataSize() const
        { return m_size; }
    point_count_t getNumPoints() const
        { return m_numPoints; }

    // this is called by the stage's iterator
    point_count_t processBuffer(PointBuffer& PointBuffer, std::istream& stream,
        point_count_t count) const;

private:
    std::string m_filename;
    QFIT_Format_Type m_format;
    std::ios::off_type m_point_bytes;
    std::size_t m_offset;
    uint32_t m_size;
    bool m_flip_x;
    double m_scale_z;
    bool m_littleEndian;
    point_count_t m_numPoints;

    virtual void processOptions(const Options& ops);
    virtual void initialize();
    virtual void addDimensions(PointContext ctx);
    virtual void ready(PointContext ctx);

    Reader& operator=(const Reader&); // not implemented
    Reader(const Reader&); // not implemented
};

namespace iterators
{
namespace sequential
{

class Reader : public StageSequentialIterator
{
public:
    Reader(const qfit::Reader& reader);
    ~Reader();

private:
    uint64_t skipImpl(uint64_t);
    point_count_t readBufferImpl(PointBuffer&);
    bool atEndImpl() const
        { return getIndex() >= m_reader.getNumPoints(); }

    const pdal::drivers::qfit::Reader& m_reader;
    std::istream* m_istream;
};

} // namespace sequential
} // namespace iterators

} // namespace qfit
} // namespace drivers
} // namespace pdal

