
#include <osg/Vec4>
#include <osg/Geode>

#include <iostream>

#include <avango/Logger.h>
#include <avango/osg/Types.h>

#include <avango/vrpn/Button.h>

namespace
{
av::Logger& logger(av::getLogger("av::vrpn::Button"));
}

AV_FC_DEFINE(av::vrpn::Button);

AV_FIELD_DEFINE(av::vrpn::SFButton);
AV_FIELD_DEFINE(av::vrpn::MFButton);

av::vrpn::Button::Button() : ::av::FieldContainer()
{
    AV_FC_ADD_FIELD(State, false);
    AV_FC_ADD_FIELD(Number, -1);
}

av::vrpn::Button::~Button() {}

void av::vrpn::Button::initClass()
{
    if(!isTypeInitialized())
    {
        ::av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::vrpn::Button, true);

        SFButton::initClass("av::vrpn::SFButton", "av::Field");
        MFButton::initClass("av::vrpn::MFButton", "av::Field");
    }
}
