#ifndef AVANGO_GUA_NURBS_LOADER_HPP
#define AVANGO_GUA_NURBS_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/NURBSLoader.hpp>
#include <gua/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/nurbs/Init.hpp>
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
class AV_NURBS_DLL NURBSLoader : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    enum Flags
    {
        DEFAULTS = ::gua::NURBSLoader::DEFAULTS,
        MAKE_PICKABLE = ::gua::NURBSLoader::MAKE_PICKABLE,
        NORMALIZE_SCALE = ::gua::NURBSLoader::NORMALIZE_SCALE,
        NORMALIZE_POSITION = ::gua::NURBSLoader::NORMALIZE_POSITION
    };

    /**
     * Constructor. When called without arguments, a new ::gua::NURBSLoader is created.
     * Otherwise, the given ::gua::NURBSLoader is used.
     */
    NURBSLoader(::gua::NURBSLoader* guaNURBSLoader = new ::gua::NURBSLoader());

    av::Link<av::gua::Node> load(std::string const& nodeName, std::string const& fileName, av::gua::Material const& fallbackMaterial, Flags flags = DEFAULTS) const;

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
    ::gua::NURBSLoader* m_guaNURBSLoader;

    av::gua::Node* createChildren(std::shared_ptr<::gua::node::Node> root) const;

    NURBSLoader(const NURBSLoader&);
    NURBSLoader& operator=(const NURBSLoader&);
};

using SFNURBSLoader = SingleField<Link<NURBSLoader>>;
using MFNURBSLoader = MultiField<Link<NURBSLoader>>;

} // namespace nurbs
} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_NURBS_DLL SingleField<Link<gua::nurbs::NURBSLoader>>;
template class AV_NURBS_DLL MultiField<Link<gua::nurbs::NURBSLoader>>;
#endif

} // namespace av

#endif // AVANGO_GUA_NURBS_LOADER_HPP
