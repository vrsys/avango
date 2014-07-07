#include <avango/gua/math/BoundingSphere.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::BoundingSphere"));
}

AV_FC_DEFINE(av::gua::BoundingSphere);

AV_FIELD_DEFINE(av::gua::SFBoundingSphere);
AV_FIELD_DEFINE(av::gua::MFBoundingSphere);

av::gua::BoundingSphere::BoundingSphere(::gua::math::BoundingSphere< ::gua::math::vec3>* guaBoundingSphere)
  : m_guaBoundingSphere(guaBoundingSphere)
{

  AV_FC_ADD_ADAPTOR_FIELD(Center,
                        boost::bind(&BoundingSphere::getCenterCB, this, _1),
                        boost::bind(&BoundingSphere::setCenterCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Radius,
                        boost::bind(&BoundingSphere::getRadiusCB, this, _1),
                        boost::bind(&BoundingSphere::setRadiusCB, this, _1));
}

av::gua::BoundingSphere::~BoundingSphere()
{}

void
av::gua::BoundingSphere::initClass()
{
  if (!isTypeInitialized()) {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::gua::BoundingSphere, true);

    SFBoundingSphere::initClass("av::gua::SFBoundingSphere", "av::Field");
    MFBoundingSphere::initClass("av::gua::MFBoundingSphere", "av::Field");
  }
}

::gua::math::BoundingSphere< ::gua::math::vec3>*
av::gua::BoundingSphere::getGuaBoundingSphere() const
{
    return m_guaBoundingSphere;
}

/* virtual */ void
av::gua::BoundingSphere::getCenterCB(const av::gua::SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaBoundingSphere->center;
}

/* virtual */ void
av::gua::BoundingSphere::setCenterCB(const av::gua::SFVec3::SetValueEvent& event)
{
  m_guaBoundingSphere->center = event.getValue();
}

/* virtual */ void
av::gua::BoundingSphere::getRadiusCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaBoundingSphere->radius;
}

/* virtual */ void
av::gua::BoundingSphere::setRadiusCB(const av::SFFloat::SetValueEvent& event)
{
  m_guaBoundingSphere->radius = event.getValue();
}


