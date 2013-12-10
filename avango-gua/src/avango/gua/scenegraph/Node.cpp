#include <avango/gua/scenegraph/Node.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Node"));
}

AV_FC_DEFINE_ABSTRACT(av::gua::Node);

AV_FIELD_DEFINE(av::gua::SFNode);
AV_FIELD_DEFINE(av::gua::MFNode);

av::gua::Node::Node(std::shared_ptr< ::gua::Node> guanode)
  : av::FieldContainer(),
    m_guaNode(guanode),
    m_selfUserDataHandle(0),
    m_childrenUserDataHandle(0)
{

  m_selfUserDataHandle = guanode->add_user_data(this);

  auto avGuaChildren(new av::MultiField<av::Link<av::gua::Node>>::ContainerType());
  m_childrenUserDataHandle = guanode->add_user_data(avGuaChildren);

  AV_FC_ADD_ADAPTOR_FIELD(Parent,
                        boost::bind(&Node::getParentCB, this, _1),
                        boost::bind(&Node::setParentCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Children,
                        boost::bind(&Node::getChildrenCB, this, _1),
                        boost::bind(&Node::setChildrenCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Name,
                        boost::bind(&Node::getNameCB, this, _1),
                        boost::bind(&Node::setNameCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Transform,
                        boost::bind(&Node::getTransformCB, this, _1),
                        boost::bind(&Node::setTransformCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(WorldTransform,
                        boost::bind(&Node::getWorldTransformCB, this, _1),
                        boost::bind(&Node::setWorldTransformCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(GroupNames,
                        boost::bind(&Node::getGroupNamesCB, this, _1),
                        boost::bind(&Node::setGroupNamesCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(BoundingBox,
                        boost::bind(&Node::getBoundingBoxCB, this, _1),
                        boost::bind(&Node::setBoundingBoxCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Depth,
                        boost::bind(&Node::getDepthCB, this, _1),
                        boost::bind(&Node::setDepthCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Path,
                        boost::bind(&Node::getPathCB, this, _1),
                        boost::bind(&Node::setPathCB, this, _1));

}

av::gua::Node::~Node()
{}

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

std::shared_ptr< ::gua::Node>
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
    std::cout << "Error in av::gua::Node::setParentCB: Parent is read only" << std::endl;
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
  std::cout << "A node's world transform cannot be set!" << std::endl;
}

void
av::gua::Node::getGroupNamesCB(const MFString::GetValueEvent& event)
{
    std::vector <std::string> v(m_guaNode->get_groups().begin(), m_guaNode->get_groups().end());
    *(event.getValuePtr()) = v;
}

void
av::gua::Node::setGroupNamesCB(const MFString::SetValueEvent& event)
{
  auto old_groups(m_guaNode->get_groups());
  for (auto group : old_groups) {
    m_guaNode->remove_from_group(group);
  }

  auto new_groups(event.getValue());
  for (auto group : new_groups) {
    m_guaNode->add_to_group(group);
  }
}

void
av::gua::Node::getBoundingBoxCB(const SFBoundingBox::GetValueEvent& event)
{
    auto gua_box(m_guaNode->get_bounding_box());
    av::Link<av::gua::BoundingBox> bbox(new av::gua::BoundingBox(&gua_box));
    *(event.getValuePtr()) = bbox;
}

void
av::gua::Node::setBoundingBoxCB(const SFBoundingBox::SetValueEvent& event)
{
  std::cout << "A node's bounding box is calculated dependent on the scenegraph hierarchy and thus cannot be set!" << std::endl;
}

void
av::gua::Node::getDepthCB(const SFInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_depth();
}

void
av::gua::Node::setDepthCB(const SFInt::SetValueEvent& event)
{
  std::cout << "A node's depth cannot be set!" << std::endl;
}

void
av::gua::Node::getPathCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNode->get_path();
}

void
av::gua::Node::setPathCB(const SFString::SetValueEvent& event)
{
  std::cout << "A node's path cannot be set!" << std::endl;
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

