#ifndef AVANGO_GUA_LOD_AUX_HPP
#define AVANGO_GUA_LOD_AUX_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Auxi.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

#include <avango/gua/lod/renderer/OctreeNode.hpp>
#include <avango/gua/lod/renderer/AuxFeature.hpp>
#include <avango/gua/lod/renderer/AuxAtlas.hpp>
#include <avango/gua/lod/renderer/AuxAtlasTile.hpp>
#include <avango/gua/lod/renderer/AuxSparsePoint.hpp>
#include <avango/gua/lod/renderer/AuxView.hpp>

namespace av
{
  namespace gua
  {

    namespace lod {
      /**
       * Wrapper for ::gua::Auxi
       *
       * \ingroup av_gua
       */
      class AV_GUA_LOD_DLL Auxi : public av::FieldContainer
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::Aux is created.
         * Otherwise, the given ::gua::Auxi is used.
         */
        Auxi(std::shared_ptr< ::gua::Auxi> guanode =
            std::shared_ptr< ::gua::Auxi>(new ::gua::Auxi()));
            

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
  //      virtual ~Aux();

      public:
        /**
         * Get the wrapped ::gua::Auxi.
         */
        std::shared_ptr< ::gua::Auxi> getGuaAux() const;


        void load_aux_file(std::string const& filename);

        const std::string get_filename() const;

        const uint32_t      get_num_views() const;
        const uint64_t      get_num_sparse_points() const;
        const uint32_t      get_num_atlas_tiles() const;
        const uint64_t      get_num_nodes() const;

        av::Link<av::gua::lod::AuxAtlas> get_atlas() const;

        av::Link<av::gua::lod::AuxAtlasTile> get_atlas_tile(uint32_t id) const;
        av::Link<av::gua::lod::AuxSparsePoint> get_sparse_point(uint32_t id) const;

        av::Link<av::gua::lod::AuxView> get_view(uint32_t id) const;

        const uint64_t get_octree_query(::gua::math::vec3 const& pos) const;
        av::Link<av::gua::lod::OctreeNode> get_octree_node(uint32_t node_id) const;



      private:

        std::shared_ptr< ::gua::Auxi> m_guaAux;

        Auxi(const Auxi&);
        Auxi& operator=(const Auxi&);
      };

      typedef SingleField<Link<Auxi> > SFAuxi;
      typedef MultiField<Link<Auxi> > MFAuxi;
    }

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::Auxi> >;
  template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::Auxi> >;
#endif

}

#endif //AVANGO_GUA_LOD_AUX_HPP
