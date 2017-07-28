#include <avango/gua/scenegraph/Node.hpp>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Node"));
}

AV_FC_DEFINE_ABSTRACT(av::gua::Node);

AV_FIELD_DEFINE(av::gua::SFNode);
AV_FIELD_DEFINE(av::gua::MFNode);

av::gua::Node::Node(std::shared_ptr< ::gua::node::Node> guanode)
  : av::FieldContainer(),
    m_guaNode(guanode),
    m_selfUserDataHandle(0),
    m_childrenUserDataHandle(0)
{

  m_selfUserDataHandle = guanode->add_user_data(this);

  auto avGuaChildren(new av::MultiField<av::Link<av::gua::Node>>::ContainerType());
  m_childrenUserDataHandle = guanode->add_user_data(avGuaChildren);

  guanode->on_world_transform_changed.connect([&](::gua::math::mat4 const&){
    WorldTransform.touch();
  });

  AV_FC_ADD_ADAPTOR_FIELD(Parent,
                        std::bind(&Node::getParentCB, this,std::placeholders::_1),
                        std::bind(&Node::setParentCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(Children,
                        std::bind(&Node::getChildrenCB, this,std::placeholders::_1),
                        std::bind(&Node::setChildrenCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(Transform,
                        std::bind(&Node::getTransformCB, this,std::placeholders::_1),
                        std::bind(&Node::setTransformCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(WorldTransform,
                        std::bind(&Node::getWorldTransformCB, this,std::placeholders::_1),
                        std::bind(&Node::setWorldTransformCB, this,std::placeholders::_1));
  WorldTransform.dontDistribute(true);

  AV_FC_ADD_ADAPTOR_FIELD(BoundingBox,
                        std::bind(&Node::getBoundingBoxCB, this,std::placeholders::_1),
                        std::bind(&Node::setBoundingBoxCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(DisplayBoundingBox,
                        std::bind(&Node::getDisplayBoundingBoxCB, this,std::placeholders::_1),
                        std::bind(&Node::setDisplayBoundingBoxCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(Depth,
                        std::bind(&Node::getDepthCB, this,std::placeholders::_1),
                        std::bind(&Node::setDepthCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(Path,
                        std::bind(&Node::getPathCB, this,std::placeholders::_1),
                        std::bind(&Node::setPathCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Tags,
                        std::bind(&Node::getTagsCB, this,std::placeholders::_1),
                        std::bind(&Node::setTagsCB, this,std::placeholders::_1));

}

av::gua::Node::~Node()
{
  if (m_guaNode) {
    auto avGuaChildren(static_cast<av::MultiField<av::Link<Node>>::ContainerType*>(m_guaNode->get_user_data(m_childrenUserDataHandle)));
    m_guaNode->add_user_data(nullptr);
    if (avGuaChildren) {
      avGuaChildren->clear();
      delete avGuaChildren;
    }
  }
}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
void av::gua::Node::on_distribute(av::gua::NetTransform& netNode)
{
}

void av::gua::Node::on_undistribute(av::gua::NetTransform& netNode)
{
}
#endif

void
av::gua::Node::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT_ABSTRACT(av::FieldContainer, av::gua::Node, true);

        SFNode::initClass("av::gua::SFNode", "av::Field");
        MFNode::initClass("av::gua::MFNode", "av::Field");
    }
}

std::shared_ptr< ::gua::node::Node>
av::gua::Node::getGuaNode() const
{
    return m_guaNode;
}

unsigned
av::gua::Node::getUserDataHandle() const
{
    return m_selfUserDataHandle;
}

/* virtual */ void
av::gua::Node::getParentCB(const av::gua::SFNode::GetValueEvent& event)
{
  if (m_guaNode->get_parent()) {
    if (m_guaNode->get_parent()->get_user_data(m_selfUserDataHandle)) {
      *(event.getValuePtr()) = static_cast<av::gua::Node*>(m_guaNode->get_parent()->get_user_data(m_selfUserDataHandle));
    }
  }
}

/* virtual */ void
av::gua::Node::setParentCB(const av::gua::SFNode::SetValueEvent& event)
{
    // std::cout << "Error in av::gua::Node::setParentCB: Parent is read only" << std::endl;
}

/* virtual */ void
av::gua::Node::getChildrenCB(const av::gua::MFNode::GetValueEvent& event)
{
    // av::MultiField<av::Link<Node>>::ContainerType children;
    // for (auto gua_child: m_guaNode->get_children()) {
    //   if (gua_child->get_user_data(m_selfUserDataHandle)) {
    //     children.push_back(av::Link<Node>(static_cast<av::gua::Node*>(gua_child->get_user_data(m_selfUserDataHandle))));
    //   }
    // }

    *(event.getValuePtr()) = *static_cast<av::MultiField<av::Link<Node>>::ContainerType*>(m_guaNode->get_user_data(m_childrenUserDataHandle));
}

/* virtual */ void
av::gua::Node::setChildrenCB(const av::gua::MFNode::SetValueEvent& event)
{
  if (m_guaNode) {

    m_guaNode->clear_children();
    auto avGuaChildren(static_cast<av::MultiField<av::Link<Node>>::ContainerType*>(m_guaNode->get_user_data(m_childrenUserDataHandle)));

    if (avGuaChildren) avGuaChildren->clear();

    const av::gua::MFNode::ContainerType &children(event.getValue());

    for (auto& child: children) {

      if (child->getGuaNode() != nullptr) {
        m_guaNode->add_child(child->getGuaNode());
        child->getField("Parent")->touch();

        if (avGuaChildren) {
          avGuaChildren->push_back(child);
        }
      }
    }

  } else {
    std::cout << "Cannot set children of null node!" << std::endl;
  }
}

void
av::gua::Node::getNameCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_name();
}

void
av::gua::Node::setNameCB(const SFString::SetValueEvent& event)
{
    m_guaNode->set_name(event.getValue());
}

void
av::gua::Node::getTransformCB(const SFMatrix::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_transform();
}

void
av::gua::Node::setTransformCB(const SFMatrix::SetValueEvent& event)
{
    m_guaNode->set_transform(event.getValue());
}

void
av::gua::Node::getWorldTransformCB(const SFMatrix::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_world_transform();
}

void
av::gua::Node::setWorldTransformCB(const SFMatrix::SetValueEvent& event)
{
  m_guaNode->set_world_transform(event.getValue());
}

void
av::gua::Node::getBoundingBoxCB(const SFBoundingBox::GetValueEvent& event)
{
    m_guaBbox = m_guaNode->get_bounding_box();
    av::Link<av::gua::BoundingBox> bbox(new av::gua::BoundingBox(&m_guaBbox));
    *(event.getValuePtr()) = bbox;
}

void
av::gua::Node::setBoundingBoxCB(const SFBoundingBox::SetValueEvent& event)
{
  // std::cout << "A node's bounding box is calculated dependent on the scenegraph hierarchy and thus cannot be set!" << std::endl;
}

void
av::gua::Node::getDisplayBoundingBoxCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_draw_bounding_box();
}

void
av::gua::Node::setDisplayBoundingBoxCB(const SFBool::SetValueEvent& event)
{
    m_guaNode->set_draw_bounding_box(event.getValue());
}

void
av::gua::Node::getDepthCB(const SFInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_depth();
}

void
av::gua::Node::setDepthCB(const SFInt::SetValueEvent& event)
{
  // std::cout << "A node's depth cannot be set!" << std::endl;
}

void
av::gua::Node::getPathCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_path();
}

void
av::gua::Node::setPathCB(const SFString::SetValueEvent& event)
{
  // std::cout << "A node's path cannot be set!" << std::endl;
}

void
av::gua::Node::getTagsCB(const MFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNode->get_tags().get_strings();
}

void
av::gua::Node::setTagsCB(const MFString::SetValueEvent& event)
{
  m_guaNode->get_tags().clear_tags();
  for (auto tag : event.getValue()) {
    m_guaNode->get_tags().add_tag(tag);
  }
}


void
av::gua::Node::addToParentChildren() {

  if (m_guaNode->get_parent()) {
    auto parent_children(static_cast<av::MultiField<
                                      av::Link<av::gua::Node>>::ContainerType*>(
                                        m_guaNode->get_parent()->get_user_data(m_childrenUserDataHandle)));

    auto link(av::Link<av::gua::Node>(this));
    parent_children->push_back(link);
  }

}

