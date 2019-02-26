
#include <osg/Vec4>
#include <osg/Geode>

#include <iostream>

#include <avango/Logger.h>
#include <avango/osg/Types.h>

#include <avango/vrpn/Service.h>

#include "vrpn_Tracker.h"
#include "vrpn_Button.h"
#include "vrpn_Analog.h"

namespace
{
av::Logger& logger(av::getLogger("av::vrpn::Service"));
}

AV_FC_DEFINE(av::vrpn::Service);

AV_FIELD_DEFINE(av::vrpn::SFService);
AV_FIELD_DEFINE(av::vrpn::MFService);

av::vrpn::Service::Service() : ::av::FieldContainer()
{
    AV_FC_ADD_FIELD(Devices, MFDevice::ContainerType());

    // the service should be evaluated each frame, since it must trigger the VRPN mainloops
    alwaysEvaluate(true);
}

av::vrpn::Service::~Service() {}

void av::vrpn::Service::initClass()
{
    if(!isTypeInitialized())
    {
        ::av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::vrpn::Service, true);

        SFService::initClass("av::vrpn::SFService", "av::Field");
        MFService::initClass("av::vrpn::MFService", "av::Field");
    }
}

/* virtual */ void av::vrpn::Service::fieldHasChanged(const av::Field& field)
{
    ::av::FieldContainer::fieldHasChanged(field);
    AVANGO_LOG(logger, av::logging::TRACE, "fieldHasChanged()");
}

/* virtual */ void av::vrpn::Service::evaluate()
{
    ::av::FieldContainer::evaluate();

    MFDevice::ContainerType devices = Devices.getValue();
    MFDevice::ContainerType::iterator iter;
    for(iter = devices.begin(); iter != devices.end(); ++iter)
    {
        (*iter)->triggerVRPNMainLoop();
    }

    vrpn_SleepMsecs(1);
}
