#include <avango/gua/renderer/MaterialShaderDescription.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::MaterialShaderDescription"));
}

AV_FC_DEFINE(av::gua::MaterialShaderDescription);

AV_FIELD_DEFINE(av::gua::SFMaterialShaderDescription);
AV_FIELD_DEFINE(av::gua::MFMaterialShaderDescription);

av::gua::MaterialShaderDescription::MaterialShaderDescription(
  ::gua::MaterialShaderDescription const& guaMaterialShaderDescription)
    : m_guaMaterialShaderDescription(guaMaterialShaderDescription)
{

  AV_FC_ADD_ADAPTOR_FIELD(FileName,
                      boost::bind(&MaterialShaderDescription::getFileNameCB, this, _1),
                      boost::bind(&MaterialShaderDescription::setFileNameCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(VertexMethods,
                      boost::bind(&MaterialShaderDescription::getVertexMethodsCB, this, _1),
                      boost::bind(&MaterialShaderDescription::setVertexMethodsCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(FragmentMethods,
                      boost::bind(&MaterialShaderDescription::getFragmentMethodsCB, this, _1),
                      boost::bind(&MaterialShaderDescription::setFragmentMethodsCB, this, _1));
}

void
av::gua::MaterialShaderDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::MaterialShaderDescription, true);

        SFMaterialShaderDescription::initClass("av::gua::SFMaterialShaderDescription", "av::Field");
        MFMaterialShaderDescription::initClass("av::gua::MFMaterialShaderDescription", "av::Field");
    }
}


void
av::gua::MaterialShaderDescription::getFileNameCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaMaterialShaderDescription.get_file_name();
}

void
av::gua::MaterialShaderDescription::setFileNameCB(const SFString::SetValueEvent& event)
{
  m_guaMaterialShaderDescription.load_from_file(event.getValue());
}

void
av::gua::MaterialShaderDescription::getVertexMethodsCB(const MFMaterialShaderMethod::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_vertexMethods;
}

void
av::gua::MaterialShaderDescription::setVertexMethodsCB(const MFMaterialShaderMethod::SetValueEvent& event)
{

  for (auto& method : event.getValue()) {
    m_vertexMethods.push_back(method);
    m_guaMaterialShaderDescription.add_vertex_method(method->getGuaMaterialShaderMethod());
  }
}

void
av::gua::MaterialShaderDescription::getFragmentMethodsCB(const MFMaterialShaderMethod::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_fragmentMethods;
}

void
av::gua::MaterialShaderDescription::setFragmentMethodsCB(const MFMaterialShaderMethod::SetValueEvent& event)
{

  for (auto& method : event.getValue()) {
    m_fragmentMethods.push_back(method);
    m_guaMaterialShaderDescription.add_fragment_method(method->getGuaMaterialShaderMethod());
  }
}


::gua::MaterialShaderDescription const&
av::gua::MaterialShaderDescription::getGuaMaterialShaderDescription() const
{
    return m_guaMaterialShaderDescription;
}


