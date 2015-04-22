#include <avango/gua/volume/VolumeLoader.hpp>
#include <avango/gua/volume/VolumeNode.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::VolumeLoader"));
}

AV_FC_DEFINE(av::gua::VolumeLoader);

AV_FIELD_DEFINE(av::gua::SFVolumeLoader);
AV_FIELD_DEFINE(av::gua::MFVolumeLoader);

av::gua::VolumeLoader::VolumeLoader(::gua::VolumeLoader* guaVolumeLoader)
    : m_guaVolumeLoader(guaVolumeLoader)
{}

//av::gua::VolumeLoader::~VolumeLoader()
//{}

av::Link<av::gua::Node>
av::gua::VolumeLoader::load( std::string const& nodename,
                              std::string const& fileName,
                              Flags flags) const
{

    auto gua_node(m_guaVolumeLoader->create_volume_from_file(nodename, fileName, flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

void
av::gua::VolumeLoader::initClass()
{
  if (!isTypeInitialized()) {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::gua::VolumeLoader, true);

    SFVolumeLoader::initClass("av::gua::SFVolumeLoader", "av::Field");
    MFVolumeLoader::initClass("av::gua::MFVolumeLoader", "av::Field");
  }
}

::gua::VolumeLoader*
av::gua::VolumeLoader::getGuaVolumeLoader() const
{
    return m_guaVolumeLoader;
}

av::gua::Node*
av::gua::VolumeLoader::createChildren(std::shared_ptr< ::gua::node::Node> root) const {

  av::gua::Node* av_node(nullptr);

  auto group_cast(std::dynamic_pointer_cast< ::gua::node::TransformNode>(root));
  if (group_cast) {
    av_node = new av::gua::TransformNode(group_cast);
  }

  if (!av_node) {
    auto geom_cast(std::dynamic_pointer_cast< ::gua::node::VolumeNode>(root));
    if (geom_cast) {
      av_node = new av::gua::VolumeNode(geom_cast);
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
