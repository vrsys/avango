#include <avango/gua/Fields.hpp>
#include <avango/gua/Types.hpp>
#include <avango/MultiField.h>
#include <avango/SingleField.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Fields"));
}

AV_FIELD_DEFINE(av::gua::SFMatrix4);
AV_FIELD_DEFINE(av::gua::MFMatrix4);
AV_FIELD_DEFINE(av::gua::SFMatrix3);
AV_FIELD_DEFINE(av::gua::MFMatrix3);
AV_FIELD_DEFINE(av::gua::SFQuat);
AV_FIELD_DEFINE(av::gua::MFQuat);

AV_FIELD_DEFINE(av::gua::SFVec4);
AV_FIELD_DEFINE(av::gua::MFVec4);
AV_FIELD_DEFINE(av::gua::SFVec3);
AV_FIELD_DEFINE(av::gua::MFVec3);
AV_FIELD_DEFINE(av::gua::SFVec2);
AV_FIELD_DEFINE(av::gua::MFVec2);

AV_FIELD_DEFINE(av::gua::SFVec4i);
AV_FIELD_DEFINE(av::gua::MFVec4i);
AV_FIELD_DEFINE(av::gua::SFVec3i);
AV_FIELD_DEFINE(av::gua::MFVec3i);
AV_FIELD_DEFINE(av::gua::SFVec2i);
AV_FIELD_DEFINE(av::gua::MFVec2i);

AV_FIELD_DEFINE(av::gua::SFVec4ui);
AV_FIELD_DEFINE(av::gua::MFVec4ui);
AV_FIELD_DEFINE(av::gua::SFVec3ui);
AV_FIELD_DEFINE(av::gua::MFVec3ui);
AV_FIELD_DEFINE(av::gua::SFVec2ui);
AV_FIELD_DEFINE(av::gua::MFVec2ui);

AV_FIELD_DEFINE(av::gua::SFColor);
AV_FIELD_DEFINE(av::gua::MFColor);

void
av::gua::initFields()
{

    SFMatrix4::initClass("av::gua::SFMatrix4", "av::Field");
    MFMatrix4::initClass("av::gua::MFMatrix4", "av::Field");
    SFMatrix3::initClass("av::gua::SFMatrix3", "av::Field");
    MFMatrix3::initClass("av::gua::MFMatrix3", "av::Field");
    SFQuat::initClass("av::gua::SFQuat", "av::Field");
    MFQuat::initClass("av::gua::MFQuat", "av::Field");

    SFVec4::initClass("av::gua::SFVec4", "av::Field");
    MFVec4::initClass("av::gua::MFVec4", "av::Field");
    SFVec3::initClass("av::gua::SFVec3", "av::Field");
    MFVec3::initClass("av::gua::MFVec3", "av::Field");
    SFVec2::initClass("av::gua::SFVec2", "av::Field");
    MFVec2::initClass("av::gua::MFVec2", "av::Field");

    SFVec4i::initClass("av::gua::SFVec4i", "av::Field");
    MFVec4i::initClass("av::gua::MFVec4i", "av::Field");
    SFVec3i::initClass("av::gua::SFVec3i", "av::Field");
    MFVec3i::initClass("av::gua::MFVec3i", "av::Field");
    SFVec2i::initClass("av::gua::SFVec2i", "av::Field");
    MFVec2i::initClass("av::gua::MFVec2i", "av::Field");

    SFVec4ui::initClass("av::gua::SFVec4ui", "av::Field");
    MFVec4ui::initClass("av::gua::MFVec4ui", "av::Field");
    SFVec3ui::initClass("av::gua::SFVec3ui", "av::Field");
    MFVec3ui::initClass("av::gua::MFVec3ui", "av::Field");
    SFVec2ui::initClass("av::gua::SFVec2ui", "av::Field");
    MFVec2ui::initClass("av::gua::MFVec2ui", "av::Field");

    SFColor::initClass("av::gua::SFColor", "av::Field");
//    MFColor::initClass("av::gua::MFColor", "av::Field");
}


