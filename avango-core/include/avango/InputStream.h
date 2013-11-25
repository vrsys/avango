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

#if !defined(AV_INPUTSTREAM_H)
#define AV_INPUTSTREAM_H

/**
 * \file
 * \ingroup av
 */

#include "windows_specific.h"

#if defined(_MSC_VER)
  #include "stdint_replacement.h"
#else
  #include <inttypes.h>
#endif // _MSC_VER

#include <istream>
#include <utility>
#include <vector>


namespace av
{

  class Reader;

  /**
   * Class for reading Avango Objects from a stream. This is used
   * for persistence and network distribution.
   *
   * \ingroup av
   */

  class AV_DLL InputStream : public std::istream
  {

  public:

    /**
     * Constructor
     */
    InputStream();

    /**
     * Construct an InputStream from an existing istream
     */
    InputStream(std::istream& is);

    /**
     * Construct an InputStream from an existing streambuf
     */
    InputStream(std::streambuf* sb);

    /**
     * Assign an istream to an InputStream
     */
    InputStream& operator=(std::istream& is);

    /**
     * Assign a streambuf to an InputStream
     */
    InputStream& operator=(std::streambuf* sb);

    /**
     * Switch for enabling binary mode
     */
    void enableBinary(bool on);

    /**
     * Return \c true if the InputStream is in binary mode
     */
    bool isBinaryEnabled() const;

    /**
     * Set Reader for this stream.
     */
    void setReader(Reader* reader);

    /**
     * Get Reader for this Stream
     */
    Reader* getReader();

  private:

    bool mBinary;
    Reader* mReader;

  };

  // basic types

  AV_DLL InputStream& operator>>(InputStream& is, int16_t& value);
  AV_DLL InputStream& operator>>(InputStream& is, uint16_t& value);
  AV_DLL InputStream& operator>>(InputStream& is, int32_t& value);
  AV_DLL InputStream& operator>>(InputStream& is, uint32_t& value);
  AV_DLL InputStream& operator>>(InputStream& is, int64_t& value);
  AV_DLL InputStream& operator>>(InputStream& is, uint64_t& value);
  AV_DLL InputStream& operator>>(InputStream& is, float& value);
  AV_DLL InputStream& operator>>(InputStream& is, double& value);
  AV_DLL InputStream& operator>>(InputStream& is, bool& value);
  AV_DLL InputStream& operator>>(InputStream& is, std::string& value);

  template <typename T1, typename T2> InputStream&
    operator>>(InputStream& is, std::pair<T1, T2>& value)
  {
    return is >> value.first >> value.second;
  }

  template <typename T> InputStream& operator>>(InputStream& is, std::vector<T>& value)
  {
    size_t count;

    is >> count;

    while (count) {
      T tmp;

      is >> tmp;
      value.push_back(tmp);

      --count;
    }

    return is;
  }

}

#endif // #if !defined(AV_INPUTSTREAM_H)
