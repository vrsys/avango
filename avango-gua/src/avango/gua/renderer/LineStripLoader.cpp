#include <avango/gua/renderer/LineStripLoader.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::gua::LineStripLoader"));
}

AV_FC_DEFINE(av::gua::LineStripLoader);

AV_FIELD_DEFINE(av::gua::SFLineStripLoader);
AV_FIELD_DEFINE(av::gua::MFLineStripLoader);

av::gua::LineStripLoader::LineStripLoader(::gua::LineStripLoader* guaLineStripLoader) : m_guaLineStripLoader(guaLineStripLoader) {}

// av::gua::LineStripLoader::~LineStripLoader()
//{}

av::Link<av::gua::Node> av::gua::LineStripLoader::createGeometryFromFile(std::string const& nodeName, std::string const& fileName, av::gua::Material const& fallbackMaterial, Flags flags) const
{
    auto gua_node(m_guaLineStripLoader->create_geometry_from_file(nodeName, fileName, fallbackMaterial.getGuaMaterial(), flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

av::Link<av::gua::Node> av::gua::LineStripLoader::createGeometryFromFile(std::string const& nodeName, std::string const& fileName, Flags flags) const
{
    auto gua_node(m_guaLineStripLoader->create_geometry_from_file(nodeName, fileName, flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

av::Link<av::gua::Node> av::gua::LineStripLoader::createEmptyGeometry(std::string const& nodeName, std::string const& emptyName, av::gua::Material const& fallbackMaterial, unsigned flags) const
{
    auto gua_node(m_guaLineStripLoader->create_empty_geometry(nodeName, emptyName, fallbackMaterial.getGuaMaterial(), flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

av::Link<av::gua::Node> av::gua::LineStripLoader::createEmptyGeometry(std::string const& nodeName, std::string const& emptyName, unsigned flags) const
{
    auto gua_node(m_guaLineStripLoader->create_empty_geometry(nodeName, emptyName, flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

void av::gua::LineStripLoader::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::LineStripLoader, true);

        SFLineStripLoader::initClass("av::gua::SFLineStripLoader", "av::Field");
        MFLineStripLoader::initClass("av::gua::MFLineStripLoader", "av::Field");
    }
}

::gua::LineStripLoader* av::gua::LineStripLoader::getGuaLineStripLoader() const { return m_guaLineStripLoader; }

av::gua::Node* av::gua::LineStripLoader::createChildren(std::shared_ptr<::gua::node::Node> root) const
{
    av::gua::Node* av_node(nullptr);

    auto group_cast(std::dynamic_pointer_cast<::gua::node::TransformNode>(root));
    if(group_cast)
    {
        av_node = new av::gua::TransformNode(group_cast);
    }

    // if (!av_node) {
    //   auto vol_cast(std::dynamic_pointer_cast< ::gua::node::VolumeNode>(root));
    //   if (vol_cast) {
    //     av_node = new av::gua::VolumeNode(vol_cast);
    //   }
    // }

    if(!av_node)
    {
        auto geom_cast(std::dynamic_pointer_cast<::gua::node::LineStripNode>(root));
        if(geom_cast)
        {
            av_node = new av::gua::LineStripNode(geom_cast);
        }
    }

    if(av_node)
    {
        av_node->addToParentChildren();
    }
    else
    {
        std::cout << "Unexpected node type encountered while loading geometry!" << std::endl;
    }

    for(auto c : root->get_children())
    {
        createChildren(c);
    }

    return av_node;
}
