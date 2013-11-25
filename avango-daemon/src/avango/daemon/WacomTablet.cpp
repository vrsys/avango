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

#include <avango/Logger.h>
#include <avango/daemon/WacomTablet.h>
#include <math.h>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::WacomTablet"));
}

AV_BASE_DEFINE(av::daemon::WacomTablet);

av::daemon::WacomTablet::WacomTablet()
{}

av::daemon::WacomTablet::~WacomTablet()
{}

void
av::daemon::WacomTablet::initClass()
{
  if (!isTypeInitialized())
  {
    av::daemon::HIDInput::initClass();
    AV_BASE_INIT(av::daemon::HIDInput, av::daemon::WacomTablet, true);
  }
}

void
av::daemon::WacomTablet::startDevice()
{
  //auto-configure axes
  std::string station = getFirstStation() + "::STATION_VALUE::";
  configureFeature(station + "0", "EV_ABS::ABS_X");        // Pen positon X axis
  configureFeature(station + "1", "EV_ABS::ABS_Y");        // Pen position Y axis
  configureFeature(station + "2", "EV_ABS::ABS_RX");       // Left touch strip
  configureFeature(station + "3", "EV_ABS::ABS_RY");       // Right touch strip
  configureFeature(station + "4", "EV_ABS::ABS_RZ");       // Rotation on 4d mouse
  configureFeature(station + "5", "EV_ABS::ABS_THROTTLE"); // unused
  configureFeature(station + "6", "EV_ABS::ABS_WHEEL");    // Wheel on airbrush pen
  configureFeature(station + "7", "EV_ABS::ABS_PRESSURE"); // Pen pressure
  configureFeature(station + "8", "EV_ABS::ABS_DISTANCE"); // Pen distance
  configureFeature(station + "9", "EV_ABS::ABS_TILT_X");   // Pen tilt x axis
  configureFeature(station + "10", "EV_ABS::ABS_TILT_Y");  // Pen tilt y axis
  configureFeature(station + "11", "EV_REL::REL_WHEEL");   // Relative Wheel on mouse
  configureFeature(station + "12", "AspectRatio");         // Ratio of Width/Height of Tablet

  //auto-configure buttons
  station = getFirstStation() + "::STATION_BUTTON::";
  configureFeature(station + "0", "EV_KEY::BTN_0");              // Left Panel Button 1
  configureFeature(station + "1", "EV_KEY::BTN_1");              // Left Panel Button 2
  configureFeature(station + "2", "EV_KEY::BTN_2");              // Left Panel Button 3
  configureFeature(station + "3", "EV_KEY::BTN_3");              // Left Panel Button 4
  configureFeature(station + "4", "EV_KEY::BTN_4");              // Right Panel Button 1
  configureFeature(station + "5", "EV_KEY::BTN_5");              // Right Panel Button 2
  configureFeature(station + "6", "EV_KEY::BTN_6");              // Right Panel Button 3
  configureFeature(station + "7", "EV_KEY::BTN_7");              // Right Panel Button 4
  configureFeature(station + "8", "EV_KEY::BTN_LEFT");           // Left Mouse Button
  configureFeature(station + "9", "EV_KEY::BTN_RIGHT");          // Right Mouse Button
  configureFeature(station + "10", "EV_KEY::BTN_MIDDLE");        // Middle Mouse Button
  configureFeature(station + "11", "EV_KEY::BTN_SIDE");          // unused
  configureFeature(station + "12", "EV_KEY::BTN_EXTRA");         // unused
  configureFeature(station + "13", "EV_KEY::BTN_TOOL_PEN");      // Regular pen in proximity
  configureFeature(station + "14", "EV_KEY::BTN_TOOL_RUBBER");   // Rubber of pen/airbrush in prox
  configureFeature(station + "15", "EV_KEY::BTN_TOOL_BRUSH");    // unused
  configureFeature(station + "16", "EV_KEY::BTN_TOOL_PENCIL");   // unused
  configureFeature(station + "17", "EV_KEY::BTN_TOOL_AIRBRUSH"); // Airbrush in proximity
  configureFeature(station + "18", "EV_KEY::BTN_TOOL_FINGER");   // Panel-button or strip touched
  configureFeature(station + "19", "EV_KEY::BTN_TOOL_MOUSE");    // Mouse in proximity
  configureFeature(station + "20", "EV_KEY::BTN_TOOL_LENS");     // unused
  configureFeature(station + "21", "EV_KEY::BTN_TOUCH");   		 // any tool touches tablet
  configureFeature(station + "22", "EV_KEY::BTN_STYLUS");        // 1st side button on pen/airbrush
  configureFeature(station + "23", "EV_KEY::BTN_STYLUS2");       // 2nd side button on pen
  configureFeature(station + "24", "Proximity");                 // true if anything in proximity
  configureFeature(station + "25", "ToggleReset");               // should be true for Intuos3
}

void
av::daemon::WacomTablet::retrieveAspectRatio()
{
  HIDInput::AbsInfoMap::iterator it (mAbsInfoMap.find(0));
  const float x_max = (*it).second.maximum;

  it = (mAbsInfoMap.find(1));
  const float y_max = (*it).second.maximum;

  NumStationMap::iterator ns = mStations.begin();

  if ((0 < x_max)&&(0 < y_max))
    (*ns).second->setValue(12,x_max/y_max);
  else
    (*ns).second->setValue(12,1);
}

/* virtual */ void
av::daemon::WacomTablet::readLoop()
{
  float x,y;
  ::gua::math::mat4 matrix;

  if (!parse_features())
  {
    logger.warn() << "startDevice: required features missing, not started.";
    return;
  }

  NumStationMap::iterator ns;
  struct input_event      event;
  int                     reset_wait_counter(0);

  for (;;)
  {

    while (!isOpen())
    {
      if (open())
      {
        retrieve_abs_info();
        retrieveAspectRatio();
        stopLEDs();
        startLEDs();
      }
      else
      {
        logger.warn() << "readLoop: HID device is not open.";
        // sleep 5 seconds
        struct timeval timeout = { 5, 0 };
        while (0 != ::select(0, 0, 0, 0, &timeout)) {}
      }
    }

    updateLEDs();
    if (readEvent(&event)) {
      reset_wait_counter = 0;
      logger.trace() << "readLoop: time " << event.time.tv_sec << "." << event.time.tv_usec
                     << ", type " << event.type << ", code " << event.code << ", value " <<  event.value;

      for (ns = mStations.begin(); ns != mStations.end(); ++ns)
      {
        int station_index = ns->first;
        if (!stationLooksForEvent(station_index, event)) {
          continue;
        }
        applyEventToStation(station_index, event);
      }

      //set Transform Matrix
      ns = mStations.begin();
      x = (*ns).second->getValue(0);
      y = (*ns).second->getValue(1);
      matrix = ::scm::math::make_translation(::gua::math::vec3(x,y,0.0f));
      (*ns).second->setMatrix(matrix);

      //set Proximity Value and reset some buttons which else get wrong values with normalization
      for (int code=13; code<21; ++code)
      {
        if (0 == (*ns).second->getButton(code))
        {
          if (code == 20)
          {
            (*ns).second->setButton(24,0);
            if (mToggleReset)
            {
              (*ns).second->setValue(0,0);
              (*ns).second->setValue(1,0);
              (*ns).second->setValue(6,0);
              (*ns).second->setValue(8,0);
              (*ns).second->setValue(9,0);
              (*ns).second->setValue(10,0);
            }
          }
        }
        else
        {
          (*ns).second->setButton(24,1);
          code = 21;
        }
      }
    }
    else
    { // timeout
      ++reset_wait_counter;
      // reset all EV_REL mappings to 0.0 if not accumulating relative values
      if (!mAccumRel && (mResetRelCycle >= 0) && (reset_wait_counter >= mResetRelCycle))
      {
        reset_wait_counter = 0;
        clearRelativeStationValues();
        logger.trace() << "readLoop: resetting all EV_REL mappings.";
      }
    }
  }

}

float
av::daemon::WacomTablet::normalizeAbsValue(const input_event& event) const
{
  float normalized_value = 0.0f;
  HIDInput::AbsInfoMap::const_iterator iter (mAbsInfoMap.find(event.code));
  const input_absinfo& info = (*iter).second;
  NumStationMap::const_iterator ns = mStations.begin();

  switch (event.code) {
  case ABS_RX:
  case ABS_RY:
    if (event.value>0.0) normalized_value = log(event.value)/log(2)+1;
    else normalized_value = 0.0f;
    break;
  case ABS_PRESSURE:
    if (event.value>0.0)
      normalized_value = ((float)event.value -(float)info.minimum)/ (float)(info.maximum - info.minimum);
    break;
  case ABS_DISTANCE:
    if (event.value < 50) normalized_value = 1.0f;
    else normalized_value = 0.0f;
    break;
  case ABS_X:
  case ABS_WHEEL:
  case ABS_TILT_X:
    normalized_value = 2.0f* ((float)event.value -(float)info.minimum) / (float)(info.maximum + info.minimum ) - 1.0f;
    break;
  case ABS_Y:
  case ABS_TILT_Y:
    normalized_value = -2.0f* ((float)event.value -(float)info.minimum) / (float)(info.maximum + info.minimum ) + 1.0f;
    break;
  default:
    normalized_value = HIDInput::normalizeAbsValue(event);
  }

  return normalized_value;
}

/*virtual*/ int
av::daemon::WacomTablet::parse_features()
{
  if (HIDInput::parse_features())
    mToggleReset = HIDInput::scanForOptionalFeature<bool>("toggle-reset", false);
  return 1;
}
