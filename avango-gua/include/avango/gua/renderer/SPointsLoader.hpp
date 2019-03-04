#ifndef AVANGO_GUA_SPOINTS_LOADER_HPP
#define AVANGO_GUA_SPOINTS_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/spoints/SPointsLoader.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/scenegraph/SPointsNode.hpp>
#include <avango/FieldContainer.h>

#include <avango/gua/windows_specific_gua.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::SPointsLoader
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL SPointsLoader : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::SPointsLoader is created.
     * Otherwise, the given ::gua::SPointsLoader is used.
     */
    SPointsLoader(::gua::SPointsLoader* guaSPointsLoader = new ::gua::SPointsLoader());

    av::Link<av::gua::Node> load(std::string const& nodename, std::string const& fileName) const;

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    //      virtual ~SPointsLoader();

  public:
    /**
     * Get the wrapped ::gua::SPointsLoader.
     */
    ::gua::SPointsLoader* getGuaSPointsLoader() const;

  private:
    ::gua::SPointsLoader* m_guaSPointsLoader;

    av::gua::Node* createChildren(std::shared_ptr<::gua::node::Node> root) const;

    SPointsLoader(const SPointsLoader&);
    SPointsLoader& operator=(const SPointsLoader&);
};

using SFSPointsLoader = SingleField<Link<SPointsLoader>>;
using MFSPointsLoader = MultiField<Link<SPointsLoader>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::SPointsLoader>>;
template class AV_GUA_DLL MultiField<Link<gua::SPointsLoader>>;
#endif

} // namespace av

#endif // AVANGO_GUA_SPOINTS_LOADER_HPP
