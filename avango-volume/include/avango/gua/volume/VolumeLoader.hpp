#ifndef AVANGO_GUA_VOLUME_LOADER_HPP
#define AVANGO_GUA_VOLUME_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/volume/VolumeLoader.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/volume/VolumeNode.hpp>
#include <avango/FieldContainer.h>

#include <avango/gua/windows_specific_gua.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::VolumeLoader
 *
 * \ingroup av_gua
 */
class AV_GUA_VOLUME_DLL VolumeLoader : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    enum Flags
    {
        DEFAULTS = ::gua::VolumeLoader::DEFAULTS,
        MAKE_PICKABLE = ::gua::VolumeLoader::MAKE_PICKABLE,
        NORMALIZE_POSITION = ::gua::VolumeLoader::NORMALIZE_POSITION,
        NORMALIZE_SCALE = ::gua::VolumeLoader::NORMALIZE_SCALE
    };

    /**
     * Constructor. When called without arguments, a new ::gua::VolumeLoader is created.
     * Otherwise, the given ::gua::VolumeLoader is used.
     */
    VolumeLoader(::gua::VolumeLoader* guaVolumeLoader = new ::gua::VolumeLoader());

    av::Link<av::gua::Node> load(std::string const& nodename, std::string const& fileName, Flags flags = DEFAULTS) const;

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    //      virtual ~VolumeLoader();

  public:
    /**
     * Get the wrapped ::gua::VolumeLoader.
     */
    ::gua::VolumeLoader* getGuaVolumeLoader() const;

  private:
    ::gua::VolumeLoader* m_guaVolumeLoader;

    av::gua::Node* createChildren(std::shared_ptr<::gua::node::Node> root) const;

    VolumeLoader(const VolumeLoader&);
    VolumeLoader& operator=(const VolumeLoader&);
};

using SFVolumeLoader = SingleField<Link<VolumeLoader>>;
using MFVolumeLoader = MultiField<Link<VolumeLoader>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_VOLUME_DLL SingleField<Link<gua::VolumeLoader>>;
template class AV_GUA_VOLUME_DLL MultiField<Link<gua::VolumeLoader>>;
#endif

} // namespace av

#endif // AVANGO_GUA_VOLUME_LOADER_HPP
