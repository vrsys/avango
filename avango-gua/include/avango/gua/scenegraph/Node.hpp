#ifndef AVANGO_GUA_NODE_HPP
#define AVANGO_GUA_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/Node.hpp>
#include <gua/math/math.hpp>
#include <avango/gua/math/BoundingBox.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::Node
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Node : public av::FieldContainer
    {
      AV_FC_DECLARE_ABSTRACT();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::Node is created.
       * Otherwise, the given ::gua::Node is used.
       */
      Node(std::shared_ptr< ::gua::Node> guanode); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Node();

    public:


      SingleField<Link<Node>>   Parent;
      MultiField<Link<Node>>    Children;
      SFString                  Name;
      SFMatrix                  Transform;
      SFMatrix                  WorldTransform;
      MFString                  GroupNames;
      SFBoundingBox             BoundingBox;
      SFInt                     Depth;
      SFString                  Path;

      /**
       * Get the wrapped ::gua::Node.
       */
      std::shared_ptr< ::gua::Node> getGuaNode() const;
      unsigned     getUserDataHandle() const;


    public:

      virtual void getParentCB(const SingleField<Link<Node> >::GetValueEvent& event);
      virtual void setParentCB(const SingleField<Link<Node> >::SetValueEvent& event);

      virtual void getChildrenCB(const MultiField<Link<Node> >::GetValueEvent& event);
      virtual void setChildrenCB(const MultiField<Link<Node> >::SetValueEvent& event);

      virtual void getNameCB(const SFString::GetValueEvent& event);
      virtual void setNameCB(const SFString::SetValueEvent& event);

      virtual void getTransformCB(const SFMatrix::GetValueEvent& event);
      virtual void setTransformCB(const SFMatrix::SetValueEvent& event);

      virtual void getWorldTransformCB(const SFMatrix::GetValueEvent& event);
      virtual void setWorldTransformCB(const SFMatrix::SetValueEvent& event);

      virtual void getGroupNamesCB(const MFString::GetValueEvent& event);
      virtual void setGroupNamesCB(const MFString::SetValueEvent& event);

      virtual void getBoundingBoxCB(const SFBoundingBox::GetValueEvent& event);
      virtual void setBoundingBoxCB(const SFBoundingBox::SetValueEvent& event);

      virtual void getDepthCB(const SFInt::GetValueEvent& event);
      virtual void setDepthCB(const SFInt::SetValueEvent& event);

      virtual void getPathCB(const SFString::GetValueEvent& event);
      virtual void setPathCB(const SFString::SetValueEvent& event);

      void addToParentChildren();

    private:

      std::shared_ptr< ::gua::Node> m_guaNode;
      unsigned m_selfUserDataHandle;
      unsigned m_childrenUserDataHandle;

      Node(const Node&);
      Node& operator=(const Node&);
    };

    typedef SingleField<Link<Node> > SFNode;
    typedef MultiField<Link<Node> > MFNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Node> >;
  template class AV_GUA_DLL MultiField<Link<gua::Node> >;
#endif

}

#endif //AVANGO_GUA_NODE_HPP
