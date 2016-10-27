#include <avango/gua/skelanim/renderer/Bone.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::Bone);

AV_FIELD_DEFINE(av::gua::SFBone);
AV_FIELD_DEFINE(av::gua::MFBone);

av::gua::Bone::Bone(std::shared_ptr< ::gua::Ray> guaBone)
    : m_guaBone(guaBone)
{
    AV_FC_ADD_ADAPTOR_FIELD(Origin,
                          std::bind(&Bone::getOriginCB, this, std::placeholders::_1),
                          std::bind(&Bone::setOriginCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(Direction,
                          std::bind(&Bone::getDirectionCB, this, std::placeholders::_1),
                          std::bind(&Bone::setDirectionCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(TMax,
                          std::bind(&Bone::getTMaxCB, this, std::placeholders::_1),
                          std::bind(&Bone::setTMaxCB, this, std::placeholders::_1));
}


void
av::gua::Bone::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Bone, true);

        SFBone::initClass("av::gua::SFBone", "av::Field");
        MFBone::initClass("av::gua::MFBone", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::Ray>
av::gua::Bone::getGuaBone() const
{
    return m_guaBone;
}

void
av::gua::Bone::getOriginCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaBone->origin_;
}

void
av::gua::Bone::setOriginCB(const SFVec3::SetValueEvent& event)
{
    m_guaBone->origin_ = event.getValue();
}

void
av::gua::Bone::getDirectionCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaBone->direction_;
}

void
av::gua::Bone::setDirectionCB(const SFVec3::SetValueEvent& event)
{
    m_guaBone->direction_ = event.getValue();
}

void
av::gua::Bone::getTMaxCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaBone->t_max_;
}

void
av::gua::Bone::setTMaxCB(const SFFloat::SetValueEvent& event)
{
    m_guaBone->t_max_ = event.getValue();
}
