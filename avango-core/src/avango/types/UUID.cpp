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

/*
 * from <dce-1.2.2-src>/src/src/rpc/runtime/uuid.c (reformatted)
 *
 * Internal structure of universal unique IDs (UUIDs).
 *
 * There are three "variants" of UUIDs that this code knows about.  The variant #0 is
 * what was defined in the 1989 HP/Apollo Network Computing Architecture (NCA)
 * specification and implemented in NCS 1.x and DECrpc v1.  Variant #1 is what was
 * defined for the joint HP/DEC specification for the OSF (in DEC's "UID Architecture
 * Functional Specification Version X1.0.4") and implemented in NCS 2.0, DECrpc v2, and
 * OSF 1.0 DCE RPC.  Variant #2 is defined by Microsoft.
 *
 * This code creates only variant #1 UUIDs.
 *
 * The three UUID variants can exist on the same wire because they have distinct values
 * in the 3 MSB bits of octet 8 (see table below).  Do NOT confuse the version number
 * with these 3 bits.  (Note the distinct use of the terms "version" and "variant".)
 * Variant #0 had no version field in it.  Changes to variant #1 (should any ever need to
 * be made) can be accomodated using the current form's 4 bit version field.
 *
 * The UUID record structure MUST NOT contain padding between fields.  The total size is
 * 128 bits.
 *
 * To minimize confusion about bit assignment within octets, the UUID record definition
 * is defined only in terms of fields that are integral numbers of octets.
 *
 * Depending on the network data representation, the multi-octet unsigned integer fields
 * are subject to byte swapping when communicated between dissimilar endian machines.
 * Note that all three UUID variants have the same record structure; this allows this
 * byte swapping to occur.  (The ways in which the contents of the fields are generated
 * can and do vary.)
 *
 * The following information applies to variant #1 UUIDs:
 *
 * The lowest addressed octet contains the global/local bit and the unicast/multicast bit,
 * and is the first octet of the address transmitted on an 802.3 LAN.
 *
 * The adjusted time stamp is split into three fields, and the clockSeq is split into two
 * fields.
 *
 * |<------------------------- 32 bits -------------------------->|
 *
 * +--------------------------------------------------------------+
 * |                     low 32 bits of time                      |
 * +-------------------------------+-------------------------------
 * |     mid 16 bits of time       |
 * +-------+-----------------------+
 * | vers. |   hi 12 bits of time  |
 * +-------+-------+---------------+
 * |Res|  clkSeqHi |
 * +---------------+
 * |   clkSeqLow   |
 * +---------------+----------...-----+
 * |            node ID               |
 * +--------------------------...-----+
 *
 *  00-03  .time_low                   =>  4 Bytes  => 32 Bits
 *  04-05  .time_mid                   =>  2 Bytes  => 16 Bits
 *  06-07  .time_hi_and_version        =>  2 Bytes  => 16 Bits
 *     08  .clock_seq_hi_and_reserved  =>  1 Byte   =>  8 Bits
 *     09  .clock_seq_low              =>  1 Byte   =>  8 Bits
 *  08-16  .node                       =>  9 Bytes  => 72 Bits
 *
 *
 * The structure layout of all three UUID variants is fixed for all time.  I.e., the
 * layout consists of a 32 bit int, 2 16 bit ints, and 8 8 bit ints.  The current form
 * version field does NOT determine/affect the layout.  This enables us to do certain
 * operations safely on the variants of UUIDs without regard to variant; this increases
 * the utility of this code even as the version number changes (i.e., this code does NOT
 * need to check the version field).
 *
 * The "Res" field in the octet #8 is the so-called "reserved" bit-field and determines
 * whether or not the uuid is a old, current or other UUID as follows:
 *
 *      MS-bit  2MS-bit  3MS-bit      Variant
 *      ---------------------------------------------
 *         0       x        x       0 (NCS 1.5)
 *         1       0        x       1 (DCE 1.0 RPC)
 *         1       1        0       2 (Microsoft)
 *         1       1        1       unspecified
 *
 * Internal structure of variant #0 UUIDs
 *
 * The first 6 octets are the number of 4 usec units of time that have passed since
 * 1/1/80 0000 GMT.  The next 2 octets are reserved for future use.  The next octet is an
 * address family.  The next 7 octets are a host ID in the form allowed by the specified
 * address family.
 *
 * Note that while the family field (octet 8) was originally conceived of as being able
 * to hold values in the range [0..255], only [0..13] were ever used.  Thus, the 2 MSB of
 * this field are always 0 and are used to distinguish old and current UUID forms.
 *
 * +--------------------------------------------------------------+
 * |                    high 32 bits of time                      |  0-3  .time_high
 * +-------------------------------+-------------------------------
 * |     low 16 bits of time       |                                 4-5  .time_low
 * +-------+-----------------------+
 * |         reserved              |                                 6-7  .reserved
 * +---------------+---------------+
 * |    family     |                                                   8  .family
 * +---------------+----------...-----+
 * |            node ID               |                             9-16  .node
 * +--------------------------...-----+
 *
 */

#include <avango/UUID.h>

#include <avango/Config.h>
#include <avango/InputStream.h>
#include <avango/Logger.h>
#include <avango/OutputStream.h>

#include <avango/Config.h>

namespace
{
  av::Logger& logger(av::getLogger("av::UUID"));

#if defined(_WIN32)
  // workaround since no ssize_t is avail under MS VS 8
  typedef long ssize_t;
#endif

  const av::UUID::RawType uuid_nil = {{0}};

  inline std::string
  hex_to_string (unsigned long a, ssize_t digits)
  {
    std::string result;

    while (--digits >= 0) {
      const size_t shift = digits << 2;
      const size_t value = 0x0FL & (a >> shift);

      if (value <= 9) {
        result.push_back ('0' + value);
      } else {
        result.push_back ('A' + (value - 10));
      }
    }

    return result;
  }

  inline unsigned long
  string_to_hex (const char *a, ssize_t digits)
  {
    unsigned long result = 0;

    while (--digits >= 0) {
      char c (*a);

      if ((c >= '0') && (c <= '9')) {
        c -= '0';
      } else if ((c >= 'a') && (c <= 'f')) {
        c -= ('a' - 10);
      } else if ((c >= 'A') && (c <= 'F')) {
        c -= ('A' - 10);
      } else {
        break;
      }

      result = (result << 4) | c;

      if ('\0' != *a) {
        a++;
      }
    }

    return result;
  }

} // namespace

// including <uuid.h> is very unlikely to pass a
// c++ compiler of any kind 8-(

extern "C" {

#if !defined(uint_t)
  typedef unsigned int uint_t;
#endif

#if defined(__linux)
  struct uuid_t {
    unsigned char bits[16];
  };

  void uuid_create(uuid_t*, uint_t*);

  // this is really dependent on e2fs-devel

  void uuid_generate (unsigned char [16]);

  void uuid_create(uuid_t* a, uint_t* b)
  {
    uuid_generate (&(a->bits[0]));

    *b = 0; // faked return value
  }

#else // e.g. Windows
  struct my_uuid_t {
    unsigned char bits[16];
  };

  void uuid_create(my_uuid_t*, uint_t*)
  {
    logger.error() << "uuid_create() not available on this platform!";
  }
#endif

}

/* static */ const av::UUID av::UUID::nil (uuid_nil);
/* static */ const av::UUID av::UUID::null (uuid_nil);

av::UUID::UUID () :
  mId (av::UUID::nil.mId)
{
#if defined(__linux)
  uuid_t new_uuid;
#else
  my_uuid_t new_uuid;
#endif

  uint_t result;

  ::uuid_create (&new_uuid, &result);

  if (0 != result)
  {
    mId = av::UUID::nil.mId;
    logger.warn() << "UUID: 'uuid_create' returned unsuccessful status of " << result;
  }

  (void) memcpy (&mId, &new_uuid.bits, sizeof (mId));

  if (!valid ())
  {
    mId = av::UUID::nil.mId;
    logger.warn() << "UUID: not a valid UUID: '%s'", new_uuid.bits;
  }

#if defined(AVANGO_DEBUG)
  LOG_TRACE(logger) << "UUID: created '%s'", static_cast<std::string>(*this);
#endif
}

av::UUID::UUID (const av::UUID& a) :
  mId (a.mId)
{
#if defined(AVANGO_DEBUG)
  LOG_TRACE(logger) <<"UUID: copied '%s'", static_cast<std::string>(*this);
#endif
}

av::UUID::UUID (const RawType& a) :
  mId (av::UUID::nil.mId)
{
  (void) memcpy (&mId, &a.bits, sizeof (mId));

  if (!valid ()) {
    mId = av::UUID::nil.mId;
    logger.warn() << "UUID: not a valid UUID: '%s'", a.bits;
  }

#if defined(AVANGO_DEBUG)
  LOG_TRACE(logger) << "UUID: initialized (raw) '%s'", static_cast<std::string>(*this);
#endif
}

av::UUID::UUID (const std::string& a) :
  mId (av::UUID::nil.mId)
{
  if (36 < a.size ()) {
    logger.warn() << "UUID: string '%s' size < 36", a;
    return;
  }

  if ('-' != a[8]) {
    logger.warn() << "UUID: string '%s' not convertible", a;
    return;
  }

  const char *buffer = a.c_str ();

  mId.mTimeLow = string_to_hex (buffer, 8);
  mId.mTimeMid = string_to_hex (buffer + 9, 4);
  mId.mTimeHiAndVersion = string_to_hex (buffer + 14, 4);
  mId.mClockSeqHiAndReserved = string_to_hex (buffer + 19, 2);
  mId.mClockSeqLow = string_to_hex (buffer + 21, 2);
  mId.mNode0 = string_to_hex (buffer + 24, 2);
  mId.mNode1 = string_to_hex (buffer + 26, 2);
  mId.mNode2 = string_to_hex (buffer + 28, 2);
  mId.mNode3 = string_to_hex (buffer + 30, 2);
  mId.mNode4 = string_to_hex (buffer + 32, 2);
  mId.mNode5 = string_to_hex (buffer + 34, 2);

  if (!valid ()) {
    mId = av::UUID::nil.mId;
    logger.warn() << "UUID: not a valid UUID: '%s'", a;
    return;
  }

#if defined(AVANGO_DEBUG)
  LOG_TRACE(logger) << "UUID: initialized (str) '%s'", static_cast<std::string>(*this);
#endif
}

/* virtual */
av::UUID::~UUID ()
{}

av::UUID::operator av::UUID::RawType() const
{
  UUID::RawType result;

  (void) memcpy (&result.bits, &mId, sizeof (result.bits));

  return result;
}

av::UUID::operator std::string() const
{
  const std::string hyphen ("-");

  return std::string (hex_to_string (mId.mTimeLow, 8) + hyphen
                      + hex_to_string (mId.mTimeMid, 4) + hyphen
                      + hex_to_string (mId.mTimeHiAndVersion, 4) + hyphen
                      + hex_to_string (mId.mClockSeqHiAndReserved, 2)
                      + hex_to_string (mId.mClockSeqLow, 2) + hyphen
                      + hex_to_string (mId.mNode0, 2)
                      + hex_to_string (mId.mNode1, 2)
                      + hex_to_string (mId.mNode2, 2)
                      + hex_to_string (mId.mNode3, 2)
                      + hex_to_string (mId.mNode4, 2)
                      + hex_to_string (mId.mNode5, 2));
}

const av::UUID&
av::UUID::operator= (const av::UUID& a)
{
  if (this != &a)
    mId = a.mId;

  return *this;
}

bool
av::operator== (const av::UUID& a, const av::UUID& b)
{
  if ((a.mId.mTimeLow == b.mId.mTimeLow) &&
      (a.mId.mTimeMid == b.mId.mTimeMid) &&
      (a.mId.mTimeHiAndVersion == b.mId.mTimeHiAndVersion) &&
      (a.mId.mClockSeqHiAndReserved == b.mId.mClockSeqHiAndReserved) &&
      (a.mId.mClockSeqLow == b.mId.mClockSeqLow) &&
      (a.mId.mNode0 == b.mId.mNode0) &&
      (a.mId.mNode1 == b.mId.mNode1) &&
      (a.mId.mNode2 == b.mId.mNode2) &&
      (a.mId.mNode3 == b.mId.mNode3) &&
      (a.mId.mNode4 == b.mId.mNode4) &&
      (a.mId.mNode5 == b.mId.mNode5))
    return true;

  return false;
}

bool
av::operator!= (const av::UUID& a, const av::UUID& b)
{
  if ((a.mId.mTimeLow != b.mId.mTimeLow) ||
      (a.mId.mTimeMid != b.mId.mTimeMid) ||
      (a.mId.mTimeHiAndVersion != b.mId.mTimeHiAndVersion) ||
      (a.mId.mClockSeqHiAndReserved != b.mId.mClockSeqHiAndReserved) ||
      (a.mId.mClockSeqLow != b.mId.mClockSeqLow) ||
      (a.mId.mNode0 != b.mId.mNode0) ||
      (a.mId.mNode1 != b.mId.mNode1) ||
      (a.mId.mNode2 != b.mId.mNode2) ||
      (a.mId.mNode3 != b.mId.mNode3) ||
      (a.mId.mNode4 != b.mId.mNode4) ||
      (a.mId.mNode5 != b.mId.mNode5))
    return true;

  return false;
}

bool
av::UUID::valid () const
{
  if ((0x00 == (mId.mClockSeqHiAndReserved & 0x80)) ||
      (0x80 == (mId.mClockSeqHiAndReserved & 0xc0)) ||
      (0xc0 == (mId.mClockSeqHiAndReserved & 0xe0)))
    return true;

  return false;
}

size_t
av::UUID::Hasher::operator() (const av::UUID& a) const
{
  const av::UUID::RawType tmp (static_cast<av::UUID::RawType> (a));

  ssize_t c0 = 0;
  ssize_t c1 = 0;

  for (size_t loop = 0; loop < 16; ++loop) {
    c0 = c0 + tmp.bits[loop];
    c1 = c1 + c0;
  }

  ssize_t x = - c1 % 255;

  if (x < 0) {
    x = x + 255;
  }

  ssize_t y = (c1 - c0) % 255;

  if (y < 0) {
    y = y + 255;
    }

  return (y * 256) + x;
}

// UUID
av::InputStream& av::operator>>(av::InputStream& is, av::UUID& value)
{
  if (is.isBinaryEnabled()) {
    av::UUID::RawType tmp;

    is.read((char*) &tmp, sizeof(tmp));

    value = UUID(tmp);
  } else {
    std::string tmp;

    is >> tmp;

    value = av::UUID(std::string(tmp.c_str()));
  }
  return is;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const av::UUID& value)
{
  if (os.isBinaryEnabled()) {
    av::UUID::RawType tmp (value);

    os.write((const char*) &tmp, sizeof(UUID::RawType));
  } else {
    os << std::string(value);
  }
  return os;
}
