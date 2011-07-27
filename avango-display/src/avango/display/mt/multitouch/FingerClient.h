#ifndef _FINGERCLIENT_H
#define _FINGERCLIENT_H

#include <boost/thread.hpp>
#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <map>
#include <set>
#include <queue>

#include "ip/UdpSocket.h"
#include "ip/PacketListener.h"

#include "osc/OscReceivedElements.h"

#include "FingerListener.h"


class FingerClient : public PacketListener
{
public:
  enum MyMessage
  {
	  MYMESS_SET_PORT = 0,
	  MYMESS_ENABLE_CALIB
  };

public:
  FingerClient(int port);
  ~FingerClient();

  void registerFingerListener(FingerListener *listener);
  void registerUserListener(UserListener *listener);

  void setPositionHistorySize(unsigned int size);

  void ReceiveThread();
  void ProcessPacket(const char *data, int size, const IpEndpointName &remoteEndpoint);

  void ProcessBundle(const osc::ReceivedBundle &bundle, const IpEndpointName &remoteEndpoint);
  void ProcessMessage(const osc::ReceivedMessage &message, const IpEndpointName &remoteEndpoint);

  int getPort() const { return mPort; }
  std::queue< std::pair< MyMessage, int > > &getMessages() { return mMessages; }

private:
  class ThreadCaller
  {
  public:
    ThreadCaller(FingerClient *fingerclient) : fingerclient(fingerclient) {}
    void operator()() { fingerclient->ReceiveThread(); }
  private:
    FingerClient *fingerclient;
  };

  osc::int32 getUserIdForFinger(osc::int32 s_id);
  void addUserIdToFinger(osc::int32 u_id, osc::int32 s_id);
  void removeUserIdFromFinger(osc::int32 s_id);
  void ProcessKinectMessage(const osc::ReceivedMessage &message, const IpEndpointName &remoteEndpoint);

  boost::thread *thread;
  boost::mutex mutex, mutex2;

  FingerListener *fingerlistener;
  UserListener *userlistener;
  unsigned int historysize;
  UdpListeningReceiveSocket *socket;
  std::map<osc::int32, FingerInfo*> cursor_id_map;
  std::map<osc::int32, UserInfo*> uid_map;
  std::set<osc::int32> alive_ids, alive_uids;
  std::vector< osc::int32 > current_fseqs;
  std::vector< std::string > address_patterns;
  int mPort;
  std::queue< std::pair< MyMessage, int > > mMessages;
};


#endif //_FINGERCLIENT_H
