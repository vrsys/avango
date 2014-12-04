#include <avango/gua/renderer/Material.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Material"));
}

AV_FC_DEFINE(av::gua::Material);

AV_FIELD_DEFINE(av::gua::SFMaterial);
AV_FIELD_DEFINE(av::gua::MFMaterial);

av::gua::Material::Material(::gua::Material const& guaMaterial)
    : m_guaMaterial(guaMaterial)
{}

void
av::gua::Material::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Material, true);

        SFMaterial::initClass("av::gua::SFMaterial", "av::Field");
        MFMaterial::initClass("av::gua::MFMaterial", "av::Field");
    }
}

::gua::Material const&
av::gua::Material::getGuaMaterial() const
{
    return m_guaMaterial;
}


