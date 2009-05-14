// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include <iostream>
#include <osg/Matrixf>
#include <avango/Logger.h>
#include <avango/daemon/VRPNClient.h>
#include <avango/daemon/Station.h>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::VRPNClient"));

  void  handle_analog(void *userdata, const vrpn_ANALOGCB a)
  {
    logger.trace() << "handle_analog: processing a VRPN analog event...";
    av::daemon::VRPNClient *device = reinterpret_cast<av::daemon::VRPNClient*>(userdata);
    device->handleAnalog(a);
  }
    
  void  handle_button(void *userdata, const vrpn_BUTTONCB b)
  {
    logger.trace() << "handle_button: processing a VRPN button event..."; 
    av::daemon::VRPNClient *device = reinterpret_cast<av::daemon::VRPNClient*>(userdata);
    device->handleButton(b);
  }
    
  void  handle_tracker(void *userdata, const vrpn_TRACKERCB t)
  {
    logger.trace() << "handle_tracker: processing a VRPN tracker event...";
    av::daemon::VRPNClient *device = reinterpret_cast<av::daemon::VRPNClient*>(userdata);
    device->handleTracker(t);
  }
}

AV_BASE_DEFINE(av::daemon::VRPNClient);

av::daemon::VRPNClient::VRPNClient()
: mStopped(true),
  mVRPNAnalog(0),
  mVRPNButton(0),
  mVRPNTracker(0)

{
  mRequiredFeatures.push_back("server");  
}

av::daemon::VRPNClient::~VRPNClient()
{
  delete mVRPNAnalog;
  delete mVRPNButton;
  delete mVRPNTracker;
}

void
av::daemon::VRPNClient::initClass()
{
  if (!isTypeInitialized())
  {
    av::daemon::Device::initClass();
    AV_BASE_INIT(av::daemon::Device, av::daemon::VRPNClient, true);
  }
}

const std::vector<std::string>&
av::daemon::VRPNClient::queryFeatures()
{
  return mRequiredFeatures;
}

bool
av::daemon::VRPNClient::parseFeatures()
{
  mVRPNServer = queryFeature("server");

  if (mVRPNServer == "") 
  {
    logger.error() << "parseFeatures: required feature 'server' not specified";
    return false;
  }
  return true;
}

/* virtual */ void
av::daemon::VRPNClient::startDevice()
{
  if (!parseFeatures())
  {
    logger.warn() << "startDevice: device not started!";
    return;
  }

  mVRPNAnalog  = new vrpn_Analog_Remote(mVRPNServer.c_str());
  mVRPNButton  = new vrpn_Button_Remote(mVRPNServer.c_str());
  mVRPNTracker = new vrpn_Tracker_Remote(mVRPNServer.c_str());

  mVRPNAnalog->register_change_handler(this, ::handle_analog);
  mVRPNButton->register_change_handler(this, ::handle_button);
  mVRPNTracker->register_change_handler(this, ::handle_tracker);

  mStopped = false;
}

/* virtual */ void
av::daemon::VRPNClient::readLoop()
{
  while(mKeepRunning)
  {
    if (!mStopped) 
    {
      if (mVRPNAnalog) mVRPNAnalog->mainloop();
      if (mVRPNButton) mVRPNButton->mainloop();
      if (mVRPNTracker) mVRPNTracker->mainloop();

      vrpn_SleepMsecs(1);
    }
  }
}

/* virtual */ void
av::daemon::VRPNClient::stopDevice()
{
  mVRPNAnalog->unregister_change_handler(this, ::handle_analog);
  mVRPNButton->unregister_change_handler(this, ::handle_button);
  mVRPNTracker->unregister_change_handler(this, ::handle_tracker);
}

void
av::daemon::VRPNClient::handleAnalog(const vrpn_ANALOGCB a)
{
  NumStationMap::iterator ns; 

  for (ns=mStations.begin(); ns!=mStations.end(); ns++) 
    for (int i=0; i<a.num_channel; i++)
      (*ns).second->setValue(i, a.channel[i]);
}

void
av::daemon::VRPNClient::handleButton(const vrpn_BUTTONCB b)
{
  NumStationMap::iterator ns;

  for (ns=mStations.begin(); ns!=mStations.end(); ns++)
    (*ns).second->setButton(b.button, b.state);
}

void
av::daemon::VRPNClient::handleTracker(const vrpn_TRACKERCB t)
{
  NumStationMap::iterator ns;

  for (ns=mStations.begin(); ns!=mStations.end(); ns++) 
  {
    ::osg::Matrixf pos_mat;
    pos_mat.makeTranslate(t.pos[0], t.pos[1], t.pos[2]);

    ::osg::Matrixf rot_mat(::osg::Quat(t.quat[0],t.quat[1],t.quat[2],t.quat[3]));
    pos_mat.preMult(rot_mat);

    (*ns).second->setMatrix(pos_mat);
  }
}
