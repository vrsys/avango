#include <avango/gua/lod/renderer/Auxi.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <avango/Logger.h>
#include <functional>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::lod::Auxi"));
}

AV_FC_DEFINE(av::gua::lod::Auxi);

AV_FIELD_DEFINE(av::gua::lod::SFAuxi);
AV_FIELD_DEFINE(av::gua::lod::MFAuxi);

av::gua::lod::Auxi::Auxi(std::shared_ptr< ::gua::Auxi> guanode)
    : m_guaAux(guanode)
{
  
}

void
av::gua::lod::Auxi::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::lod::Auxi, true);

        SFAuxi::initClass("av::gua::SFAuxi", "av::Field");
        MFAuxi::initClass("av::gua::MFAuxi", "av::Field");
    }
}


void
av::gua::lod::Auxi::load_aux_file(std::string const& filename) {
  m_guaAux->load_aux_file(filename);
}

const std::string
av::gua::lod::Auxi::get_filename() const {
  return m_guaAux->get_filename();
}

const uint32_t 
av::gua::lod::Auxi::get_num_views() const {
  std::cout << "Calling gua get num view " << m_guaAux->get_num_views()<< std::endl;
  return m_guaAux->get_num_views(); 
}

const uint64_t 
av::gua::lod::Auxi::get_num_sparse_points() const {
  std::cout << "Calling gua get num sparse points " << m_guaAux->get_num_sparse_points()<< std::endl;
  return m_guaAux->get_num_sparse_points(); 
}

const uint64_t 
av::gua::lod::Auxi::get_num_nodes() const {
  std::cout << "Calling gua get num nodes " << m_guaAux->get_num_nodes()<< std::endl;
  return m_guaAux->get_num_nodes(); 
}

const uint32_t 
av::gua::lod::Auxi::get_num_atlas_tiles() const { 
  std::cout << "Calling gua get num atlas tiles " << m_guaAux->get_num_atlas_tiles()<< std::endl;
  return m_guaAux->get_num_atlas_tiles(); 
}

av::Link<av::gua::lod::AuxAtlas>
av::gua::lod::Auxi::get_atlas() const {
  //return av::Link<av::gua::lod::AuxAtlas>(new av::gua::lod::AuxAtlas(new ::gua::Auxi::atlas(m_guaAux->get_atlas())));
  return av::Link<av::gua::lod::AuxAtlas>(new av::gua::lod::AuxAtlas(m_guaAux->get_atlas()));
}


av::Link<av::gua::lod::AuxAtlasTile>
av::gua::lod::Auxi::get_atlas_tile( uint32_t id) const {
  return av::Link<av::gua::lod::AuxAtlasTile>(new av::gua::lod::AuxAtlasTile(m_guaAux->get_atlas_tile(id)));
}

av::Link<av::gua::lod::AuxSparsePoint>
av::gua::lod::Auxi::get_sparse_point( uint32_t id) const {
  return av::Link<av::gua::lod::AuxSparsePoint>(new av::gua::lod::AuxSparsePoint(m_guaAux->get_sparse_point(id)));
}



av::Link<av::gua::lod::AuxView>
av::gua::lod::Auxi::get_view( uint32_t id) const {
  return av::Link<av::gua::lod::AuxView>(new av::gua::lod::AuxView(m_guaAux->get_view(id)));
}

const uint64_t 
av::gua::lod::Auxi::get_octree_query(::gua::math::vec3 const& pos) const {
  return m_guaAux->get_octree_query(scm::math::vec3f(pos));
}


av::Link<av::gua::lod::OctreeNode> 
av::gua::lod::Auxi::get_octree_node(uint32_t node_id) const {
  return av::Link<av::gua::lod::OctreeNode>(new av::gua::lod::OctreeNode(m_guaAux->get_octree_node(node_id)));
}


std::shared_ptr< ::gua::Auxi>
av::gua::lod::Auxi::getGuaAux() const {
  return m_guaAux;
}