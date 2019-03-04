#ifndef AVANGO_GUA_LOD_LOADER_HPP
#define AVANGO_GUA_LOD_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/LodLoader.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/lod/scenegraph/PLodNode.hpp>
#include <avango/gua/lod/scenegraph/MLodNode.hpp>
#include <avango/gua/scenegraph/Node.hpp>
#include <avango/gua/renderer/Material.hpp>
#include <avango/FieldContainer.h>
#include <avango/gua/scenegraph/PickResult.hpp>

#include <avango/gua/windows_specific_gua.hpp>

namespace av
{
namespace gua
{
namespace lod
{
/**
 * Wrapper for ::gua::LodLoader
 *
 * \ingroup av_gua
 */
class AV_GUA_LOD_DLL LodLoader : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    enum Flags
    {
        DEFAULTS = ::gua::LodLoader::DEFAULTS,
        MAKE_PICKABLE = ::gua::LodLoader::MAKE_PICKABLE,
        NORMALIZE_SCALE = ::gua::LodLoader::NORMALIZE_SCALE,
        NORMALIZE_POSITION = ::gua::LodLoader::NORMALIZE_POSITION
    };

    /**
     * Constructor. When called without arguments, a new ::gua::LodLoader is created.
     * Otherwise, the given ::gua::LodLoader is used.
     */
    LodLoader(::gua::LodLoader* guaLodLoader = new ::gua::LodLoader());

    av::Link<av::gua::Node> loadLodPointcloud(std::string const& nodeName, std::string const& fileName, av::Link<av::gua::Material> const& fallbackMaterial, Flags flags = DEFAULTS) const;

    av::Link<av::gua::Node> loadLodPointcloud(std::string const& fileName, Flags flags = DEFAULTS) const;

    av::Link<av::gua::Node> loadLodTrimesh(std::string const& nodeName, std::string const& fileName, av::Link<av::gua::Material> const& fallbackMaterial, Flags flags = DEFAULTS) const;

    av::Link<av::gua::Node> loadLodTrimesh(std::string const& fileName, Flags flags = DEFAULTS) const;

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    //      virtual ~LodLoader();

  public:
    /**
     * Get the wrapped ::gua::LodLoader.
     */
    ::gua::LodLoader* getGuaLodLoader() const;

    SFInt UploadBudget;
    SFInt RenderBudget;
    SFInt OutOfCoreBudget;

    virtual void setUploadBudgetCB(const SFInt::SetValueEvent& event);
    virtual void setRenderBudgetCB(const SFInt::SetValueEvent& event);
    virtual void setOutOfCoreBudgetCB(const SFInt::SetValueEvent& event);
    virtual void getUploadBudgetCB(const SFInt::GetValueEvent& event);
    virtual void getRenderBudgetCB(const SFInt::GetValueEvent& event);
    virtual void getOutOfCoreBudgetCB(const SFInt::GetValueEvent& event);

  private:
    ::gua::LodLoader* m_guaLodLoader;

    av::gua::Node* createChildren(std::shared_ptr<::gua::node::Node> root) const;

    LodLoader(const LodLoader&);
    LodLoader& operator=(const LodLoader&);

  public:
    std::pair<std::string, ::gua::math::vec3>
    pick_plod_bvh(::gua::math::vec3 const& ray_origin, ::gua::math::vec3 const& ray_forward, float max_distance, std::set<std::string> const& model_filenames, float aabb_scale) const;

    av::gua::MFPickResult* pick_plod_interpolate(::gua::math::vec3 const& bundle_origin,
                                                 ::gua::math::vec3 const& bundle_forward,
                                                 ::gua::math::vec3 const& bundle_up,
                                                 float bundle_radius,
                                                 float max_distance,
                                                 unsigned int max_depth,
                                                 unsigned int surfel_skip,
                                                 float aabb_scale) const;
};

typedef SingleField<Link<LodLoader>> SFLodLoader;
typedef MultiField<Link<LodLoader>> MFLodLoader;

} // namespace lod
} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::LodLoader>>;
template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::LodLoader>>;
#endif

} // namespace av

#endif // AVANGO_GUA_LOD_LOADER_HPP
