#ifndef AVANGO_GUA_SCENEGRAPH_HPP
#define AVANGO_GUA_SCENEGRAPH_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/SceneGraph.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::SceneGraph
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL SceneGraph : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::Node is created.
     * Otherwise, the given ::gua::Node is used.
     */
    SceneGraph(::gua::SceneGraph* guaSceneGraph = new ::gua::SceneGraph());

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    // virtual ~SceneGraph();

  public:
    /**
     * Children field is read only!
     */
    SingleField<Link<Node>> Root;

    SFString Name;

    void updateCache() const;

    /**
     * Get the wrapped ::gua::Node.
     */
    ::gua::SceneGraph* getGuaSceneGraph() const;

  public:
    virtual void getRootCB(const SingleField<Link<Node>>::GetValueEvent& event);
    virtual void setRootCB(const SingleField<Link<Node>>::SetValueEvent& event);

    virtual void getNameCB(const SFString::GetValueEvent& event);
    virtual void setNameCB(const SFString::SetValueEvent& event);

  private:
    ::gua::SceneGraph* m_guaSceneGraph;
    av::Link<av::gua::Node> m_root;

    SceneGraph(const SceneGraph&);
    SceneGraph& operator=(const SceneGraph&);
};

using SFSceneGraph = SingleField<Link<SceneGraph>>;
using MFSceneGraph = MultiField<Link<SceneGraph>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::SceneGraph>>;
template class AV_GUA_DLL MultiField<Link<gua::SceneGraph>>;
#endif

} // namespace av

#endif // AVANGO_GUA_SCENEGRAPH_HPP
