// #include <avango/gua/renderer/PLODLoader.hpp>
// #include <avango/gua/scenegraph/PLODNode.hpp>
// #include <avango/gua/scenegraph/TransformNode.hpp>
// #include <avango/Base.h>
// #include <boost/bind.hpp>
// #include <avango/Logger.h>

// namespace
// {
//   av::Logger& logger(av::getLogger("av::gua::PLODLoader"));
// }

// AV_FC_DEFINE(av::gua::PLODLoader);

// AV_FIELD_DEFINE(av::gua::SFPLODLoader);
// AV_FIELD_DEFINE(av::gua::MFPLODLoader);

// av::gua::PLODLoader::PLODLoader(::gua::PLODLoader* guaPLODLoader)
//     : m_guaPLODLoader(guaPLODLoader)
// {
//   AV_FC_ADD_ADAPTOR_FIELD(UploadBudget,
//                       boost::bind(&PLODLoader::getUploadBudgetCB, this, _1),
//                       boost::bind(&PLODLoader::setUploadBudgetCB, this, _1));
//   AV_FC_ADD_ADAPTOR_FIELD(RenderBudget,
//                       boost::bind(&PLODLoader::getRenderBudgetCB, this, _1),
//                       boost::bind(&PLODLoader::setRenderBudgetCB, this, _1));
//   AV_FC_ADD_ADAPTOR_FIELD(OutOfCoreBudget,
//                       boost::bind(&PLODLoader::getOutOfCoreBudgetCB, this, _1),
//                       boost::bind(&PLODLoader::setOutOfCoreBudgetCB, this, _1));
// }

// //av::gua::PLODLoader::~PLODLoader()
// //{}

// av::Link<av::gua::Node>
// av::gua::PLODLoader::load(std::string const& nodename,
//                           std::string const& fileName,
//                           av::gua::PLODLoader::Flags flags) const
// {

//     auto gua_node(m_guaPLODLoader->create_geometry_from_file(nodename, fileName, flags));
//     auto root(createChildren(gua_node));

//     return av::Link<av::gua::Node>(root);
// }

// bool
// av::gua::PLODLoader::is_supported(std::string const& fileName) const
// {
//   return m_guaPLODLoader->is_supported(fileName);
// }

// void
// av::gua::PLODLoader::initClass()
// {
//     if (!isTypeInitialized())
//     {
//         av::FieldContainer::initClass();

//         AV_FC_INIT(av::FieldContainer, av::gua::PLODLoader, true);

//         SFPLODLoader::initClass("av::gua::SFPLODLoader", "av::Field");
//         MFPLODLoader::initClass("av::gua::MFPLODLoader", "av::Field");
//     }
// }

// ::gua::PLODLoader*
// av::gua::PLODLoader::getGuaPLODLoader() const
// {
//     return m_guaPLODLoader;
// }

// av::gua::Node*
// av::gua::PLODLoader::createChildren(std::shared_ptr< ::gua::node::Node> root) const {

//   av::gua::Node* av_node(nullptr);

//   auto group_cast(std::dynamic_pointer_cast< ::gua::node::TransformNode>(root));
//   if (group_cast) {
//     av_node = new av::gua::TransformNode(group_cast);
//   }

//   if (!av_node) {
//     auto geom_cast(std::dynamic_pointer_cast< ::gua::node::PLODNode>(root));
//     if (geom_cast) {
//       av_node = new av::gua::PLODNode(geom_cast);
//     }
//   }

//   if (av_node) {
//     av_node->addToParentChildren();
//   } else {
//     std::cout << "Unexpected node type encountered while loading geometry!" << std::endl;
//   }

//   for (auto c : root->get_children()) {
//     createChildren(c);
//   }

//   return av_node;
// }

// void
// av::gua::PLODLoader::getUploadBudgetCB(const SFInt::GetValueEvent& event)
// {
//   *(event.getValuePtr()) = m_guaPLODLoader->get_upload_budget_in_mb();
// }

// void
// av::gua::PLODLoader::setUploadBudgetCB(const SFInt::SetValueEvent& event)
// {
//   m_guaPLODLoader->set_upload_budget_in_mb(event.getValue());
// }

// void
// av::gua::PLODLoader::getRenderBudgetCB(const SFInt::GetValueEvent& event)
// {
//   *(event.getValuePtr()) = m_guaPLODLoader->get_render_budget_in_mb();
// }

// void
// av::gua::PLODLoader::setRenderBudgetCB(const SFInt::SetValueEvent& event)
// {
//   m_guaPLODLoader->set_render_budget_in_mb(event.getValue());
// }

// void
// av::gua::PLODLoader::getOutOfCoreBudgetCB(const SFInt::GetValueEvent& event)
// {
//   *(event.getValuePtr()) = m_guaPLODLoader->get_out_of_core_budget_in_mb();
// }

// void
// av::gua::PLODLoader::setOutOfCoreBudgetCB(const SFInt::SetValueEvent& event)
// {
//   m_guaPLODLoader->set_out_of_core_budget_in_mb(event.getValue());
// }
