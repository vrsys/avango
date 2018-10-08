#ifndef AVANGO_GUA_LOD_AUXATLAS_HPP
#define AVANGO_GUA_LOD_AUXATLAS_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/OctreeNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {

    namespace lod {
      /**
       * Wrapper for ::gua::OctreeNode
       *
       * \ingroup av_gua
       */
      class AV_GUA_LOD_DLL OctreeNode : public av::FieldContainer
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::OctreeNode is created.
         * Otherwise, the given ::gua::OctreeNode is used.
         */
        OctreeNode(std::shared_ptr< ::gua::OctreeNode> guanode =
            std::shared_ptr< ::gua::OctreeNode>(new ::gua::OctreeNode()));
            

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~OctreeNode();

      public:
        /**
         * Get the wrapped ::gua::OctreeNode.
         */
        std::shared_ptr< ::gua::OctreeNode> getGuaOctreeNode() const;


        SFUInt Idx;
        SFUInt ChildMask;
        SFUInt ChildIdx;
        SFVec3 Min;
        SFVec3 Max;
        

        virtual void getIdxCB(const SFUInt::GetValueEvent& event);
        
        virtual void getChildMaskCB(const SFUInt::GetValueEvent& event);
        
        virtual void getChildIdxCB(const SFUInt::GetValueEvent& event);
        
        virtual void getMinCB(const SFUInt::GetValueEvent& event);

        virtual void getMaxCB(const SFUInt::GetValueEvent& event);


        inline uint32_t getIdxCB() {
          if (m_guaOctreeNode)
            return m_guaOctreeNode->get_idx();
          else
            return 0;
        }

        inline uint32_t getChildMaskCB() {
          if (m_guaOctreeNode)
            return m_guaOctreeNode->get_child_mask();
          else
            return 0;
        }

        inline uint32_t getChildIdxCB() {
          if (m_guaOctreeNode)
            return m_guaOctreeNode->get_child_idx();
          else
            return 0;
        }
        inline uint32_t getMinCB() {
          if (m_guaOctreeNode)
            return m_guaOctreeNode->get_min();
          else
            return 0;
        }
        inline uint32_t getMinCB() {
          if (m_guaOctreeNode)
            return m_guaOctreeNode->get_max();
          else
            return 0;
        }


      private:
        std::shared_ptr< ::gua::OctreeNode> m_guaOctreeNode;

        OctreeNode(const OctreeNode&);
        OctreeNode& operator=(const OctreeNode&);
      };

      typedef SingleField<Link<OctreeNode> > SFOctreeNode;
      typedef MultiField<Link<OctreeNode> > MFOctreeNode;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::OctreeNode> >;
  template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::OctreeNode> >;
#endif

}

#endif //AVANGO_GUA_LOD_AUXATLAS_HPP
