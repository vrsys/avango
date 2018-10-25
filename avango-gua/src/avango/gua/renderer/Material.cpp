#include <avango/gua/renderer/Material.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

#include <avango/gua/Types.hpp>

#include <gua/databases/MaterialShaderDatabase.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Material"));
}

AV_FC_DEFINE(av::gua::Material);

AV_FIELD_DEFINE(av::gua::SFMaterial);
AV_FIELD_DEFINE(av::gua::MFMaterial);

av::gua::Material::Material(std::shared_ptr< ::gua::Material> const& guaMaterial)
    : m_guaMaterial(guaMaterial)
    , m_distributed(false)
{
    AV_FC_ADD_ADAPTOR_FIELD(ShaderName,
                      std::bind(&Material::getShaderNameCB, this,std::placeholders::_1),
                      std::bind(&Material::setShaderNameCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableBackfaceCulling,
                      std::bind(&Material::getEnableBackfaceCullingCB, this,std::placeholders::_1),
                      std::bind(&Material::setEnableBackfaceCullingCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableWireframeRendering,
                      std::bind(&Material::getEnableWireframeRenderingCB, this,std::placeholders::_1),
                      std::bind(&Material::setEnableWireframeRenderingCB, this,std::placeholders::_1));

#if defined(AVANGO_VIRTUAL_TEXTURING_SUPPORT)
    AV_FC_ADD_ADAPTOR_FIELD(EnableVirtualTexturing,
                      std::bind(&Material::getEnableVirtualTexturingCB, this,std::placeholders::_1),
                      std::bind(&Material::setEnableVirtualTexturingCB, this,std::placeholders::_1));
#endif

    AV_FC_ADD_FIELD(m_materialShaderDescription, SFMaterialShaderDescription::ValueType());
    AV_FC_ADD_FIELD(m_serializedUniforms, "");
    AV_FC_ADD_FIELD(m_uniformsDirty, false);
    m_uniformsDirty.dontDistribute(true);    
}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
void av::gua::Material::on_distribute(av::gua::NetTransform& netNode) 
{
  // store material description for distribution
  if (ShaderName.getValue() != "") {
    auto shader(::gua::MaterialShaderDatabase::instance()->lookup(ShaderName.getValue()));
    if (shader) {
      m_materialShaderDescription.setValue(new av::gua::MaterialShaderDescription(shader->get_description()));
    }
  }

  if (m_materialShaderDescription.getValue().isValid()) {
    m_materialShaderDescription.getValue()->on_distribute(netNode);
    netNode.distributeFieldContainer(m_materialShaderDescription.getValue());
  }

  m_distributed = true;
  m_uniformsDirty.setValue(true);
}

void av::gua::Material::on_undistribute(av::gua::NetTransform& netNode) 
{   
  if (m_materialShaderDescription.getValue().isValid()) {
    m_materialShaderDescription.getValue()->on_undistribute(netNode);
  }
  netNode.undistributeFieldContainer(m_materialShaderDescription.getValue());
  m_distributed = false;
}
#endif

void
av::gua::Material::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Material, true);

        SFMaterial::initClass("av::gua::SFMaterial", "av::Field");
        MFMaterial::initClass("av::gua::MFMaterial", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

void av::gua::Material::fieldHasChangedLocalSideEffect(Field const& field) {
    if (field.getName() == "m_serializedUniforms") {
        if (!m_distributed) {
            m_guaMaterial->set_uniforms_from_serialized_string(m_serializedUniforms.getValue());
        }
    } else if (field.getName() == "m_uniformsDirty") {
        if (m_distributed) {
            std::stringstream sstr;
            m_guaMaterial->serialize_uniforms_to_stream(sstr);
            m_serializedUniforms.setValue(sstr.str());
        }
    }
}

void av::gua::Material::evaluateLocalSideEffect() {
  // load shader on client side
  if (!m_distributed) {
    if (ShaderName.getValue() != "") {
      if (m_materialShaderDescription.getValue().isValid()) {
        if (!::gua::MaterialShaderDatabase::instance()->contains(ShaderName.getValue())) {
            auto shader(std::make_shared<::gua::MaterialShader>(ShaderName.getValue(), m_materialShaderDescription.getValue()->getGuaMaterialShaderDescription()));
            ::gua::MaterialShaderDatabase::instance()->add(shader);
        }
      }
    }
  }
}

void
av::gua::Material::getShaderNameCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaMaterial->get_shader_name();
}

void
av::gua::Material::setShaderNameCB(const SFString::SetValueEvent& event)
{
  m_guaMaterial->set_shader_name(event.getValue());
  m_uniformsDirty.setValue(true);

  // store material description for distribution
  if (event.getValue() != "" && m_distributed) {
    auto shader(::gua::MaterialShaderDatabase::instance()->lookup(event.getValue()));
    if (shader) {
      m_materialShaderDescription.setValue(new av::gua::MaterialShaderDescription(shader->get_description()));
    }
  }
}

void
av::gua::Material::getEnableBackfaceCullingCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = !m_guaMaterial->get_show_back_faces();
}

void
av::gua::Material::setEnableBackfaceCullingCB(const SFBool::SetValueEvent& event)
{
  m_guaMaterial->set_show_back_faces(!event.getValue());
}

void
av::gua::Material::getEnableWireframeRenderingCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaMaterial->get_render_wireframe();
}

void
av::gua::Material::setEnableWireframeRenderingCB(const SFBool::SetValueEvent& event)
{
  m_guaMaterial->set_render_wireframe(event.getValue());
}

#if defined(AVANGO_VIRTUAL_TEXTURING_SUPPORT)
void
av::gua::Material::getEnableVirtualTexturingCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaMaterial->get_enable_virtual_texturing();
}

void
av::gua::Material::setEnableVirtualTexturingCB(const SFBool::SetValueEvent& event)
{
  m_guaMaterial->set_enable_virtual_texturing(event.getValue());
}
#endif

std::shared_ptr< ::gua::Material> const&
av::gua::Material::getGuaMaterial() const
{
    return m_guaMaterial;
}


