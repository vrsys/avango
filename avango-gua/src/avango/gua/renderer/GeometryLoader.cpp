#include <avango/gua/renderer/GeometryLoader.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::GeometryLoader"));
}

AV_FC_DEFINE(av::gua::GeometryLoader);

AV_FIELD_DEFINE(av::gua::SFGeometryLoader);
AV_FIELD_DEFINE(av::gua::MFGeometryLoader);

av::gua::GeometryLoader::GeometryLoader(::gua::GeometryLoader* guaGeometryLoader)
    : m_guaGeometryLoader(guaGeometryLoader)
{}

//av::gua::GeometryLoader::~GeometryLoader()
//{}

av::Link<av::gua::Node>
av::gua::GeometryLoader::createGeometryFromFile(std::string const& nodeName,
                                                std::string const& fileName,
                                                std::string const& fallbackMaterial,
                                                Flags flags) const
{

    auto gua_node(m_guaGeometryLoader->create_geometry_from_file(
                                            nodeName, fileName, fallbackMaterial, flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

void
av::gua::GeometryLoader::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::GeometryLoader, true);

        SFGeometryLoader::initClass("av::gua::SFGeometryLoader", "av::Field");
        MFGeometryLoader::initClass("av::gua::MFGeometryLoader", "av::Field");
    }
}

::gua::GeometryLoader*
av::gua::GeometryLoader::getGuaGeometryLoader() const
{
    return m_guaGeometryLoader;
}

av::gua::Node*
av::gua::GeometryLoader::createChildren(std::shared_ptr< ::gua::Node> root) const {
  for (auto c : root->get_children()) {
    createChildren(c);
  }

  auto group_cast(std::dynamic_pointer_cast< ::gua::TransformNode>(root));
  if (group_cast) {
    return new av::gua::TransformNode(group_cast);
  } else {
    auto geom_cast(std::dynamic_pointer_cast< ::gua::GeometryNode>(root));
    return new av::gua::GeometryNode(geom_cast);
  }

}
