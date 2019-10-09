#include "Auxi.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/lod/renderer/Auxi.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
  namespace python
  {
    template <class T> struct pointee<av::Link<T> >
    {
      typedef T type;
    };
  }
}

void load_aux_file(av::gua::lod::Auxi & auxi,
                    std::string const& filename) {
  auxi.load_aux_file(filename);
}

const std::string get_filename(av::gua::lod::Auxi const& auxi) {
  return auxi.get_filename();
}

const uint32_t get_num_views(av::gua::lod::Auxi const& auxi) {
  return auxi.get_num_views(); 
}

const uint64_t get_num_sparse_points(av::gua::lod::Auxi const& auxi) {
  return auxi.get_num_sparse_points(); 
}

const uint32_t get_num_atlas_tiles(av::gua::lod::Auxi const& auxi) { 
  return auxi.get_num_atlas_tiles(); 
}

const uint32_t get_num_nodes(av::gua::lod::Auxi const& auxi) { 
  return auxi.get_num_nodes(); 
}

const av::Link<av::gua::lod::AuxAtlas> get_atlas(av::gua::lod::Auxi const& auxi) {
  return auxi.get_atlas();
}

const av::Link<av::gua::lod::AuxAtlasTile> get_atlas_tile(av::gua::lod::Auxi const& auxi,
                                                          uint32_t id) {
  return auxi.get_atlas_tile(id);
}

const av::Link<av::gua::lod::AuxSparsePoint> get_sparse_point(av::gua::lod::Auxi const& auxi,
                                                          uint32_t id) {
  return auxi.get_sparse_point(id);
}

const av::Link<av::gua::lod::AuxView> get_view(av::gua::lod::Auxi const& auxi,
                                                    uint32_t id) {
  return auxi.get_view(id);
}

const av::Link<av::gua::lod::OctreeNode> get_octree_node(av::gua::lod::Auxi const& auxi,
                                                    uint32_t node_id) {
  return auxi.get_octree_node(node_id);
}

const uint64_t get_octree_query(av::gua::lod::Auxi const& auxi,
                          ::gua::math::vec3 const& pos){
  return auxi.get_octree_query(pos);
}

void init_Auxi()
{

  //register_ptr_to_python<av::Link<av::gua::lod::AuxAtlas> >();

  register_field<av::gua::lod::SFAuxi>("SFAuxi");
  register_multifield<av::gua::lod::MFAuxi>("MFAuxi");

  class_< av::gua::lod::Auxi,
          av::Link<av::gua::lod::Auxi>,
          bases<av::FieldContainer>, boost::noncopyable>("Auxi", "docstring", no_init)
          .def("load_aux_file", &load_aux_file)
          .def("get_filename", &get_filename)
          .def("get_num_views", &get_num_views)
          .def("get_num_sparse_points", &get_num_sparse_points)
          .def("get_num_atlas_tiles", &get_num_atlas_tiles)
          .def("get_atlas", &get_atlas)
          .def("get_atlas_tile", &get_atlas_tile)
          .def("get_sparse_point", &get_sparse_point)
          .def("get_view", &get_view)
          .def("get_octree_node", &get_octree_node)
          .def("get_num_nodes", &get_num_nodes)
          .def("get_octree_query", &get_octree_query)

    ;

  // register_field<av::gua::lod::SFAux>("SFAux");
  // register_multifield<av::gua::lod::MFAux>("MFAux");
}
