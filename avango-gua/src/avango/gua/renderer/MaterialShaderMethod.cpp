#include <avango/gua/renderer/MaterialShaderMethod.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::MaterialShaderMethod"));
}

AV_FC_DEFINE(av::gua::MaterialShaderMethod);

AV_FIELD_DEFINE(av::gua::SFMaterialShaderMethod);
AV_FIELD_DEFINE(av::gua::MFMaterialShaderMethod);

av::gua::MaterialShaderMethod::MaterialShaderMethod(::gua::MaterialShaderMethod const& guaMaterialShaderMethod)
    : m_guaMaterialShaderMethod(guaMaterialShaderMethod)
{

  AV_FC_ADD_ADAPTOR_FIELD(FileName,
                      boost::bind(&MaterialShaderMethod::getFileNameCB, this, _1),
                      boost::bind(&MaterialShaderMethod::setFileNameCB, this, _1));

}

void
av::gua::MaterialShaderMethod::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::MaterialShaderMethod, true);

        SFMaterialShaderMethod::initClass("av::gua::SFMaterialShaderMethod", "av::Field");
        MFMaterialShaderMethod::initClass("av::gua::MFMaterialShaderMethod", "av::Field");
    }
}


void
av::gua::MaterialShaderMethod::getFileNameCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaMaterialShaderMethod.get_file_name();
}

void
av::gua::MaterialShaderMethod::setFileNameCB(const SFString::SetValueEvent& event)
{
  m_guaMaterialShaderMethod.load_from_file(event.getValue());
}

::gua::MaterialShaderMethod const&
av::gua::MaterialShaderMethod::getGuaMaterialShaderMethod() const
{
    return m_guaMaterialShaderMethod;
}


