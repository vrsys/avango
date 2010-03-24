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

#include "ip/UdpSocket.h"
#include "ip/PacketListener.h"

#include "osc/OscReceivedElements.h"

#include "FingerListener.h"


class FingerClient : public PacketListener
{
public:
  FingerClient(int port);
  ~FingerClient();

  void registerFingerListener(FingerListener *listener);

  void setPositionHistorySize(unsigned int size);

  void ReceiveThread();
  void ProcessPacket(const char *data, int size, const IpEndpointName &remoteEndpoint);

  void ProcessBundle(const osc::ReceivedBundle &bundle, const IpEndpointName &remoteEndpoint);
  void ProcessMessage(const osc::ReceivedMessage &message, const IpEndpointName &remoteEndpoint);

private:
  class ThreadCaller
  {
  public:
    ThreadCaller(FingerClient *fingerclient) : fingerclient(fingerclient) {}
    void operator()() { fingerclient->ReceiveThread(); }
  private:
    FingerClient *fingerclient;
  };

  boost::thread *thread;
  boost::mutex mutex;

  FingerListener *fingerlistener;
  unsigned int historysize;
  UdpListeningReceiveSocket *socket;
  std::map<osc::int32, FingerInfo*> cursor_id_map;
  std::set<osc::int32> alive_ids;
  osc::int32 current_fseq;
};


#endif //_FINGERCLIENT_H
