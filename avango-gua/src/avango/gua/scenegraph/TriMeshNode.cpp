#include <avango/gua/scenegraph/TriMeshNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::TriMeshNode);

AV_FIELD_DEFINE(av::gua::SFTriMeshNode);
AV_FIELD_DEFINE(av::gua::MFTriMeshNode);

av::gua::TriMeshNode::TriMeshNode(std::shared_ptr< ::gua::TriMeshNode> guanode)
    : GeometryNode(guanode)
{
}

//av::gua::TriMeshNode::~TriMeshNode()
//{}

void
av::gua::TriMeshNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::GeometryNode::initClass();

        AV_FC_INIT(av::gua::GeometryNode, av::gua::TriMeshNode, true);

        SFTriMeshNode::initClass("av::gua::SFTriMeshNode", "av::Field");
        MFTriMeshNode::initClass("av::gua::MFTriMeshNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}
