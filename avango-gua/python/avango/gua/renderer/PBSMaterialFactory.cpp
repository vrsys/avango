#include "PBSMaterialFactory.hpp"

#include <boost/python.hpp>
#include <gua/renderer/PBSMaterialFactory.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Material.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
namespace python
{
template <class T>
struct pointee<av::Link<T>>
{
    using type = T;
};
} // namespace python
} // namespace boost

av::Link<av::gua::Material> create_material(int capabilities)
{
    return av::Link<av::gua::Material>(new av::gua::Material(::gua::PBSMaterialFactory::create_material(static_cast<::gua::PBSMaterialFactory::Capabilities>(capabilities))));
}

void init_PBSMaterialFactory()
{
    def("create_material", &create_material);

    enum_<::gua::PBSMaterialFactory::Capabilities>("MaterialCapabilities")
        .value("COLOR_VALUE", ::gua::PBSMaterialFactory::COLOR_VALUE)
        .value("COLOR_MAP", ::gua::PBSMaterialFactory::COLOR_MAP)
        .value("COLOR_VALUE_AND_MAP", ::gua::PBSMaterialFactory::COLOR_VALUE_AND_MAP)
        .value("METALNESS_VALUE", ::gua::PBSMaterialFactory::METALNESS_VALUE)
        .value("METALNESS_MAP", ::gua::PBSMaterialFactory::METALNESS_MAP)
        .value("ROUGHNESS_VALUE", ::gua::PBSMaterialFactory::ROUGHNESS_VALUE)
        .value("ROUGHNESS_MAP", ::gua::PBSMaterialFactory::ROUGHNESS_MAP)
        .value("EMISSIVITY_VALUE", ::gua::PBSMaterialFactory::EMISSIVITY_VALUE)
        .value("EMISSIVITY_MAP", ::gua::PBSMaterialFactory::EMISSIVITY_MAP)
        .value("NORMAL_MAP", ::gua::PBSMaterialFactory::NORMAL_MAP)
        .value("ALL", ::gua::PBSMaterialFactory::ALL);
}
