#include <avango/gua/renderer/SPointsLoader.hpp>
#include <avango/gua/scenegraph/SPointsNode.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::SPointsLoader"));
}

AV_FC_DEFINE(av::gua::SPointsLoader);

AV_FIELD_DEFINE(av::gua::SFSPointsLoader);
AV_FIELD_DEFINE(av::gua::MFSPointsLoader);

av::gua::SPointsLoader::SPointsLoader(::gua::SPointsLoader* guaSPointsLoader)
    : m_guaSPointsLoader(guaSPointsLoader)
{}

//av::gua::SPointsLoader::~SPointsLoader()
//{}

av::Link<av::gua::Node>
av::gua::SPointsLoader::load( std::string const& nodename,
                              std::string const& fileName) const
{

    auto gua_node(m_guaSPointsLoader->create_geometry_from_file(nodename, fileName));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

void
av::gua::SPointsLoader::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::SPointsLoader, true);

        SFSPointsLoader::initClass("av::gua::SFSPointsLoader", "av::Field");
        MFSPointsLoader::initClass("av::gua::MFSPointsLoader", "av::Field");
    }
}

::gua::SPointsLoader*
av::gua::SPointsLoader::getGuaSPointsLoader() const
{
    return m_guaSPointsLoader;
}

av::gua::Node*
av::gua::SPointsLoader::createChildren(std::shared_ptr< ::gua::node::Node> root) const {

  av::gua::Node* av_node(nullptr);

  auto group_cast(std::dynamic_pointer_cast< ::gua::node::TransformNode>(root));
  if (group_cast) {
    av_node = new av::gua::TransformNode(group_cast);
  }

  if (!av_node) {
    auto geom_cast(std::dynamic_pointer_cast< ::gua::node::SPointsNode>(root));
    if (geom_cast) {
      av_node = new av::gua::SPointsNode(geom_cast);
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
