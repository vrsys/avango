#include <avango/gua/renderer/MaterialShaderDescription.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

namespace
{
  av::Logger& logger(av::getLogger("av::gua::MaterialShaderDescription"));
}

AV_FC_DEFINE(av::gua::MaterialShaderDescription);

AV_FIELD_DEFINE(av::gua::SFMaterialShaderDescription);
AV_FIELD_DEFINE(av::gua::MFMaterialShaderDescription);

av::gua::MaterialShaderDescription::MaterialShaderDescription(
  std::shared_ptr< ::gua::MaterialShaderDescription> const& guaMaterialShaderDescription)
    : m_guaMaterialShaderDescription(guaMaterialShaderDescription)
{

  AV_FC_ADD_ADAPTOR_FIELD(VertexMethods,
                      std::bind(&MaterialShaderDescription::getVertexMethodsCB, this,std::placeholders::_1),
                      std::bind(&MaterialShaderDescription::setVertexMethodsCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(FragmentMethods,
                      std::bind(&MaterialShaderDescription::getFragmentMethodsCB, this,std::placeholders::_1),
                      std::bind(&MaterialShaderDescription::setFragmentMethodsCB, this,std::placeholders::_1));

  for (auto& method : guaMaterialShaderDescription->get_vertex_methods()) {
    m_vertexMethods.push_back(new av::gua::MaterialShaderMethod(method));
  }

  for (auto& method : guaMaterialShaderDescription->get_fragment_methods()) {
    m_fragmentMethods.push_back(new av::gua::MaterialShaderMethod(method));
  }
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

        sClassTypeId.setDistributable(true);
    }
}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
void av::gua::MaterialShaderDescription::on_distribute(av::gua::NetTransform& netNode) 
{
  for (auto& method : VertexMethods.getValue()) {
    method->on_distribute(netNode);
    netNode.distributeFieldContainer(method);
  }
  for (auto& method : FragmentMethods.getValue()) {
    method->on_distribute(netNode);
    netNode.distributeFieldContainer(method);
  }
}

void av::gua::MaterialShaderDescription::on_undistribute(av::gua::NetTransform& netNode) 
{
  for (auto& method : VertexMethods.getValue()) {
    method->on_undistribute(netNode);
    netNode.undistributeFieldContainer(method);
  }
  for (auto& method : FragmentMethods.getValue()) {
    method->on_undistribute(netNode);
    netNode.undistributeFieldContainer(method);
  }
}
#endif

void
av::gua::MaterialShaderDescription::getVertexMethodsCB(const MFMaterialShaderMethod::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_vertexMethods;
}

void
av::gua::MaterialShaderDescription::setVertexMethodsCB(const MFMaterialShaderMethod::SetValueEvent& event)
{
  m_vertexMethods.clear();
  m_guaMaterialShaderDescription->clear_vertex_methods();

  for (auto& method : event.getValue()) {
    m_vertexMethods.push_back(method);
    m_guaMaterialShaderDescription->add_vertex_method(method->getGuaMaterialShaderMethod());
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
  m_fragmentMethods.clear();
  m_guaMaterialShaderDescription->clear_fragment_methods();

  for (auto& method : event.getValue()) {
    m_fragmentMethods.push_back(method);
    m_guaMaterialShaderDescription->add_fragment_method(method->getGuaMaterialShaderMethod());
  }
}

void 
av::gua::MaterialShaderDescription::load_from_file(std::string const& file) {
  m_guaMaterialShaderDescription->load_from_file(file);

  for (auto& method : m_guaMaterialShaderDescription->get_vertex_methods()) {
    m_vertexMethods.push_back(new av::gua::MaterialShaderMethod(method));
  }

  for (auto& method : m_guaMaterialShaderDescription->get_fragment_methods()) {
    m_fragmentMethods.push_back(new av::gua::MaterialShaderMethod(method));
  }
}

void 
av::gua::MaterialShaderDescription::load_from_json(std::string const& json) {
  m_guaMaterialShaderDescription->load_from_json(json);

  for (auto& method : m_guaMaterialShaderDescription->get_vertex_methods()) {
    m_vertexMethods.push_back(new av::gua::MaterialShaderMethod(method));
  }

  for (auto& method : m_guaMaterialShaderDescription->get_fragment_methods()) {
    m_fragmentMethods.push_back(new av::gua::MaterialShaderMethod(method));
  }
}

std::shared_ptr< ::gua::MaterialShaderDescription> const&
av::gua::MaterialShaderDescription::getGuaMaterialShaderDescription() const
{
    return m_guaMaterialShaderDescription;
}


