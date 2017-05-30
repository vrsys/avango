#ifndef AVANGO_GUA_TV_3_LOADER_HPP
#define AVANGO_GUA_TV_3_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/TV_3Loader.hpp>
#include <gua/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/tv_3/scenegraph/TV_3Node.hpp>
#include <avango/gua/renderer/Material.hpp>
#include <avango/FieldContainer.h>
#include <avango/gua/scenegraph/PickResult.hpp>
#include <avango/gua/windows_specific_gua.hpp>


namespace av
{
  namespace gua
  {
    namespace tv_3 
    {
   /**
    * Wrapper for ::gua::TV_3Loader
    *
    * \ingroup av_gua
    */
    class AV_GUA_DLL TV_3Loader : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      enum Flags {
        DEFAULTS = ::gua::TV_3Loader::DEFAULTS,
        MAKE_PICKABLE = ::gua::TV_3Loader::MAKE_PICKABLE,
        NORMALIZE_SCALE = ::gua::TV_3Loader::NORMALIZE_SCALE,
        NORMALIZE_POSITION = ::gua::TV_3Loader::NORMALIZE_POSITION
      };

   /**
    * Constructor. When called without arguments, a new ::gua::TV_3Loader is created.
    * Otherwise, the given ::gua::TV_3Loader is used.
    */
      TV_3Loader(::gua::TV_3Loader* guaTV_3Loader = new ::gua::TV_3Loader());

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
         * Get the wrapped ::gua::TV_3Loader.
         */
        ::gua::TV_3Loader* getGuaTV_3Loader() const;
  /*
        virtual void getUploadBudgetCB(const SFInt::GetValueEvent& event);
        virtual void setUploadBudgetCB(const SFInt::SetValueEvent& event);
        virtual void getRenderBudgetCB(const SFInt::GetValueEvent& event);
        virtual void setRenderBudgetCB(const SFInt::SetValueEvent& event);
        virtual void getOutOfCoreBudgetCB(const SFInt::GetValueEvent& event);
        virtual void setOutOfCoreBudgetCB(const SFInt::SetValueEvent& event);
  */
      private:

        ::gua::TV_3Loader *m_guaTV_3Loader;

        av::gua::Node* createChildren(std::shared_ptr< ::gua::node::Node> root) const;

        TV_3Loader(const TV_3Loader&);
        TV_3Loader& operator=(const TV_3Loader&);
      };

      using SFTV_3Loader = SingleField<Link<TV_3Loader> >;
      using MFTV_3Loader = MultiField<Link<TV_3Loader> >;

    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TV_3Loader> >;
  template class AV_GUA_DLL MultiField<Link<gua::TV_3Loader> >;
#endif

}

#endif //AVANGO_GUA_TV_3_LOADER_HPP
