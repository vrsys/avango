#include <avango/gua/renderer/TriMeshLoader.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/gua/scenegraph/VolumeNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::TriMeshLoader"));
}

AV_FC_DEFINE(av::gua::TriMeshLoader);

AV_FIELD_DEFINE(av::gua::SFTriMeshLoader);
AV_FIELD_DEFINE(av::gua::MFTriMeshLoader);

av::gua::TriMeshLoader::TriMeshLoader(::gua::TriMeshLoader* guaTriMeshLoader)
    : m_guaTriMeshLoader(guaTriMeshLoader)
{}

//av::gua::TriMeshLoader::~TriMeshLoader()
//{}

av::Link<av::gua::Node>
av::gua::TriMeshLoader::createGeometryFromFile(std::string const& nodeName,
                                               std::string const& fileName,
                                               av::Link<av::gua::Material> const& fallbackMaterial,
                                               Flags flags) const
{

    auto gua_node(m_guaTriMeshLoader->create_geometry_from_file(
                                            nodeName, fileName, fallbackMaterial->getGuaMaterial(), flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

void
av::gua::TriMeshLoader::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::TriMeshLoader, true);

        SFTriMeshLoader::initClass("av::gua::SFTriMeshLoader", "av::Field");
        MFTriMeshLoader::initClass("av::gua::MFTriMeshLoader", "av::Field");
    }
}

::gua::TriMeshLoader*
av::gua::TriMeshLoader::getGuaTriMeshLoader() const
{
    return m_guaTriMeshLoader;
}

av::gua::Node*
av::gua::TriMeshLoader::createChildren(std::shared_ptr< ::gua::node::Node> root) const {

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
    auto geom_cast(std::dynamic_pointer_cast< ::gua::node::TriMeshNode>(root));
    if (geom_cast) {
      av_node = new av::gua::TriMeshNode(geom_cast);
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
