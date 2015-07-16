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

#if !defined(AVANGO_MSG_H)
#define AVANGO_MSG_H

#include <string>
#include <vector>
#include <utility>

#include <rpc/rpc.h>

#include <boost/shared_ptr.hpp>

namespace av
{
  class NetNode;

  /**
   * Serialization facility for Avango distribution.
   * This class is a smart handler for the underlying memory buffer.
   */
  class Msg
  {

  public:

    enum MsgType
    {
      relative,    // push only changes
      absolute     // push the complete object
    };

    Msg();
    ~Msg();

    // not implemented, compiler does the right thing
    // copy construction copies the object, but not the buffer
    // Msg(const Msg& msg);
    // Msg& operator=(const Msg& msg);

    /// copies Msg object and buffer. Cloned object has the same
    /// content as this object, but seperate buffer
    Msg clone() const;

    void setType(MsgType mt) { mMsgType = mt; }
    MsgType getType() const { return mMsgType; }

    /// get byte size of message
    size_t getSize();
    /// resize message. newSize can be bigger or smaller than old size
    void   resize(size_t newSize);


    /// remove \e bytes from message and put them into \e buffer
    void pop(void* buffer, size_t bytes);
    /// add raw memory buffer of size \e bytes to end of message
    void push(const void* buffer, size_t bytes);

    /// get raw message buffer pointer
    unsigned char* getBuffer();

    /// get NetNode that this message belongs to. Can be 0 is message is not associated with a NetNode yet.
    NetNode* getNetNode() const;
    /// set NetNode that this message belongs to.
    void setNetNode(NetNode* netNode);

  private:
    using MessageBuffer = std::vector<unsigned char>;

    Msg(MsgType newMsgType, boost::shared_ptr<MessageBuffer> newBuffer);

    MsgType       mMsgType;

    boost::shared_ptr<MessageBuffer> mMsgBuffer;
    NetNode*                         mNetNode;
  };


  class XDRHandle {
  public:
    XDRHandle(void* buffer, size_t bufferSize, xdr_op direction)
    { xdrmem_create(&mXDR, (caddr_t)buffer, bufferSize, direction); }
    ~XDRHandle()
    { xdr_destroy(&mXDR); }

    XDR* getXDR() { return &mXDR; }

  private:
    XDRHandle(const XDRHandle&);
    XDRHandle& operator=(const XDRHandle&);

    XDR mXDR;
  };

  class Field;
  class Distributed;
  class NetID;
  class AnyLink;
  class Type;
  class Field;

  // bool
  void av_pushMsg(Msg& msg, const bool& buf);
  void av_popMsg(Msg& msg, bool& buf);

  // int32_t
  void av_pushMsg(Msg& msg, const int32_t& buf);
  void av_popMsg(Msg& msg, int32_t& buf);

  // std::vector<int32_t>
  void av_pushMsg(Msg& msg, const std::vector<int32_t>& buf);
  void av_popMsg(Msg& msg, std::vector<int32_t>& buf);

  // uint32_t
  void av_pushMsg(Msg& msg, const uint32_t& buf);
  void av_popMsg(Msg& msg, uint32_t& buf);

  // std::vector<uint32_t>
  void av_pushMsg(Msg& msg, const std::vector<uint32_t>& buf);
  void av_popMsg(Msg& msg, std::vector<uint32_t>& buf);

  // int64_t
  void av_pushMsg(Msg& msg, const int64_t& buf);
  void av_popMsg(Msg& msg, int64_t& buf);

  // std::vector<int64_t>
  void av_pushMsg(Msg& msg, const std::vector<int64_t>& buf);
  void av_popMsg(Msg& msg, std::vector<int64_t>& buf);

  // uint64_t
  void av_pushMsg(Msg& msg, const uint64_t& buf);
  void av_popMsg(Msg& msg, uint64_t& buf);

  // std::vector<uint64_t>
  void av_pushMsg(Msg& msg, const std::vector<uint64_t>& buf);
  void av_popMsg(Msg& msg, std::vector<uint64_t>& buf);

  // float
  void av_pushMsg(Msg& msg, const float& buf);
  void av_popMsg(Msg& msg, float& buf);

  // std::vector<float>
  void av_pushMsg(Msg& msg, const std::vector<float>& buf);
  void av_popMsg(Msg& msg, std::vector<float>& buf);

  // double
  void av_pushMsg(Msg& msg, const double& buf);
  void av_popMsg(Msg& msg, double& buf);

  // std::vector<double>
  void av_pushMsg(Msg& msg, const std::vector<double>& buf);
  void av_popMsg(Msg& msg, std::vector<double>& buf);

  // AnyLink
  void av_pushMsg(Msg& msg, const AnyLink& buf);
  void av_popMsg(Msg& msg, AnyLink& buf);

  // Type
  void av_pushMsg(Msg& msg, const Type& buf);
  void av_popMsg(Msg& msg, Type& buf);

  // Field
  void av_pushMsg(Msg& msg, Field* buf);
  void av_popMsg(Msg& msg, Field* buf);

  // Distributed
  void av_pushMsg(Msg& msg, Distributed* buf);
  void av_popMsg(Msg& msg, Distributed* buf);

  // std::string
  void av_pushMsg(Msg& msg, const std::string& buf);
  void av_popMsg(Msg& msg, std::string& buf);

  // NetID
  void av_pushMsg(Msg& msg, const NetID& buf);
  void av_popMsg(Msg& msg, NetID& buf);

  template <typename T1, typename T2>
    void av_pushMsg(Msg& msg, const std::pair<T1, T2>& val)
  {
    av_pushMsg(msg, val.first);
    av_pushMsg(msg, val.second);
  }

  template <typename T1, typename T2>
  void av_popMsg(Msg& msg, std::pair<T1, T2>& val)
  {
    av_popMsg(msg, val.second);
    av_popMsg(msg, val.first);
  }

  template <typename T>
  void av_pushMsg(Msg& msg, const std::vector<T>& val)
  {
    typename std::vector<T>::const_iterator it(val.begin());

    while (it != val.end())
      {
      av_pushMsg(msg, *it);
      ++it;

    }
    av_pushMsg(msg, val.size());
  }

  template <typename T>
  void av_popMsg(Msg& msg, std::vector<T>& val)
  {
    int count;

    av_popMsg(msg, count);

    std::vector<T> swapVec(count);

    val.swap(swapVec);

    T tmp;

    for (int i = count; i > 0; --i)
    {
      av_popMsg(msg, tmp);
      val[i-1] = tmp;
    }
  }

} // namespace av

#endif // #if !defined(AVANGO_NETMSG_H)
