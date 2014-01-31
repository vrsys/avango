
#include <string.h>
#include <iostream>
#include <algorithm>

#include "FingerClient.h"

#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::devices::FingerClient"));
}


FingerClient::FingerClient(int port) : fingerlistener(0), historysize(0), mPort(port)
{
  address_patterns.push_back("/tuio/2Dcur");
  address_patterns.push_back("/tuio/2Dobj");
  address_patterns.push_back("/tuio/2Dblb");
  address_patterns.push_back("/tuio/_kinect");
  current_fseqs.resize(address_patterns.size(), 0);

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

void FingerClient::registerUserListener(UserListener *listener)
{
  boost::mutex::scoped_lock lock(mutex);
  userlistener = listener;
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
  osc::int32 fseq = 0;
  int pattern = -1;

  osc::ReceivedBundle::const_iterator iter = bundle.ElementsBegin();
  osc::ReceivedBundle::const_iterator iter_end = bundle.ElementsEnd();

  //std::cout << "Received Bundle" << std::endl;
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

		{
			osc::ReceivedMessageArgumentStream args2 = message.ArgumentStream();
			//std::cout << message.AddressPattern() << " ";

			if (!args2.Eos())
			{
				const char *cmd;
				args2 >> cmd;
				//std::cout << cmd;
			}
			//args2 >> osc::EndMessage;
		}
		//std::cout << std::endl;

		for(unsigned int i = 0; i < address_patterns.size(); ++i)
		{
			if (strcmp(message.AddressPattern(), address_patterns[i].data()) == 0)
			{
			  pattern = (int)i;
			  const char *cmd;
			  args >> cmd;

			  if (strcmp(cmd, "fseq") == 0)
				args >> fseq >> osc::EndMessage;
			}
		}
      }
      catch (osc::Exception &e)
      {
        logger.error() << "Error while parsing TUIO message: " << e.what();
      }
    }
  }

  if (pattern >= 0)
  {
	  // if fseq is negative, then reset current_fseq to 0
	  if (fseq < 0)
		current_fseqs[pattern] = fseq = 0;

	  if (fseq >= current_fseqs[pattern])
	  {
		current_fseqs[pattern] = fseq;

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
}

osc::int32
FingerClient::getUserIdForFinger(osc::int32 s_id)
{
	for(std::map<osc::int32, UserInfo*>::iterator it = uid_map.begin(); it != uid_map.end(); ++it)
	{
		std::set< int >::iterator itID = it->second->FingerIds().find(s_id);
		if(itID != it->second->FingerIds().end())
			return *itID;
	}
	return -1;
}

void
FingerClient::addUserIdToFinger(osc::int32 u_id, osc::int32 s_id)
{
  std::map<osc::int32, FingerInfo*>::iterator iter = cursor_id_map.find(s_id);
  if (iter != cursor_id_map.end())
  {
    FingerInfo *fingerinfo = iter->second;

	/*if(fingerinfo->getUserId() != u_id)
		std::cout << "Finger " << s_id << " switch from user " << fingerinfo->getUserId() << " to " << u_id << std::endl;*/

	fingerinfo->setUserId(u_id);
      {
		FingerListener *fingerlistener_secure;
		{
			boost::mutex::scoped_lock lock(mutex);
			fingerlistener_secure = fingerlistener;
		}
		if (fingerlistener_secure)
			fingerlistener_secure->fingerMoved(fingerinfo);
      }
  }
}

void
FingerClient::removeUserIdFromFinger(osc::int32 s_id)
{
  std::map<osc::int32, FingerInfo*>::iterator iter = cursor_id_map.find(s_id);
  if (iter != cursor_id_map.end())
  {
    FingerInfo *fingerinfo = iter->second;
	/*if(fingerinfo->getUserId() != -1)
		std::cout << "Finger " << s_id << " switch from user " << fingerinfo->getUserId() << " to " << -1 << std::endl;*/
	fingerinfo->setUserId(-1);

      {
        FingerListener *fingerlistener_secure;
		{
			boost::mutex::scoped_lock lock(mutex);
			fingerlistener_secure = fingerlistener;
		}
		if (fingerlistener_secure)
			fingerlistener_secure->fingerMoved(fingerinfo);
      }
  }
}
void
FingerClient::ProcessKinectMessage(const osc::ReceivedMessage &message, const IpEndpointName &remoteEndpoint)
{
	osc::ReceivedMessageArgumentStream args = message.ArgumentStream();

	const char *cmd;
	args >> cmd;

	//std::cout << message.AddressPattern();

	if (strcmp(cmd, "set") == 0)
    {
	  //std::cout << " set";
	  static osc::int32 u_id;
	  std::set< int > finger_ids;

	  args >> u_id;
	  //std::cout << " U" << u_id;

	  osc::int32 s_id;
      while (!args.Eos()) {
        args >> s_id;
		//std::cout << " F" << s_id;
        finger_ids.insert(s_id);
      }
      args >> osc::EndMessage;
	  //std::cout << std::endl;

      std::map<osc::int32, UserInfo*>::iterator iter = uid_map.find(u_id);
      if (iter == uid_map.end())
      {
        UserInfo *userinfo = new UserInfo(u_id);
        if (userinfo)
        {
		  userinfo->FingerIds() = finger_ids;
		  for(std::set< int >::iterator it = finger_ids.begin(), end = finger_ids.end(); it != end; ++it)
			addUserIdToFinger(u_id, *it);

          uid_map[u_id] = userinfo;
          alive_uids.insert(u_id);

		  UserListener *userlistener_secure;
		  {
			  boost::mutex::scoped_lock lock(mutex);
			  userlistener_secure = userlistener;
		  }
		  if (userlistener_secure)
			  userlistener_secure->userAdded(userinfo);
        }
      }
      else
      {
		int changed = false;
        UserInfo *userinfo = iter->second;
        
		std::set< int > &fIds = userinfo->FingerIds();

		// add new fingers
		for(std::set< int >::iterator it = finger_ids.begin(), end = finger_ids.end(); it != end; ++it)
		{
			if(fIds.insert(*it).second)
			{
				addUserIdToFinger(u_id, *it);
				//std::cout << "User ID " << u_id << " added to finger " << *it << std::endl;
				changed = true;
			}
		}
		// remove left fingers
		if(fIds.size() > finger_ids.size())
		{
			std::vector< int > remove_fids;
			for(std::set< int >::iterator it = fIds.begin(), end = fIds.end(); it != end; ++it)
			{
				if(finger_ids.find(*it) == finger_ids.end())
				{
					removeUserIdFromFinger(*it);
					remove_fids.push_back(*it);
				}
			}
			for(unsigned int i = 0; i < remove_fids.size(); ++i)
				fIds.erase(remove_fids[i]);
			changed = true;
		}

		if(changed)
        {
			UserListener *userlistener_secure;
			  {
				  boost::mutex::scoped_lock lock(mutex);
				  userlistener_secure = userlistener;
			  }
			  if (userlistener_secure)
				  userlistener_secure->userMoved(userinfo);
        }
      }
    }
	else if (strcmp(cmd, "alive") == 0)
    {
	  //std::cout << " alive";
      std::set<osc::int32> removed_uids = alive_uids;

      osc::int32 u_id;
      while (!args.Eos()) {
        args >> u_id;
		//std::cout << " " << u_id;
        removed_uids.erase(u_id);
      }
      args >> osc::EndMessage;
	  //std::cout << std::endl;

	  UserListener *userlistener_secure;
	  {
		  boost::mutex::scoped_lock lock(mutex);
		  userlistener_secure = userlistener;
	  }

      std::set<osc::int32>::const_iterator iter = removed_uids.begin();
      std::set<osc::int32>::const_iterator iter_end = removed_uids.end();
      for (; iter != iter_end; ++iter)
      {
        u_id = *iter;
        UserInfo *userinfo = uid_map[u_id];
        if (userlistener_secure)
		  userlistener_secure->userRemoved(userinfo);
        uid_map.erase(u_id);
        alive_uids.erase(u_id);
        delete userinfo;
      }
    }
}

void
FingerClient::ProcessMessage(const osc::ReceivedMessage &message, const IpEndpointName &remoteEndpoint)
{
	boost::mutex::scoped_lock lock(mutex2);

	//std::cout << message.AddressPattern();
	if(strcmp(message.AddressPattern(), "/tuio/_kinect") == 0)
	{
		ProcessKinectMessage(message, remoteEndpoint);
		return;
	}


	bool isBlob = false, isObject = false, isValid = true;
	if(strcmp(message.AddressPattern(), "/tuio/2Dobj") == 0)
		isObject = true;
	else if(strcmp(message.AddressPattern(), "/tuio/2Dblb") == 0)
		isBlob = true;
	else if (strcmp(message.AddressPattern(), "/tuio/2Dcur") != 0)
		isValid = false;

  if(isValid)
  {
    osc::ReceivedMessageArgumentStream args = message.ArgumentStream();

    const char *cmd;
    args >> cmd;

    if (strcmp(cmd, "set") == 0)
    {
	  static osc::int32 s_id, i_id;
      static float xpos, ypos, xspeed, yspeed, maccel;
	  static float angle, width, height, area, rotSpeed, rotAccel;

	  if(isBlob)
		  args >> s_id >> xpos >> ypos >> angle >> width >> height >> area >> xspeed >> yspeed >> rotSpeed >> maccel >> rotAccel >> osc::EndMessage;
	  else if(isObject)
		  args >> s_id >> i_id >> xpos >> ypos >> angle >> xspeed >> yspeed >> rotSpeed >> maccel >> rotAccel >> osc::EndMessage;
	  else
		  args >> s_id >> xpos >> ypos >> xspeed >> yspeed >> maccel >> osc::EndMessage;

      std::map<osc::int32, FingerInfo*>::iterator iter = cursor_id_map.find(s_id);
      if (iter == cursor_id_map.end())
      {
        FingerInfo *fingerinfo = new FingerInfo(s_id, Vec2(xpos, ypos));
        if (fingerinfo)
        {
          {
            fingerinfo->setPositionHistorySize(historysize);
			if(isBlob)
			{
				fingerinfo->setAngle(angle);
				fingerinfo->setWidthHeightArea(width, height, area);
			}
			else if(isObject)
				fingerinfo->setAngle(angle);
          }
		  fingerinfo->setUserId(getUserIdForFinger(s_id));
		  //std::cout << "New finger " << s_id << " from user " << fingerinfo->getUserId() << std::endl;
          cursor_id_map[s_id] = fingerinfo;
          alive_ids.insert(s_id);
		  FingerListener *fingerlistener_secure;
			{
				boost::mutex::scoped_lock lock(mutex);
				fingerlistener_secure = fingerlistener;
			}
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
          {
            fingerinfo->setPositionHistorySize(historysize);
			if(isBlob)
			{
				fingerinfo->setAngle(angle);
				fingerinfo->setWidthHeightArea(width, height, area);
			}
			else if(isObject)
				fingerinfo->setAngle(angle);
          }
          fingerinfo->addPosition(pos);
		  FingerListener *fingerlistener_secure;
			{
				boost::mutex::scoped_lock lock(mutex);
				fingerlistener_secure = fingerlistener;
			}
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