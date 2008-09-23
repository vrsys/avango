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

#if !defined(AV_UUID_H)
#define AV_UUID_H

/**
 * \file
 * \ingroup av
 */

#include <string>

#include "windows_specific.h"

namespace av
{
  class InputStream;
  class OutputStream;

  /**
   * Interface for generating UUIDs. This is primarily
   * used for having unique identifiers for distributed nodes.
   *
   * \ingroup av
   */
  class AV_DLL UUID
  {

  public:

    /**
     * Hash Functor for using UUID in hash_map.
     */
    class Hasher
    {
    public:
      size_t operator()(const av::UUID& uuidToHash) const;
    };

    /**
     * Null ID
     */
    static const av::UUID nil;

    /**
     * Null ID
     */
    static const av::UUID null;

    /**
     * Access uuid as raw values.
     */
    struct RawType
    {
      unsigned char bits[16];
    };

    /**
     * Constructor
     */
    UUID();

    /**
     * Copy Constructor
     */
    UUID(const av::UUID& rhs);

    /**
     * Construct an UUID from raw values.
     */
    UUID(const RawType& uuidRaw);

    /**
     * Construct an UUID from a string represantation.
     */
    UUID(const std::string& uuidString);

    /**
     * Destructor
     */
    virtual ~UUID();

    /**
     * Access UUID as raw values
     */
    operator RawType() const;

    /**
     * Access UUID as string representation
     */
    operator std::string() const;

    /**
     * Assignment from another UUID
     */
    const av::UUID& operator= (const UUID& rhs);

    friend bool operator== (const UUID&, const UUID&);
    friend bool operator!= (const UUID&, const UUID&);

    /**
     * Returns true when the UUID values are valid.
     */
    bool valid() const;

  private:

    struct
    {
      unsigned mTimeLow                 :32;
      unsigned mTimeMid                 :16;
      unsigned mTimeHiAndVersion        :16;
      unsigned mClockSeqHiAndReserved    :8;
      unsigned mClockSeqLow              :8;
      unsigned mNode0                    :8;
      unsigned mNode1                    :8;
      unsigned mNode2                    :8;
      unsigned mNode3                    :8;
      unsigned mNode4                    :8;
      unsigned mNode5                    :8;
    } mId;

  };

  /**
   * Returns true when the UUID values are equal.
   */
  bool operator== (const UUID&, const UUID&);

  /**
   * Returns true when the UUID values are not equal.
   */
  bool operator!= (const UUID&, const UUID&);

  InputStream& operator>>(InputStream& is, UUID& value);
  OutputStream& operator<<(OutputStream& os, const UUID& value);

}

#endif // #if !defined(AV_UUID_H)
