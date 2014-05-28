#include <avango/gua/renderer/PBRLoader.hpp>
#include <avango/gua/scenegraph/PBRNode.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::PBRLoader"));
}

AV_FC_DEFINE(av::gua::PBRLoader);

AV_FIELD_DEFINE(av::gua::SFPBRLoader);
AV_FIELD_DEFINE(av::gua::MFPBRLoader);

av::gua::PBRLoader::PBRLoader(::gua::PBRLoader* guaPBRLoader)
    : m_guaPBRLoader(guaPBRLoader)
{}

//av::gua::PBRLoader::~PBRLoader()
//{}

av::Link<av::gua::Node>
av::gua::PBRLoader::load( std::string const& nodename,
                              std::string const& fileName) const
{

    auto gua_node(m_guaPBRLoader->create_geometry_from_file(nodename, fileName));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

void
av::gua::PBRLoader::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::PBRLoader, true);

        SFPBRLoader::initClass("av::gua::SFPBRLoader", "av::Field");
        MFPBRLoader::initClass("av::gua::MFPBRLoader", "av::Field");
    }
}

::gua::PBRLoader*
av::gua::PBRLoader::getGuaPBRLoader() const
{
    return m_guaPBRLoader;
}

av::gua::Node*
av::gua::PBRLoader::createChildren(std::shared_ptr< ::gua::Node> root) const {

  av::gua::Node* av_node(nullptr);

  auto group_cast(std::dynamic_pointer_cast< ::gua::TransformNode>(root));
  if (group_cast) {
    av_node = new av::gua::TransformNode(group_cast);
  }

  if (!av_node) {
    auto geom_cast(std::dynamic_pointer_cast< ::gua::PBRNode>(root));
    if (geom_cast) {
      av_node = new av::gua::PBRNode(geom_cast);
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

bool av::gua::PBRLoader::is_supported(std::string const& fileName) const
{
  return m_guaPBRLoader->is_supported(fileName);
}
