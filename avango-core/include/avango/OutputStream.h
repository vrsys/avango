// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#if !defined(AV_OUTPUTSTREAM_H)
#define AV_OUTPUTSTREAM_H

/**
 * \file
 * \ingroup av
 */

#include "windows_specific.h"

#if defined(_MSC_VER)
#include "stdint_replacement.h"
#else
#include <inttypes.h>
#endif

#include <ostream>
#include <utility>
#include <vector>

namespace av
{
class WriteAction;

/**
 * Class for writing Avango Objects to a stream. This is used
 * for persistence and network distribution.
 *
 * \ingroup av
 */
class AV_DLL OutputStream : public std::ostream
{
  public:
    /**
     * Constructor
     */
    OutputStream();

    /**
     * Construct an OutputStream from an existing ostream
     */
    OutputStream(std::ostream&);

    /**
     * Construct an OutputStream from an existing streambuf
     */
    OutputStream(std::streambuf*);

    /**
     * Assign an ostream to an OutputStream
     */
    OutputStream& operator=(std::ostream&);

    /**
     * Assign a streambuf to an OutputStream
     */
    OutputStream& operator=(std::streambuf*);

    /**
     * Switch for enabling binary mode
     */
    void enableBinary(bool on);

    /**
     * Return \c true if the OutputStream is in binary mode
     */
    bool isBinaryEnabled() const;

    /**
     * Set WriteAction for this stream.
     */
    void setWriteAction(WriteAction* writeAction);

    /**
     * Get WriteAction for this stream.
     */
    av::WriteAction* getWriteAction();

  private:
    av::WriteAction* mWriteAction;
    bool mBinary;
};

/**
 * Write end-of-line to the OutputStream.
 *
 * \ingroup av
 */
AV_DLL OutputStream& endl(OutputStream&);

/**
 * Flush the OutputStream.
 *
 * \ingroup av
 */
AV_DLL OutputStream& flush(OutputStream&);

// basic types
AV_DLL OutputStream& operator<<(OutputStream& os, const int16_t& value);
AV_DLL OutputStream& operator<<(OutputStream& os, const uint16_t& value);
AV_DLL OutputStream& operator<<(OutputStream& os, const int32_t& value);
AV_DLL OutputStream& operator<<(OutputStream& os, const uint32_t& value);
AV_DLL OutputStream& operator<<(OutputStream& os, const int64_t& value);
AV_DLL OutputStream& operator<<(OutputStream& os, const uint64_t& value);
AV_DLL OutputStream& operator<<(OutputStream& os, const float& value);
AV_DLL OutputStream& operator<<(OutputStream& os, const double& value);
AV_DLL OutputStream& operator<<(OutputStream& os, const bool& value);
AV_DLL OutputStream& operator<<(OutputStream& os, const std::string& value);

template <typename T1, typename T2>
OutputStream& operator<<(OutputStream& os, const std::pair<T1, T2>& value)
{
    return os << value.first << value.second;
}

template <typename T>
OutputStream& operator<<(OutputStream& os, const std::vector<T>& value)
{
    os << value.size();

    typename std::vector<T>::const_iterator it(value.begin());

    while(it != value.end())
    {
        os << *it;

        ++it;
    }

    return os;
}

} // namespace av

#endif // #if !defined(AV_OUTPUTSTREAM_H)
