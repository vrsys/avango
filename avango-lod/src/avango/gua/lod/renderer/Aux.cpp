#include <avango/gua/lod/renderer/Aux.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <avango/Logger.h>
#include <functional>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::lod::Aux"));
}

AV_FC_DEFINE(av::gua::lod::Aux);

AV_FIELD_DEFINE(av::gua::lod::SFAux);
AV_FIELD_DEFINE(av::gua::lod::MFAux);

av::gua::lod::Aux::Aux(std::shared_ptr< ::gua::Aux> guanode)
    : m_guaAux(guanode)
{
  
}

void
av::gua::lod::Aux::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::lod::Aux, true);

        SFAux::initClass("av::gua::SFAux", "av::Field");
        MFAux::initClass("av::gua::MFAux", "av::Field");
    }
}




void
av::gua::lod::Aux::test_wrapping() const {
  std::cout << "Calling gua function" << std::endl;
  m_guaAux->test_wrapping();
}

void
av::gua::lod::Aux::load_aux_file(std::string const& filename) {
  std::cout << "Calling gua 'LOAD AUX FILE' function" << std::endl;
  m_guaAux->load_aux_file(filename);
}

const std::string
av::gua::lod::Aux::get_filename() const {
  std::cout << "Calling gua get filename" << m_guaAux->get_filename()<< std::endl;
  return m_guaAux->get_filename();
}

const uint32_t 
av::gua::lod::Aux::get_num_views() const {
  std::cout << "Calling gua get num view " << m_guaAux->get_num_views()<< std::endl;
  return m_guaAux->get_num_views(); 
}

const uint64_t 
av::gua::lod::Aux::get_num_sparse_points() const {
  std::cout << "Calling gua get num sparse points " << m_guaAux->get_num_sparse_points()<< std::endl;
  return m_guaAux->get_num_sparse_points(); 
}

const uint32_t 
av::gua::lod::Aux::get_num_atlas_tiles() const { 
  std::cout << "Calling gua get num atlas tiles " << m_guaAux->get_num_atlas_tiles()<< std::endl;
  return m_guaAux->get_num_atlas_tiles(); 
}

// const av::gua::lod::AuxAtlas
// av::gua::lod::Aux::get_atlas2 () const {
//   return av::gua::lod::AuxAtlas(m_guaAux->get_atlas());
// }

av::Link<av::gua::lod::AuxAtlas>
av::gua::lod::Aux::get_atlas() const {
  //return av::Link<av::gua::lod::AuxAtlas>(new av::gua::lod::AuxAtlas(new ::gua::Aux::atlas(m_guaAux->get_atlas())));
  return av::Link<av::gua::lod::AuxAtlas>(new av::gua::lod::AuxAtlas(m_guaAux->get_atlas()));
}


av::Link<av::gua::lod::AuxAtlasTile>
av::gua::lod::Aux::get_atlas_tile( uint32_t id) const {
  return av::Link<av::gua::lod::AuxAtlasTile>(new av::gua::lod::AuxAtlasTile(m_guaAux->get_atlas_tile(id)));
}

av::Link<av::gua::lod::AuxSparsePoint>
av::gua::lod::Aux::get_sparse_point( uint32_t id) const {
  return av::Link<av::gua::lod::AuxSparsePoint>(new av::gua::lod::AuxSparsePoint(m_guaAux->get_sparse_point(id)));
}



av::Link<av::gua::lod::AuxView>
av::gua::lod::Aux::get_view( uint32_t id) const {
  return av::Link<av::gua::lod::AuxView>(new av::gua::lod::AuxView(m_guaAux->get_view(id)));
}


std::shared_ptr< ::gua::Aux>
av::gua::lod::Aux::getGuaAux() const {
  return m_guaAux;
}