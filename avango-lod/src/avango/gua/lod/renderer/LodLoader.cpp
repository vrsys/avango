#include <avango/gua/lod/renderer/LodLoader.hpp>
#include <avango/gua/lod/scenegraph/PLodNode.hpp>
#include <avango/gua/lod/scenegraph/MLodNode.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::gua::lod::LodLoader"));
}

AV_FC_DEFINE(av::gua::lod::LodLoader);

AV_FIELD_DEFINE(av::gua::lod::SFLodLoader);
AV_FIELD_DEFINE(av::gua::lod::MFLodLoader);

av::gua::lod::LodLoader::LodLoader(::gua::LodLoader* guaLodLoader) : m_guaLodLoader(guaLodLoader)
{
    AV_FC_ADD_ADAPTOR_FIELD(UploadBudget, std::bind(&LodLoader::getUploadBudgetCB, this, std::placeholders::_1), std::bind(&LodLoader::setUploadBudgetCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(RenderBudget, std::bind(&LodLoader::getRenderBudgetCB, this, std::placeholders::_1), std::bind(&LodLoader::setRenderBudgetCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(OutOfCoreBudget, std::bind(&LodLoader::getOutOfCoreBudgetCB, this, std::placeholders::_1), std::bind(&LodLoader::setOutOfCoreBudgetCB, this, std::placeholders::_1));
}

// av::gua::lod::LodLoader::~LodLoader()
//{}

av::Link<av::gua::Node> av::gua::lod::LodLoader::loadLodPointcloud(std::string const& nodeName, std::string const& fileName, av::Link<av::gua::Material> const& fallbackMaterial, Flags flags) const
{
    auto gua_node(m_guaLodLoader->load_lod_pointcloud(nodeName, fileName, fallbackMaterial->getGuaMaterial(), flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

av::Link<av::gua::Node> av::gua::lod::LodLoader::loadLodPointcloud(std::string const& fileName, Flags flags) const
{
    auto gua_node(m_guaLodLoader->load_lod_pointcloud(fileName, flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

av::Link<av::gua::Node> av::gua::lod::LodLoader::loadLodTrimesh(std::string const& nodeName, std::string const& fileName, av::Link<av::gua::Material> const& fallbackMaterial, Flags flags) const
{
    auto gua_node(m_guaLodLoader->load_lod_trimesh(nodeName, fileName, fallbackMaterial->getGuaMaterial(), flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

av::Link<av::gua::Node> av::gua::lod::LodLoader::loadLodTrimesh(std::string const& fileName, Flags flags) const
{
    auto gua_node(m_guaLodLoader->load_lod_trimesh(fileName, flags));
    auto root(createChildren(gua_node));

    return av::Link<av::gua::Node>(root);
}

void av::gua::lod::LodLoader::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::lod::LodLoader, true);

        SFLodLoader::initClass("av::gua::SFLodLoader", "av::Field");
        MFLodLoader::initClass("av::gua::MFLodLoader", "av::Field");
    }
}

::gua::LodLoader* av::gua::lod::LodLoader::getGuaLodLoader() const { return m_guaLodLoader; }

av::gua::Node* av::gua::lod::LodLoader::createChildren(std::shared_ptr<::gua::node::Node> root) const
{
    av::gua::Node* av_node(nullptr);

    auto group_cast(std::dynamic_pointer_cast<::gua::node::TransformNode>(root));
    if(group_cast)
    {
        av_node = new av::gua::TransformNode(group_cast);
    }

    // if (!av_node) {
    //   auto vol_cast(std::dynamic_pointer_cast< ::gua::node::VolumeNode>(root));
    //   if (vol_cast) {
    //     av_node = new av::gua::VolumeNode(vol_cast);
    //   }
    // }

    if(!av_node)
    {
        auto geom_cast(std::dynamic_pointer_cast<::gua::node::PLodNode>(root));
        if(geom_cast)
        {
            av_node = new av::gua::lod::PLodNode(geom_cast);
        }
    }

    if(!av_node)
    {
        auto geom_cast(std::dynamic_pointer_cast<::gua::node::MLodNode>(root));
        if(geom_cast)
        {
            av_node = new av::gua::lod::MLodNode(geom_cast);
        }
    }

    if(av_node)
    {
        av_node->addToParentChildren();
    }
    else
    {
        std::cout << "Unexpected node type encountered while loading geometry!" << std::endl;
    }

    for(auto c : root->get_children())
    {
        createChildren(c);
    }

    return av_node;
}

void av::gua::lod::LodLoader::setUploadBudgetCB(const SFInt::SetValueEvent& event) { m_guaLodLoader->set_upload_budget_in_mb(event.getValue()); }

void av::gua::lod::LodLoader::setRenderBudgetCB(const SFInt::SetValueEvent& event) { m_guaLodLoader->set_render_budget_in_mb(event.getValue()); }

void av::gua::lod::LodLoader::setOutOfCoreBudgetCB(const SFInt::SetValueEvent& event) { m_guaLodLoader->set_out_of_core_budget_in_mb(event.getValue()); }

void av::gua::lod::LodLoader::getUploadBudgetCB(const SFInt::GetValueEvent& event) { *(event.getValuePtr()) = m_guaLodLoader->get_upload_budget_in_mb(); }

void av::gua::lod::LodLoader::getRenderBudgetCB(const SFInt::GetValueEvent& event) { *(event.getValuePtr()) = m_guaLodLoader->get_render_budget_in_mb(); }

void av::gua::lod::LodLoader::getOutOfCoreBudgetCB(const SFInt::GetValueEvent& event) { *(event.getValuePtr()) = m_guaLodLoader->get_out_of_core_budget_in_mb(); }

std::pair<std::string, ::gua::math::vec3> av::gua::lod::LodLoader::pick_plod_bvh(
    ::gua::math::vec3 const& ray_origin, ::gua::math::vec3 const& ray_forward, float max_distance, std::set<std::string> const& model_filenames, float aabb_scale) const
{
    return m_guaLodLoader->pick_lod_bvh(ray_origin, ray_forward, max_distance, model_filenames, aabb_scale);
}

av::gua::MFPickResult* av::gua::lod::LodLoader::pick_plod_interpolate(::gua::math::vec3 const& bundle_origin,
                                                                      ::gua::math::vec3 const& bundle_forward,
                                                                      ::gua::math::vec3 const& bundle_up,
                                                                      float bundle_radius,
                                                                      float max_distance,
                                                                      unsigned int max_depth,
                                                                      unsigned int surfel_skip,
                                                                      float aabb_scale) const
{
    auto gua_results = m_guaLodLoader->pick_lod_interpolate(bundle_origin, bundle_forward, bundle_up, bundle_radius, max_distance, max_depth, surfel_skip, aabb_scale);

    auto results(new av::gua::MFPickResult());
    for(auto result : gua_results)
    {
        results->add1Value(new av::gua::PickResult(result));
    }

    return results;
}
