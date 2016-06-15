#include <avango/gua/scenegraph/SceneGraph.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::SceneGraph);

AV_FIELD_DEFINE(av::gua::SFSceneGraph);
AV_FIELD_DEFINE(av::gua::MFSceneGraph);

av::gua::SceneGraph::SceneGraph(::gua::SceneGraph* guaSceneGraph)
    : m_guaSceneGraph(guaSceneGraph),
      m_root(new av::gua::TransformNode(std::dynamic_pointer_cast< ::gua::node::TransformNode>((*m_guaSceneGraph)["/"])))
{
    AV_FC_ADD_ADAPTOR_FIELD(Root,
                          std::bind(&SceneGraph::getRootCB, this,std::placeholders::_1),
                          std::bind(&SceneGraph::setRootCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Name,
                          std::bind(&SceneGraph::getNameCB, this,std::placeholders::_1),
                          std::bind(&SceneGraph::setNameCB, this,std::placeholders::_1));
}

//av::gua::SceneGraph::~SceneGraph()
//{}

void
av::gua::SceneGraph::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::SceneGraph, true);

        SFSceneGraph::initClass("av::gua::SFSceneGraph", "av::Field");
        MFSceneGraph::initClass("av::gua::MFSceneGraph", "av::Field");
    }
}

void
av::gua::SceneGraph::updateCache() const
{
    m_guaSceneGraph->update_cache();
}

::gua::SceneGraph*
av::gua::SceneGraph::getGuaSceneGraph() const
{
    return m_guaSceneGraph;
}

/* virtual */ void
av::gua::SceneGraph::getRootCB(const av::gua::SFNode::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_root;
}

/* virtual */ void
av::gua::SceneGraph::setRootCB(const av::gua::SFNode::SetValueEvent& event)
{
    m_root = event.getValue();

    // auto gua_root(std::dynamic_pointer_cast< ::gua::Node>(m_root->getGuaNode()));
    if (m_root->getGuaNode())
      m_guaSceneGraph->set_root(m_root->getGuaNode());
}

/* virtual */ void
av::gua::SceneGraph::getNameCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaSceneGraph->get_name();
}

/* virtual */ void
av::gua::SceneGraph::setNameCB(const SFString::SetValueEvent& event)
{
    m_guaSceneGraph->set_name(event.getValue());
}
