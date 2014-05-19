#include <avango/gua/renderer/Video3DLoader.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/gua/scenegraph/VolumeNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Video3DLoader"));
}

AV_FC_DEFINE(av::gua::Video3DLoader);

AV_FIELD_DEFINE(av::gua::SFVideo3DLoader);
AV_FIELD_DEFINE(av::gua::MFVideo3DLoader);

av::gua::Video3DLoader::Video3DLoader(::gua::Video3DLoader* guaVideo3DLoader)
    : m_guaVideo3DLoader(guaVideo3DLoader)
{}

//av::gua::Video3DLoader::~Video3DLoader()
//{}

av::Link<av::gua::Node>
av::gua::Video3DLoader::load( std::string const& fileName, Flags flags) const
{

    auto gua_node(m_guaVideo3DLoader->load(fileName, flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

void
av::gua::Video3DLoader::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Video3DLoader, true);

        SFVideo3DLoader::initClass("av::gua::SFVideo3DLoader", "av::Field");
        MFVideo3DLoader::initClass("av::gua::MFVideo3DLoader", "av::Field");
    }
}

::gua::Video3DLoader*
av::gua::Video3DLoader::getGuaVideo3DLoader() const
{
    return m_guaVideo3DLoader;
}

av::gua::Node*
av::gua::Video3DLoader::createChildren(std::shared_ptr< ::gua::Node> root) const {

  av::gua::Node* av_node(nullptr);

  auto group_cast(std::dynamic_pointer_cast< ::gua::TransformNode>(root));
  if (group_cast) {
    av_node = new av::gua::TransformNode(group_cast);
  }

  if (!av_node) {
    auto vol_cast(std::dynamic_pointer_cast< ::gua::VolumeNode>(root));
    if (vol_cast) {
      av_node = new av::gua::VolumeNode(vol_cast);
    }
  }

  if (!av_node) {
    auto geom_cast(std::dynamic_pointer_cast< ::gua::TriMeshNode>(root));
    if (geom_cast) {
      av_node = new av::gua::TriMeshNode(geom_cast);
    }
  }

  if (!av_node) {
    auto geom_cast(std::dynamic_pointer_cast< ::gua::Video3DNode>(root));
    if (geom_cast) {
      av_node = new av::gua::Video3DNode(geom_cast);
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
