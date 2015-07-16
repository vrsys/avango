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

#include <avango/Type.h>
#include <avango/daemon/Config.h>
#include <avango/daemon/DeviceDaemon.h>
#include <avango/daemon/DeviceSensor.h>
#include <avango/daemon/DeviceService.h>
#include <avango/daemon/DeviceActuator.h>
#include <avango/daemon/DTrack.h>
#include <avango/daemon/HIDInput.h>
#include <avango/daemon/TUIOInput.h>
#include <avango/daemon/Init.h>
#include <avango/daemon/StationSegment.h>

#ifndef WIN32
#  include <avango/daemon/WacomTablet.h>
#  include <avango/daemon/Wiimote.h>
#  include <avango/daemon/WiimoteActuator.h>
#endif

#include <avango/python/register_field.h>
#include <boost/python.hpp>
#include <boost/mpl/vector.hpp>

#ifdef VRPN_SUPPORT
#  include <avango/daemon/VRPNClient.h>
#endif

#ifdef AVANGO_OCULUS_RIFT_SUPPORT
#include <avango/daemon/Oculus.h>
#endif

using namespace boost::python;
using namespace av::python;

namespace boost
{
  namespace python
  {
    template <class T> struct pointee<av::Link<T> >
    {
      using type = T;
    };
  }
}

namespace
{
  boost::shared_ptr<av::daemon::StationSegment> mStationSegment(new av::daemon::StationSegment());

  bool
  doesTypeExist(std::string name)
  {
    return (av::Type::getByName(name) != av::Type::badType())? true : false;
  }

  void
  addStation(av::daemon::Device* self, int number, std::string name)
  {
    if (!mStationSegment)
      throw std::runtime_error("No StationSegment available.");

    self->addStation(number, mStationSegment->getStation(name));
  }

  // wrapper for specialized queryFeature calls, required by .add_property
  // (a better solution would be to use boost::bind, but unfortunately
  // combining boost::bind with boost::python doesn't work in this case)
  std::string getTTYFeature(av::daemon::Device* self) { return self->queryFeature("tty"); }
  std::string getNormAbsFeature(av::daemon::Device* self) { return self->queryFeature("norm-abs"); }
  std::string getAccumRelFeature(av::daemon::Device* self) { return self->queryFeature("accum-rel-events"); }
  std::string getResetRelCycleFeature(av::daemon::Device* self) { return self->queryFeature("reset-rel-values-cycle"); }
  std::string getTimeoutFeature(av::daemon::Device* self) { return self->queryFeature("timeout"); }
  std::string getPortFeature(av::daemon::Device* self) { return self->queryFeature("port"); }
  std::string getToggleResetFeature(av::daemon::Device* self) { return self->queryFeature("toggle-reset"); }
  std::string getServerFeature(av::daemon::Device* self) { return self->queryFeature("server"); }

  // wrapper for specialized configureFeature calls, required by .add_property
  std::string parseBoolString(std::string value)
  {
    if ((value == "true")||(value=="TRUE")||(value=="True")) return "1";
    if ((value == "false")||(value=="FALSE")||(value=="False")) return "0";
    return value;
  }
  void setTTYFeature(av::daemon::Device* self, std::string value) { self->configureFeature("tty", value); }
  void setNormAbsFeature(av::daemon::Device* self, std::string value) { self->configureFeature("norm-abs",parseBoolString(value)); }
  //void setNormAbsFeature(av::daemon::Device* self, bool value) { (value)?self->configureFeature("norm-abs", "1"):self->configureFeature("norm-abs", "0"); }
  void setAccumRelFeature(av::daemon::Device* self, std::string value) { self->configureFeature("accum-rel-events", parseBoolString(value)); }
  void setResetRelCycleFeature(av::daemon::Device* self, std::string value) { self->configureFeature("reset-rel-values-cycle", value); }
  void setTimeoutFeature(av::daemon::Device* self, std::string value) { self->configureFeature("timeout", value); }
  void setPortFeature(av::daemon::Device* self, std::string value) { self->configureFeature("port", value); }
  void setToggleResetFeature(av::daemon::Device* self, std::string value) { self->configureFeature("toggle-reset", parseBoolString(value)); }
  void setServerFeature(av::daemon::Device* self, std::string value) { self->configureFeature("server", value); }

  // set LED states
  void setLED(av::daemon::HIDInput* self, int number, int value)
  {
    mStationSegment->getStation(self->getFirstStation())->setLED(number,value);
  }
  void setAllLEDs(av::daemon::HIDInput* self, int value)
  {
    av::daemon::Station* st = mStationSegment->getStation(self->getFirstStation());
    for (int i=0; i<av::daemon::Station::sMaxLeds; ++i)
      st->setLED(i, value);
  }

  // wrapper for mappings
  void
  mapToStationButton(av::daemon::HIDInput* self, int number, std::string event)
  {
    std::string feature = self->getFirstStation() + "::STATION_BUTTON::" + boost::lexical_cast<std::string>(number);
    self->configureFeature(feature, event);
  }
  void
  mapToStationValue(av::daemon::HIDInput* self, int number, std::string event)
  {
    std::string feature = self->getFirstStation() + "::STATION_VALUE::" + boost::lexical_cast<std::string>(number);
    self->configureFeature(feature, event);
  }
  void
  mapToStationLED(av::daemon::HIDInput* self, int number, std::string event)
  {
    std::string feature = self->getFirstStation() + "::STATION_LED::" + boost::lexical_cast<std::string>(number);
    self->configureFeature(feature, event);
  }

  void
  run(object devices)
  {
    av::Link<av::daemon::DeviceDaemon> daemon(new av::daemon::DeviceDaemon());
    if(!daemon.isValid())
      throw std::runtime_error("No instance of a DeviceDaemon available.");

    for (int i = 0; i != extract<int>(devices.attr("__len__")()); ++i)
    {
      daemon->appendDevice(extract<av::daemon::Device*>(devices[i]));
    }

    daemon->run();
  }
}

BOOST_PYTHON_MODULE(_daemon)
{
  // initialize Avango Daemon
  av::daemon::Init::initClass();

  // Avango NG application instances
  class_<av::daemon::DeviceSensor, av::Link<av::daemon::DeviceSensor>, bases<av::FieldContainer>, boost::noncopyable >("DeviceSensor", "docstring", no_init);
  class_<av::daemon::DeviceActuator, av::Link<av::daemon::DeviceActuator>, bases<av::FieldContainer>, boost::noncopyable >("_DeviceActuator", "docstring", no_init);

#ifndef WIN32
  class_<av::daemon::WiimoteActuator, av::Link<av::daemon::WiimoteActuator>, bases<av::daemon::DeviceActuator>, boost::noncopyable >("WiimoteActuator", "docstring", no_init)
    .def("reset", &av::daemon::WiimoteActuator::reset)
    ;
#endif

  class_<av::daemon::DeviceService, av::Link<av::daemon::DeviceService>, bases<av::Base>, boost::noncopyable >("DeviceService", "docstring")
    .def("set_led", &av::daemon::DeviceService::setLED)
    ;

  register_field<av::daemon::SFDeviceService>("SFDeviceService");
  register_multifield<av::daemon::MFDeviceService>("MFDeviceService");

  // base classes (this classes cannot be instanciated from Python)
  class_<av::daemon::Device, av::Link<av::daemon::Device>, bases<av::Base>, boost::noncopyable >("_Device",
    "Avango Daemon base class for devices", no_init)
    .def("add_station", &::addStation)
    ;
  class_<av::daemon::Station, av::Link<av::daemon::Station>, bases<av::Base>, boost::noncopyable >("_DeviceStation",
    "An Avango Daemon station", no_init);


  // Avango NG device: HIDInput
  class_<av::daemon::HIDInput, av::Link<av::daemon::HIDInput>, bases<av::daemon::Device>, boost::noncopyable >("_HIDHelper",
    "A helper class that provides some basic properties as well as some functions to configure a HIDInput device.")
    .add_property("device", &::getTTYFeature, &::setTTYFeature)
    .add_property("norm_abs", &::getNormAbsFeature, &::setNormAbsFeature)
    .add_property("accum_rel_events", &::getAccumRelFeature, &::setAccumRelFeature)
    .add_property("reset_rel_values_cycle", &::getResetRelCycleFeature, &::setResetRelCycleFeature)
    .add_property("timeout", &::getTimeoutFeature, &::setTimeoutFeature)
    .def("map_to_station_button", &::mapToStationButton)
    .def("map_to_station_value", &::mapToStationValue)
    .def("map_to_station_led", &::mapToStationLED)
    .def("set_led", &::setLED)
    .def("set_leds", &::setAllLEDs)
    ;
#ifndef WIN32
  // Avango NG device: WacomTablet (derived from HIDInput)
  class_<av::daemon::WacomTablet, av::Link<av::daemon::WacomTablet>, bases<av::daemon::HIDInput>, boost::noncopyable >("_WacomTabletHelper",
    "A helper class that provides some basic properties and functions inherited from HIDInput.")
    .add_property("toggle_reset", &::getToggleResetFeature, &::setToggleResetFeature)
    ;

  // Avango NG device: Wiimote (derived from HIDInput)
  class_<av::daemon::Wiimote, av::Link<av::daemon::Wiimote>, bases<av::daemon::HIDInput>, boost::noncopyable >("_WiimoteHelper",
    "A helper class that provides some basic properties and functions inherited from HIDInput.")
    ;
#endif

  // Avango NG device: DTrack
  class_<av::daemon::DTrack, av::Link<av::daemon::DTrack>, bases<av::daemon::Device>, boost::noncopyable >("_DTrackHelper",
    "A helper class that provides some basic properties and function inherited from DTrack,"
    "used to construct a concrete Python device representation.")
    .add_property("port", &::getPortFeature, &::setPortFeature)
    ;

  class_<av::daemon::TUIOInput, av::Link<av::daemon::TUIOInput>, bases<av::daemon::Device>, boost::noncopyable >("_TUIOInputHelper",
    "A helper class that provides some basic properties and function inherited from TUIOInput,"
    "used to construct a concrete Python device representation.")
    .add_property("port", &::getPortFeature, &::setPortFeature)
    ;

#ifdef VRPN_SUPPORT
  // Avango NG device: VRPNClient
  class_<av::daemon::VRPNClient, av::Link<av::daemon::VRPNClient>, bases<av::daemon::Device>, boost::noncopyable >("_VRPNClientHelper",
    "A helper class used to construct a concrete Python VRPN client device representation.")
    .add_property("server", &::getServerFeature, &::setServerFeature)
    ;
#endif

#ifdef AVANGO_OCULUS_RIFT_SUPPORT
  // Avango NG device: Oculus
  class_<av::daemon::Oculus, av::Link<av::daemon::Oculus>, bases<av::daemon::Device>, boost::noncopyable >("_OculusHelper",
    "A helper class that provides some basic properties and function inherited from Oculus,"
    "used to construct a concrete Python device representation.")
    ;
#endif

  // wrap helper function
  def("does_type_exist", &::doesTypeExist);

  // start the daemon
  def("run", &::run);
}
