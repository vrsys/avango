#include <avango/gua/renderer/DynamicTriangleLoader.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::DynamicTriangleLoader"));
}

AV_FC_DEFINE(av::gua::DynamicTriangleLoader);

AV_FIELD_DEFINE(av::gua::SFDynamicTriangleLoader);
AV_FIELD_DEFINE(av::gua::MFDynamicTriangleLoader);

av::gua::DynamicTriangleLoader::DynamicTriangleLoader(::gua::DynamicTriangleLoader* guaDynamicTriangleLoader)
    : m_guaDynamicTriangleLoader(guaDynamicTriangleLoader)
{}

//av::gua::DynamicTriangleLoader::~DynamicTriangleLoader()
//{}




av::Link<av::gua::Node>
av::gua::DynamicTriangleLoader::createEmptyGeometry(std::string const& nodeName,
                                              std::string const& emptyName,
                                              av::gua::Material const& fallbackMaterial,
                                              unsigned flags) const
{
    auto gua_node(m_guaDynamicTriangleLoader->create_empty_geometry(
                                            nodeName, emptyName, fallbackMaterial.getGuaMaterial(), flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

av::Link<av::gua::Node>
av::gua::DynamicTriangleLoader::createEmptyGeometry(std::string const& nodeName,
                                              std::string const& emptyName,
                                              unsigned flags) const
{
    auto gua_node(m_guaDynamicTriangleLoader->create_empty_geometry(nodeName, emptyName, flags));
    auto root(createChildren(gua_node));
    
    return av::Link<av::gua::Node>(root);
}

void
av::gua::DynamicTriangleLoader::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::DynamicTriangleLoader, true);

        SFDynamicTriangleLoader::initClass("av::gua::SFDynamicTriangleLoader", "av::Field");
        MFDynamicTriangleLoader::initClass("av::gua::MFDynamicTriangleLoader", "av::Field");
    }
}

::gua::DynamicTriangleLoader*
av::gua::DynamicTriangleLoader::getGuaDynamicTriangleLoader() const
{
    return m_guaDynamicTriangleLoader;
}

av::gua::Node*
av::gua::DynamicTriangleLoader::createChildren(std::shared_ptr< ::gua::node::Node> root) const {

  av::gua::Node* av_node(nullptr);

  auto group_cast(std::dynamic_pointer_cast< ::gua::node::TransformNode>(root));
  if (group_cast) {
    av_node = new av::gua::TransformNode(group_cast);
  }

  // if (!av_node) {
  //   auto vol_cast(std::dynamic_pointer_cast< ::gua::node::VolumeNode>(root));
  //   if (vol_cast) {
  //     av_node = new av::gua::VolumeNode(vol_cast);
  //   }
  // }

  if (!av_node) {
    auto geom_cast(std::dynamic_pointer_cast< ::gua::node::DynamicTriangleNode>(root));
    if (geom_cast) {
      av_node = new av::gua::DynamicTriangleNode(geom_cast);
    }
  }

  if (av_node) {
    av_node->addToParentChildren();
  } else {
    std::cout << "Unexpected node type encountered while loading geometry!" << std::endl;
  }


  for (auto c : root->get_children()) {
    createChildren(c);
  }

  return av_node;
}
