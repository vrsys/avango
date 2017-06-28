#include <avango/gua/tv_3/renderer/TV_3Loader.hpp>
#include <avango/gua/tv_3/scenegraph/TV_3Node.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::tv_3::TV_3Loader"));
}

AV_FC_DEFINE(av::gua::tv_3::TV_3Loader);

AV_FIELD_DEFINE(av::gua::tv_3::SFTV_3Loader);
AV_FIELD_DEFINE(av::gua::tv_3::MFTV_3Loader);

av::gua::tv_3::TV_3Loader::TV_3Loader(::gua::TV_3Loader* guaTV_3Loader)
    : m_guaTV_3Loader(guaTV_3Loader)
{
  /*
  AV_FC_ADD_ADAPTOR_FIELD(UploadBudget,
                      std::bind(&PLODLoader::getUploadBudgetCB, this,std::placeholders::_1),
                      std::bind(&PLODLoader::setUploadBudgetCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(RenderBudget,
                      std::bind(&PLODLoader::getRenderBudgetCB, this,std::placeholders::_1),
                      std::bind(&PLODLoader::setRenderBudgetCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(OutOfCoreBudget,
                      std::bind(&PLODLoader::getOutOfCoreBudgetCB, this,std::placeholders::_1),
                      std::bind(&PLODLoader::setOutOfCoreBudgetCB, this,std::placeholders::_1));
  */
}

//av::gua::tv_3::PLODLoader::~PLODLoader()
//{}

av::Link<av::gua::Node>
av::gua::tv_3::TV_3Loader::load( std::string const& nodeName,
                           std::string const& fileName,
                           Flags flags,
                           int const cpu_budget,
                           int const gpu_budget) const
{

    auto gua_node(m_guaTV_3Loader->create_geometry_from_file(
                                          nodeName, fileName,
                                          flags, cpu_budget, gpu_budget));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

av::Link<av::gua::Node>
av::gua::tv_3::TV_3Loader::load( std::string const& nodeName,
                           std::string const& fileName,
                           av::gua::Material const& fallbackMaterial,
                           Flags flags,
                           int const cpu_budget,
                           int const gpu_budget) const
{

    auto gua_node(m_guaTV_3Loader->create_geometry_from_file(
                                          nodeName, fileName,
                                          fallbackMaterial.getGuaMaterial(),
                                          flags, cpu_budget, gpu_budget));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}


bool
av::gua::tv_3::TV_3Loader::is_supported(std::string const& fileName) const
{
  return m_guaTV_3Loader->is_supported(fileName);
}


/*
std::pair<std::string, ::gua::math::vec3>
av::gua::tv_3::PLODLoader::pick_plod_bvh(::gua::math::vec3 const& ray_origin,
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

av::gua::tv_3::MFPickResult*
av::gua::tv_3::PLODLoader::pick_plod_interpolate(
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

  auto results(new av::gua::tv_3::MFPickResult());
  for (auto result : gua_results) {
    results->add1Value(new av::gua::tv_3::PickResult(result));
  }

  return results;
}

*/

void
av::gua::tv_3::TV_3Loader::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::tv_3::TV_3Loader, true);

        SFTV_3Loader::initClass("av::gua::tv_3::SFTV_3Loader", "av::Field");
        MFTV_3Loader::initClass("av::gua::tv_3::MFTV_3Loader", "av::Field");
    }
}

::gua::TV_3Loader*
av::gua::tv_3::TV_3Loader::getGuaTV_3Loader() const
{
    return m_guaTV_3Loader;
}

av::gua::Node*
av::gua::tv_3::TV_3Loader::createChildren(std::shared_ptr< ::gua::node::Node> root) const {

  av::gua::Node* av_node(nullptr);

  auto group_cast(std::dynamic_pointer_cast< ::gua::node::TransformNode>(root));
  if (group_cast) {
    av_node = new av::gua::TransformNode(group_cast);
  }

  if (!av_node) {
    auto geom_cast(std::dynamic_pointer_cast< ::gua::node::TV_3Node>(root));
    if (geom_cast) {
      av_node = new av::gua::tv_3::TV_3Node(geom_cast);
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
av::gua::tv_3::PLODLoader::getUploadBudgetCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODLoader->get_upload_budget_in_mb();
}

void
av::gua::tv_3::PLODLoader::setUploadBudgetCB(const SFInt::SetValueEvent& event)
{
  m_guaPLODLoader->set_upload_budget_in_mb(event.getValue());
}

void
av::gua::tv_3::PLODLoader::getRenderBudgetCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODLoader->get_render_budget_in_mb();
}

void
av::gua::tv_3::PLODLoader::setRenderBudgetCB(const SFInt::SetValueEvent& event)
{
  m_guaPLODLoader->set_render_budget_in_mb(event.getValue());
}

void
av::gua::tv_3::PLODLoader::getOutOfCoreBudgetCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODLoader->get_out_of_core_budget_in_mb();
}

void
av::gua::tv_3::PLODLoader::setOutOfCoreBudgetCB(const SFInt::SetValueEvent& event)
{
  m_guaPLODLoader->set_out_of_core_budget_in_mb(event.getValue());
}
*/
