#include <avango/gua/renderer/MaterialShaderMethod.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::MaterialShaderMethod"));
}

AV_FC_DEFINE(av::gua::MaterialShaderMethod);

AV_FIELD_DEFINE(av::gua::SFMaterialShaderMethod);
AV_FIELD_DEFINE(av::gua::MFMaterialShaderMethod);

av::gua::MaterialShaderMethod::MaterialShaderMethod(std::shared_ptr<::gua::MaterialShaderMethod> const& guaMaterialShaderMethod)
    : m_guaMaterialShaderMethod(guaMaterialShaderMethod), m_distributed(false)
{
    AV_FC_ADD_ADAPTOR_FIELD(Name, std::bind(&MaterialShaderMethod::getNameCB, this, std::placeholders::_1), std::bind(&MaterialShaderMethod::setNameCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Source, std::bind(&MaterialShaderMethod::getSourceCB, this, std::placeholders::_1), std::bind(&MaterialShaderMethod::setSourceCB, this, std::placeholders::_1));

    AV_FC_ADD_FIELD(m_serializedUniforms, "");
    AV_FC_ADD_FIELD(m_uniformsDirty, false);
}

void av::gua::MaterialShaderMethod::on_distribute(av::gua::NetTransform& netNode)
{
    m_distributed = true;
    m_uniformsDirty.setValue(true);
}

void av::gua::MaterialShaderMethod::on_undistribute(av::gua::NetTransform& netNode) { m_distributed = false; }

void av::gua::MaterialShaderMethod::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::MaterialShaderMethod, true);

        SFMaterialShaderMethod::initClass("av::gua::SFMaterialShaderMethod", "av::Field");
        MFMaterialShaderMethod::initClass("av::gua::MFMaterialShaderMethod", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

void av::gua::MaterialShaderMethod::fieldHasChangedLocalSideEffect(Field const& field)
{
    if(field.getName() == "m_serializedUniforms")
    {
        if(!m_distributed)
        {
            m_guaMaterialShaderMethod->set_uniforms_from_serialized_string(m_serializedUniforms.getValue());
        }
    }
    else if(field.getName() == "m_uniformsDirty")
    {
        if(m_distributed)
        {
            std::stringstream sstr;
            m_guaMaterialShaderMethod->serialize_uniforms_to_stream(sstr);
            m_serializedUniforms.setValue(sstr.str());
        }
    }
}

void av::gua::MaterialShaderMethod::getNameCB(const SFString::GetValueEvent& event) { *(event.getValuePtr()) = m_guaMaterialShaderMethod->get_name(); }

void av::gua::MaterialShaderMethod::setNameCB(const SFString::SetValueEvent& event) { m_guaMaterialShaderMethod->set_name(event.getValue()); }

void av::gua::MaterialShaderMethod::getSourceCB(const SFString::GetValueEvent& event) { *(event.getValuePtr()) = m_guaMaterialShaderMethod->get_source(); }

void av::gua::MaterialShaderMethod::setSourceCB(const SFString::SetValueEvent& event) { m_guaMaterialShaderMethod->set_source(event.getValue()); }

std::shared_ptr<::gua::MaterialShaderMethod> const& av::gua::MaterialShaderMethod::getGuaMaterialShaderMethod() const { return m_guaMaterialShaderMethod; }
