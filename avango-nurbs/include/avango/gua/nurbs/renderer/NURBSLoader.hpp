#ifndef AVANGO_GUA_NURBS_LOADER_HPP
#define AVANGO_GUA_NURBS_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/NURBSLoader.hpp>
#include <gua/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/nurbs/scenegraph/NURBSNode.hpp>
#include <avango/gua/renderer/Material.hpp>
#include <avango/FieldContainer.h>
#include <avango/gua/scenegraph/PickResult.hpp>
#include <avango/gua/windows_specific_gua.hpp>


namespace av
{
  namespace gua
  {
    namespace nurbs
    {
   /**
    * Wrapper for ::gua::NURBSLoader
    *
    * \ingroup av_gua
    */
    class AV_GUA_DLL NURBSLoader : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      enum Flags {
        DEFAULTS = ::gua::TV_3Loader::DEFAULTS,
        MAKE_PICKABLE = ::gua::TV_3Loader::MAKE_PICKABLE,
        NORMALIZE_SCALE = ::gua::TV_3Loader::NORMALIZE_SCALE,
        NORMALIZE_POSITION = ::gua::TV_3Loader::NORMALIZE_POSITION,
        USE_SURFACE_MODE = ::gua::TV_3Loader::USE_SURFACE_MODE
      };

   /**
    * Constructor. When called without arguments, a new ::gua::TV_3Loader is created.
    * Otherwise, the given ::gua::TV_3Loader is used.
    */
      NURBSLoader(::gua::NURBSLoader* guaNURBSLoader = new ::gua::NURBSLoader());

      av::Link<av::gua::Node> load( std::string const& nodeName,
                                    std::string const& fileName,
                                    Flags flags = DEFAULTS,
                                    int const cpu_budget = 1024,
                                    int const gpu_budget = 1024) const;
      av::Link<av::gua::Node> load( std::string const& nodeName,
                                    std::string const& fileName,
                                    av::gua::Material const& fallbackMaterial,
                                    Flags flags = DEFAULTS,
                                    int const cpu_budget = 1024,
                                    int const gpu_budget = 1024) const;
      bool is_supported(std::string const& fileName) const;

  /*
      std::pair<std::string, ::gua::math::vec3> pick_plod_bvh(
                                ::gua::math::vec3 const& ray_origin,
                                ::gua::math::vec3 const& ray_forward,
                                float max_distance,
                                std::set<std::string> const& model_filenames,
                                float aabb_scale) const;

      av::gua::MFPickResult* pick_plod_interpolate(
                                    ::gua::math::vec3 const& bundle_origin,
                                    ::gua::math::vec3 const& bundle_forward,
                                    ::gua::math::vec3 const& bundle_up,
                                    float bundle_radius,
                                    float max_distance,
                                    unsigned int max_depth,
                                    unsigned int surfel_skip,
                                    float aabb_scale) const;
  */
    public:
  /*
        SFInt   UploadBudget;
        SFInt   RenderBudget;
        SFInt   OutOfCoreBudget;
  */
        /**
         * Get the wrapped ::gua::NURBSLoader.
         */
        ::gua::NURBSLoader* getGuaNURBSLoader() const;
  /*
        virtual void getUploadBudgetCB(const SFInt::GetValueEvent& event);
        virtual void setUploadBudgetCB(const SFInt::SetValueEvent& event);
        virtual void getRenderBudgetCB(const SFInt::GetValueEvent& event);
        virtual void setRenderBudgetCB(const SFInt::SetValueEvent& event);
        virtual void getOutOfCoreBudgetCB(const SFInt::GetValueEvent& event);
        virtual void setOutOfCoreBudgetCB(const SFInt::SetValueEvent& event);
  */
      private:

        ::gua::NURBSLoader *m_guaNURBSLoader;

        av::gua::Node* createChildren(std::shared_ptr< ::gua::node::Node> root) const;

        NURBSLoader(const NURBSLoader&);
        NURBSLoader& operator=(const NURBSLoader&);
      };

      using SFNURBSLoader = SingleField<Link<NURBSLoader> >;
      using MFNURBSLoader = MultiField<Link<NURBSLoader> >;

    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::NURBSLoader> >;
  template class AV_GUA_DLL MultiField<Link<gua::NURBSLoader> >;
#endif

}

#endif //AVANGO_GUA_TV_3_LOADER_HPP
