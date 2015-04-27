#include <avango/gua/renderer/PLODLoader.hpp>
#include <avango/gua/scenegraph/PLODNode.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::PLODLoader"));
}

AV_FC_DEFINE(av::gua::PLODLoader);

AV_FIELD_DEFINE(av::gua::SFPLODLoader);
AV_FIELD_DEFINE(av::gua::MFPLODLoader);

av::gua::PLODLoader::PLODLoader(::gua::PLODLoader* guaPLODLoader)
    : m_guaPLODLoader(guaPLODLoader)
{
  AV_FC_ADD_ADAPTOR_FIELD(UploadBudget,
                      boost::bind(&PLODLoader::getUploadBudgetCB, this, _1),
                      boost::bind(&PLODLoader::setUploadBudgetCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(RenderBudget,
                      boost::bind(&PLODLoader::getRenderBudgetCB, this, _1),
                      boost::bind(&PLODLoader::setRenderBudgetCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(OutOfCoreBudget,
                      boost::bind(&PLODLoader::getOutOfCoreBudgetCB, this, _1),
                      boost::bind(&PLODLoader::setOutOfCoreBudgetCB, this, _1));
}

//av::gua::PLODLoader::~PLODLoader()
//{}

av::Link<av::gua::Node>
av::gua::PLODLoader::load(std::string const& fileName,
                          av::gua::PLODLoader::Flags flags) const
{

    auto gua_node(m_guaPLODLoader->load_geometry(fileName, flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

av::Link<av::gua::Node>
av::gua::PLODLoader::load( std::string const& nodeName,
                           std::string const& fileName,
                           av::gua::Material const& fallbackMaterial,
                           Flags flags) const
{

    auto gua_node(m_guaPLODLoader->load_geometry(
                                          nodeName, fileName,
                                          fallbackMaterial.getGuaMaterial(),
                                          flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}


bool
av::gua::PLODLoader::is_supported(std::string const& fileName) const
{
  return m_guaPLODLoader->is_supported(fileName);
}

std::pair<std::string, ::gua::math::vec3>
av::gua::PLODLoader::pick_plod_bvh(::gua::math::vec3 const& ray_origin,
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

av::gua::MFPickResult*
av::gua::PLODLoader::pick_plod_interpolate(
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

  auto results(new av::gua::MFPickResult());
  for (auto result : gua_results) {
    results->add1Value(new av::gua::PickResult(result));
  }

  return results;
}


void
av::gua::PLODLoader::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::PLODLoader, true);

        SFPLODLoader::initClass("av::gua::SFPLODLoader", "av::Field");
        MFPLODLoader::initClass("av::gua::MFPLODLoader", "av::Field");
    }
}

::gua::PLODLoader*
av::gua::PLODLoader::getGuaPLODLoader() const
{
    return m_guaPLODLoader;
}

av::gua::Node*
av::gua::PLODLoader::createChildren(std::shared_ptr< ::gua::node::Node> root) const {

  av::gua::Node* av_node(nullptr);

  auto group_cast(std::dynamic_pointer_cast< ::gua::node::TransformNode>(root));
  if (group_cast) {
    av_node = new av::gua::TransformNode(group_cast);
  }

  if (!av_node) {
    auto geom_cast(std::dynamic_pointer_cast< ::gua::node::PLODNode>(root));
    if (geom_cast) {
      av_node = new av::gua::PLODNode(geom_cast);
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

void
av::gua::PLODLoader::getUploadBudgetCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODLoader->get_upload_budget_in_mb();
}

void
av::gua::PLODLoader::setUploadBudgetCB(const SFInt::SetValueEvent& event)
{
  m_guaPLODLoader->set_upload_budget_in_mb(event.getValue());
}

void
av::gua::PLODLoader::getRenderBudgetCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODLoader->get_render_budget_in_mb();
}

void
av::gua::PLODLoader::setRenderBudgetCB(const SFInt::SetValueEvent& event)
{
  m_guaPLODLoader->set_render_budget_in_mb(event.getValue());
}

void
av::gua::PLODLoader::getOutOfCoreBudgetCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODLoader->get_out_of_core_budget_in_mb();
}

void
av::gua::PLODLoader::setOutOfCoreBudgetCB(const SFInt::SetValueEvent& event)
{
  m_guaPLODLoader->set_out_of_core_budget_in_mb(event.getValue());
}

