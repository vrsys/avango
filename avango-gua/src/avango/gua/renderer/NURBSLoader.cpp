#include <avango/gua/renderer/NURBSLoader.hpp>
#include <avango/gua/scenegraph/NURBSNode.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::NURBSLoader"));
}

AV_FC_DEFINE(av::gua::NURBSLoader);

AV_FIELD_DEFINE(av::gua::SFNURBSLoader);
AV_FIELD_DEFINE(av::gua::MFNURBSLoader);

av::gua::NURBSLoader::NURBSLoader(::gua::NURBSLoader* guaNURBSLoader)
    : m_guaNURBSLoader(guaNURBSLoader)
{}

//av::gua::NURBSLoader::~NURBSLoader()
//{}

av::Link<av::gua::Node>
av::gua::NURBSLoader::createGeometryFromFile(
    std::string const& nodename,
    std::string const& fileName,
    std::string const& material,
    Flags flags) const
{

    auto gua_node(m_guaNURBSLoader->create_geometry_from_file(
                                  nodename, fileName, material, flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

void
av::gua::NURBSLoader::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::NURBSLoader, true);

        SFNURBSLoader::initClass("av::gua::SFNURBSLoader", "av::Field");
        MFNURBSLoader::initClass("av::gua::MFNURBSLoader", "av::Field");
    }
}

::gua::NURBSLoader*
av::gua::NURBSLoader::getGuaNURBSLoader() const
{
    return m_guaNURBSLoader;
}

av::gua::Node*
av::gua::NURBSLoader::createChildren(std::shared_ptr< ::gua::node::Node> root) const {

  av::gua::Node* av_node(nullptr);

  auto group_cast(std::dynamic_pointer_cast< ::gua::node::TransformNode>(root));
  if (group_cast) {
    av_node = new av::gua::TransformNode(group_cast);
  }

  if (!av_node) {
    auto geom_cast(std::dynamic_pointer_cast< ::gua::node::NURBSNode>(root));
    if (geom_cast) {
      av_node = new av::gua::NURBSNode(geom_cast);
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
