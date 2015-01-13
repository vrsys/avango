#include <avango/gua/renderer/Material.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>
#include <avango/gua/network/NetTransform.h>

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
{

    AV_FC_ADD_FIELD(m_materialShaderName, "");
    AV_FC_ADD_FIELD(m_materialShaderDescription, SFMaterialShaderDescription::ValueType());

    // for remote construction
    auto name(guaMaterial->get_shader_name());
    m_materialShaderName.setValue(name);

    if (name != "") {
        auto shader(::gua::MaterialShaderDatabase::instance()->lookup(name));
        if (shader) {
            m_materialShaderDescription.setValue(new av::gua::MaterialShaderDescription(shader->get_description()));
        }
    }
}

void av::gua::Material::on_distribute(av::gua::NetTransform& netNode) 
{
    netNode.distributeFieldContainer(m_materialShaderDescription.getValue());
}

void av::gua::Material::on_undistribute(av::gua::NetTransform& netNode) 
{
    netNode.undistributeFieldContainer(m_materialShaderDescription.getValue());
}

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
    std::cout << "----------------------- " << m_materialShaderName.getValue() << std::endl;
    if (field.getName() == "m_materialShaderName" || field.getName() == "m_materialShaderDescription") {
        if (m_materialShaderName.getValue() != "" && m_materialShaderDescription.getValue().isValid()) {
            if (!::gua::MaterialShaderDatabase::instance()->contains(m_materialShaderName.getValue())) {
                auto shader(std::make_shared<::gua::MaterialShader>(m_materialShaderName.getValue(), m_materialShaderDescription.getValue()->getGuaMaterialShaderDescription()));
                ::gua::MaterialShaderDatabase::instance()->add(shader);
            }
        }
    }
}


std::shared_ptr< ::gua::Material> const&
av::gua::Material::getGuaMaterial() const
{
    return m_guaMaterial;
}


