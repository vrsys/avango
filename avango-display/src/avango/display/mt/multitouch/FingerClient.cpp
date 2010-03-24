
#include <string.h>
#include <iostream>
#include <algorithm>

#include "FingerClient.h"

#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::devices::FingerClient"));
}


FingerClient::FingerClient(int port) : fingerlistener(0), historysize(0), current_fseq(0)
{
  socket = 0;
  try
  {
    socket = new UdpListeningReceiveSocket(IpEndpointName(IpEndpointName::ANY_ADDRESS, port), this);
    if (socket->IsBound())
    {
      logger.info() << "Listening to TUIO messages on UDP port " << port << ".";
      thread = new boost::thread(ThreadCaller(this));
    }
    else
    {
      delete socket;
      socket = 0;
    }
  }
  catch (std::exception &e)
  {
    logger.error() << "Could not bind to UDP port " << port << ".";
  }
}

FingerClient::~FingerClient()
{
  if (socket)
    socket->AsynchronousBreak();
  if (thread) {
    thread->join();
    delete thread;
  }
  if (socket)
    delete socket;
}

void FingerClient::registerFingerListener(FingerListener *listener)
{
  boost::mutex::scoped_lock lock(mutex);
  fingerlistener = listener;
}

void
FingerClient::setPositionHistorySize(unsigned int size)
{
  boost::mutex::scoped_lock lock(mutex);
  historysize = size;
}

void
FingerClient::ReceiveThread()
{
  socket->Run();
}

void
FingerClient::ProcessPacket(const char *data, int size, const IpEndpointName &remoteEndpoint)
{
  osc::ReceivedPacket packet(data, size);
  if (packet.IsBundle())
    ProcessBundle(osc::ReceivedBundle(packet), remoteEndpoint);
  else
    ProcessMessage(osc::ReceivedMessage(packet), remoteEndpoint);
}

void
FingerClient::ProcessBundle(const osc::ReceivedBundle &bundle, const IpEndpointName &remoteEndpoint)
{
  osc::int32 fseq = current_fseq;

  osc::ReceivedBundle::const_iterator iter = bundle.ElementsBegin();
  osc::ReceivedBundle::const_iterator iter_end = bundle.ElementsEnd();

  // find fseq
  for (; iter != iter_end; ++iter)
  {
    osc::ReceivedBundleElement element = *iter;
    if (!iter->IsBundle())
    {
      try
      {
        const osc::ReceivedMessage &message = osc::ReceivedMessage(element);
        osc::ReceivedMessageArgumentStream args = message.ArgumentStream();

        if (strcmp(message.AddressPattern(), "/tuio/2Dcur") == 0)
        {
          const char *cmd;
          args >> cmd;

          if (strcmp(cmd, "fseq") == 0)
            args >> fseq >> osc::EndMessage;
        }
      }
      catch (osc::Exception &e)
      {
        logger.error() << "Error while parsing TUIO message: " << e.what();
      }
    }
  }

  // if fseq is negative, then reset current_fseq to 0
  if (fseq < 0)
    current_fseq = fseq = 0;

  if (fseq >= current_fseq)
  {
    current_fseq = fseq;

    // process other messages
    iter = bundle.ElementsBegin();
    for (; iter != iter_end; ++iter)
    {
      osc::ReceivedBundleElement element = *iter;
      if (iter->IsBundle())
        ProcessBundle(osc::ReceivedBundle(element), remoteEndpoint);
      else
      {
        try
        {
          ProcessMessage(osc::ReceivedMessage(element), remoteEndpoint);
        }
        catch (osc::Exception &e)
        {
          logger.error() << "Error while parsing TUIO message: " << e.what();
        }
      }
    }
  }
}

void
FingerClient::ProcessMessage(const osc::ReceivedMessage &message, const IpEndpointName &remoteEndpoint)
{
  if (strcmp(message.AddressPattern(), "/tuio/2Dcur") == 0)
  {
    osc::ReceivedMessageArgumentStream args = message.ArgumentStream();

    const char *cmd;
    args >> cmd;

    if (strcmp(cmd, "set") == 0)
    {
      osc::int32 s_id;
      float xpos, ypos, xspeed, yspeed, maccel;

      args >> s_id >> xpos >> ypos >> xspeed >> yspeed >> maccel >> osc::EndMessage;

      std::map<osc::int32, FingerInfo*>::iterator iter = cursor_id_map.find(s_id);
      if (iter == cursor_id_map.end())
      {
        FingerInfo *fingerinfo = new FingerInfo(s_id, Vec2(xpos, ypos));
        if (fingerinfo)
        {
          FingerListener *fingerlistener_secure;
          {
            boost::mutex::scoped_lock lock(mutex);
            fingerlistener_secure = fingerlistener;
            fingerinfo->setPositionHistorySize(historysize);
          }
          cursor_id_map[s_id] = fingerinfo;
          alive_ids.insert(s_id);
          if (fingerlistener_secure)
            fingerlistener_secure->fingerAdded(fingerinfo);
        }
      }
      else
      {
        FingerInfo *fingerinfo = iter->second;
        Vec2 pos(xpos, ypos);
        if (pos != fingerinfo->getPosition(0))
        {
          FingerListener *fingerlistener_secure;
          {
            boost::mutex::scoped_lock lock(mutex);
            fingerlistener_secure = fingerlistener;
            fingerinfo->setPositionHistorySize(historysize);
          }
          fingerinfo->addPosition(pos);
          if (fingerlistener_secure)
            fingerlistener_secure->fingerMoved(fingerinfo);
        }
      }
    }
    else if (strcmp(cmd, "alive") == 0)
    {
      std::set<osc::int32> removed_ids = alive_ids;

      osc::int32 s_id;
      while (!args.Eos()) {
        args >> s_id;
        removed_ids.erase(s_id);
      }
      args >> osc::EndMessage;

      FingerListener *fingerlistener_secure;
      {
        boost::mutex::scoped_lock lock(mutex);
        fingerlistener_secure = fingerlistener;
      }

      std::set<osc::int32>::const_iterator iter = removed_ids.begin();
      std::set<osc::int32>::const_iterator iter_end = removed_ids.end();
      for (; iter != iter_end; ++iter)
      {
        const osc::int32 s_id = *iter;
        FingerInfo *fingerinfo = cursor_id_map[s_id];
        if (fingerlistener_secure)
          fingerlistener_secure->fingerRemoved(fingerinfo);
        cursor_id_map.erase(s_id);
        alive_ids.erase(s_id);
        delete fingerinfo;
      }
    }
  }
}
