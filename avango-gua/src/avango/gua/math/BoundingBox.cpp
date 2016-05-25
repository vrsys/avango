#include <avango/gua/math/BoundingBox.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::BoundingBox"));
}

AV_FC_DEFINE(av::gua::BoundingBox);

AV_FIELD_DEFINE(av::gua::SFBoundingBox);
AV_FIELD_DEFINE(av::gua::MFBoundingBox);

av::gua::BoundingBox::BoundingBox(::gua::math::BoundingBox< ::gua::math::vec3>* guaBoundingBox)
  : m_guaBoundingBox(guaBoundingBox)
{

  AV_FC_ADD_ADAPTOR_FIELD(Min,
                        std::bind(&BoundingBox::getMinCB, this, std::placeholders::_1),
                        std::bind(&BoundingBox::setMinCB, this, std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(Max,
                        std::bind(&BoundingBox::getMaxCB, this, std::placeholders::_1),
                        std::bind(&BoundingBox::setMaxCB, this, std::placeholders::_1));
}

av::gua::BoundingBox::~BoundingBox()
{}

void
av::gua::BoundingBox::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::BoundingBox, true);

        SFBoundingBox::initClass("av::gua::SFBoundingBox", "av::Field");
        MFBoundingBox::initClass("av::gua::MFBoundingBox", "av::Field");
    }
}

::gua::math::BoundingBox< ::gua::math::vec3>*
av::gua::BoundingBox::getGuaBoundingBox() const
{
    return m_guaBoundingBox;
}


/* virtual */ void
av::gua::BoundingBox::getMinCB(const av::gua::SFVec3::GetValueEvent& event)
{

  *(event.getValuePtr()) = m_guaBoundingBox->min;
}

/* virtual */ void
av::gua::BoundingBox::setMinCB(const av::gua::SFVec3::SetValueEvent& event)
{

  m_guaBoundingBox->min = event.getValue();
}

/* virtual */ void
av::gua::BoundingBox::getMaxCB(const av::gua::SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaBoundingBox->max;
}

/* virtual */ void
av::gua::BoundingBox::setMaxCB(const av::gua::SFVec3::SetValueEvent& event)
{

  m_guaBoundingBox->max = event.getValue();
}


