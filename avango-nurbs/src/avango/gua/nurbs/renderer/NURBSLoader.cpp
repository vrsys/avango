#include <avango/gua/nurbs/renderer/NURBSLoader.hpp>
#include <avango/gua/nurbs/scenegraph/NURBSNode.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::nurbs::NURBSLoader"));
}

AV_FC_DEFINE(av::gua::nurbs::NURBSLoader);

AV_FIELD_DEFINE(av::gua::nurbs::SFNURBSLoader);
AV_FIELD_DEFINE(av::gua::nurbs::MFNURBSLoader);

av::gua::nurbs::NURBSLoader::NURBSLoader(::gua::NURBSLoader* guaNURBSLoader)
    : m_guaNURBSLoader(guaNURBSLoader)
{}

av::Link<av::gua::Node>
av::gua::nurbs::NURBSLoader::load( std::string const& nodeName,
                           std::string const& fileName,
                           av::gua::Material const& fallbackMaterial,
                           Flags flags) const
{

    auto gua_node(m_guaNURBSLoader->load_geometry(nodeName, fileName, fallbackMaterial.getGuaMaterial(), flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}




/*
std::pair<std::string, ::gua::math::vec3>
av::gua::nurbs::PLODLoader::pick_plod_bvh(::gua::math::vec3 const& ray_origin,
                                               ::gua::math::vec3 const& ray_forward,
                                               float max_distance,
                                               std::set<std::string> const& model_filenames,
                                               float aabb_scale) const {

  return m_guaPLODLoader->pick_plod_bvh(ray_origin,
                                        ray_forward,
                                        max_distance,
                                        model_filenames,
                                        aabb_scale);

}

av::gua::nurbs::MFPickResult*
av::gua::nurbs::PLODLoader::pick_plod_interpolate(
                                      ::gua::math::vec3 const& bundle_origin,
                                      ::gua::math::vec3 const& bundle_forward,
                                      ::gua::math::vec3 const& bundle_up,
                                      float bundle_radius,
                                      float max_distance,
                                      unsigned int max_depth,
                                      unsigned int surfel_skip,
                                      float aabb_scale) const {

  auto gua_results = m_guaPLODLoader->pick_plod_interpolate(bundle_origin,
                                                bundle_forward,
                                                bundle_up,
                                                bundle_radius,
                                                max_distance,
                                                max_depth,
                                                surfel_skip,
                                                aabb_scale);

  auto results(new av::gua::nurbs::MFPickResult());
  for (auto result : gua_results) {
    results->add1Value(new av::gua::nurbs::PickResult(result));
  }

  return results;
}

*/

void
av::gua::nurbs::NURBSLoader::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::nurbs::NURBSLoader, true);

        SFNURBSLoader::initClass("av::gua::nurbs::SFNURBSLoader", "av::Field");
        MFNURBSLoader::initClass("av::gua::nurbs::MFNURBSLoader", "av::Field");
    }
}

::gua::NURBSLoader*
av::gua::nurbs::NURBSLoader::getGuaNURBSLoader() const
{
    return m_guaNURBSLoader;
}

av::gua::Node*
av::gua::nurbs::NURBSLoader::createChildren(std::shared_ptr< ::gua::node::Node> root) const {

  av::gua::Node* av_node(nullptr);

  auto group_cast(std::dynamic_pointer_cast< ::gua::node::TransformNode>(root));
  if (group_cast) {
    av_node = new av::gua::TransformNode(group_cast);
  }

  if (!av_node) {
    auto geom_cast(std::dynamic_pointer_cast< ::gua::node::NURBSNode>(root));
    if (geom_cast) {
      av_node = new av::gua::nurbs::NURBSNode(geom_cast);
    }
  }

  if (av_node) {
    av_node->addToParentChildren();
  } else {
    std::cout << "Unexpected node type encountered while loading geometry!" << std::endl;
  }

  for (auto c : root->get_children()) {
    createChildren(c);
  }

  return av_node;
}

/*
void
av::gua::nurbs::PLODLoader::getUploadBudgetCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODLoader->get_upload_budget_in_mb();
}

void
av::gua::nurbs::PLODLoader::setUploadBudgetCB(const SFInt::SetValueEvent& event)
{
  m_guaPLODLoader->set_upload_budget_in_mb(event.getValue());
}

void
av::gua::nurbs::PLODLoader::getRenderBudgetCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODLoader->get_render_budget_in_mb();
}

void
av::gua::nurbs::PLODLoader::setRenderBudgetCB(const SFInt::SetValueEvent& event)
{
  m_guaPLODLoader->set_render_budget_in_mb(event.getValue());
}

void
av::gua::nurbs::PLODLoader::getOutOfCoreBudgetCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODLoader->get_out_of_core_budget_in_mb();
}

void
av::gua::nurbs::PLODLoader::setOutOfCoreBudgetCB(const SFInt::SetValueEvent& event)
{
  m_guaPLODLoader->set_out_of_core_budget_in_mb(event.getValue());
}
*/
