#ifndef AVANGO_GUA_LOD_PLOD_NODE_HPP
#define AVANGO_GUA_LOD_PLOD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/PLodNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {

    namespace lod {
      /**
       * Wrapper for ::gua::PLodNode
       *
       * \ingroup av_gua
       */
      class AV_GUA_ANIM_DLL PLodNode : public av::gua::GeometryNode
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::PLodNode is created.
         * Otherwise, the given ::gua::PLodNode is used.
         */
        PLodNode(std::shared_ptr< ::gua::node::PLodNode> guanode =
            std::shared_ptr< ::gua::node::PLodNode>(new ::gua::node::PLodNode("")));
      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~PLodNode();

      public:
        /**
         * Get the wrapped ::gua::PLodNode.
         */
        std::shared_ptr< ::gua::node::PLodNode> getGuaPLodNode() const;

      private:

        std::shared_ptr< ::gua::node::PLodNode> m_guaPLodNode;

        PLodNode(const PLodNode&);
        PLodNode& operator=(const PLodNode&);
      };

      typedef SingleField<Link<PLodNode> > SFPLodNode;
      typedef MultiField<Link<PLodNode> > MFPLodNode;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_ANIM_DLL SingleField<Link<gua::lod::PLodNode> >;
  template class AV_GUA_ANIM_DLL MultiField<Link<gua::skelanim::PLodNode> >;
#endif

}

#endif //AVANGO_GUA_LOD_PLOD_NODE_HPP
