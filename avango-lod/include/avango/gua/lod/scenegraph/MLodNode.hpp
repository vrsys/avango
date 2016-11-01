#ifndef AVANGO_GUA_LOD_MLOD_NODE_HPP
#define AVANGO_GUA_LOD_MLOD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/MLodNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {

    namespace lod {
      /**
       * Wrapper for ::gua::MLodNode
       *
       * \ingroup av_gua
       */
      class AV_GUA_ANIM_DLL MLodNode : public av::gua::GeometryNode
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::MLodNode is created.
         * Otherwise, the given ::gua::MLodNode is used.
         */
        MLodNode(std::shared_ptr< ::gua::node::MLodNode> guanode =
            std::shared_ptr< ::gua::node::MLodNode>(new ::gua::node::MLodNode("")));
      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~MLodNode();

      public:
        /**
         * Get the wrapped ::gua::MLodNode.
         */
        std::shared_ptr< ::gua::node::MLodNode> getGuaMLodNode() const;

      private:

        std::shared_ptr< ::gua::node::MLodNode> m_guaMLodNode;

        MLodNode(const MLodNode&);
        MLodNode& operator=(const MLodNode&);
      };

      typedef SingleField<Link<MLodNode> > SFMLodNode;
      typedef MultiField<Link<MLodNode> > MFMLodNode;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_ANIM_DLL SingleField<Link<gua::lod::MLodNode> >;
  template class AV_GUA_ANIM_DLL MultiField<Link<gua::skelanim::MLodNode> >;
#endif

}

#endif //AVANGO_GUA_LOD_MLOD_NODE_HPP
