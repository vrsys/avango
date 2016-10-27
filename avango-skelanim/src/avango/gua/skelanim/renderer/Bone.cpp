#include <avango/gua/skelanim/renderer/Bone.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::skelanim::Bone);

AV_FIELD_DEFINE(av::gua::skelanim::SFBone);
AV_FIELD_DEFINE(av::gua::skelanim::MFBone);

av::gua::skelanim::Bone::Bone(::gua::Bone const& guaBone) {
  name.setValue(guaBone.name);
  children.setValue(guaBone.children);
  idle_matrix.setValue(::gua::math::mat4{guaBone.idle_matrix});
  offset_matrix.setValue(::gua::math::mat4{guaBone.offset_matrix});
}

void
av::gua::skelanim::Bone::initClass() {
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::gua::skelanim::Bone, true);

    SFBone::initClass("av::gua::skelanim::SFBone", "av::Field");
    MFBone::initClass("av::gua::skelanim::MFBone", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::gua::Bone
av::gua::skelanim::Bone::getGuaBone() const {
  ::gua::Bone guaBone{name.getValue(), 
    scm::math::mat4f{idle_matrix.getValue()},
    scm::math::mat4f{offset_matrix.getValue()},
    children.getValue()
  };
  return guaBone;
}