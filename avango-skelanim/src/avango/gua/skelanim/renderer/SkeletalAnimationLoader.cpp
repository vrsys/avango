#include <avango/gua/skelanim/renderer/SkeletalAnimationLoader.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/gua/scenegraph/VolumeNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::skelanim::SkeletalAnimationLoader"));
}

AV_FC_DEFINE(av::gua::skelanim::SkeletalAnimationLoader);

AV_FIELD_DEFINE(av::gua::skelanim::SFSkeletalAnimationLoader);
AV_FIELD_DEFINE(av::gua::skelanim::MFSkeletalAnimationLoader);

av::gua::skelanim::SkeletalAnimationLoader::SkeletalAnimationLoader(::gua::SkeletalAnimationLoader* guaSkeletalAnimationLoader)
    : m_guaSkeletalAnimationLoader(guaSkeletalAnimationLoader)
{}

//av::gua::skelanim::SkeletalAnimationLoader::~SkeletalAnimationLoader()
//{}

av::Link<av::gua::Node>
av::gua::skelanim::SkeletalAnimationLoader::createGeometryFromFile(std::string const& nodeName,
                                               std::string const& fileName,
                                               av::Link<av::gua::Material> const& fallbackMaterial,
                                               Flags flags) const
{

    auto gua_node(m_guaSkeletalAnimationLoader->create_geometry_from_file(
                                            nodeName, fileName, fallbackMaterial->getGuaMaterial(), flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

av::Link<av::gua::Node>
av::gua::skelanim::SkeletalAnimationLoader::createGeometryFromFile(std::string const& nodeName,
                                               std::string const& fileName,
                                               Flags flags) const
{

    auto gua_node(m_guaSkeletalAnimationLoader->create_geometry_from_file(
                                            nodeName, fileName, flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

void
av::gua::skelanim::SkeletalAnimationLoader::load_animation(av::Link<av::gua::Node>& node,
                                                 std::string const& file_name,
                                                 std::string const& animation_name) const
{
  auto gua_node(node->getGuaNode());

  m_guaSkeletalAnimationLoader->load_animation(gua_node, file_name, animation_name);
}

void
av::gua::skelanim::SkeletalAnimationLoader::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::skelanim::SkeletalAnimationLoader, true);

        SFSkeletalAnimationLoader::initClass("av::gua::SFSkeletalAnimationLoader", "av::Field");
        MFSkeletalAnimationLoader::initClass("av::gua::MFSkeletalAnimationLoader", "av::Field");
    }
}

::gua::SkeletalAnimationLoader*
av::gua::skelanim::SkeletalAnimationLoader::getGuaSkeletalAnimationLoader() const
{
    return m_guaSkeletalAnimationLoader;
}

av::gua::Node*
av::gua::skelanim::SkeletalAnimationLoader::createChildren(std::shared_ptr< ::gua::node::Node> root) const {

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
    auto geom_cast(std::dynamic_pointer_cast< ::gua::node::SkeletalAnimationNode>(root));
    if (geom_cast) {
      av_node = new av::gua::skelanim::SkeletalAnimationNode(geom_cast);
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
