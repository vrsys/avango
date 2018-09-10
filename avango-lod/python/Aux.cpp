#include "Aux.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/lod/renderer/Aux.hpp>

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

void test_wrapping(av::gua::lod::Aux const& aux) {
  aux.test_wrapping();
}

void load_aux_file(av::gua::lod::Aux & aux,
                    std::string const& filename) {
  aux.load_aux_file(filename);
}

const std::string get_filename(av::gua::lod::Aux const& aux) {
  return aux.get_filename();
}


const uint32_t get_num_views(av::gua::lod::Aux const& aux) {
  return aux.get_num_views(); 
}

const uint64_t get_num_sparse_points(av::gua::lod::Aux const& aux) {
  return aux.get_num_sparse_points(); 
}

const uint32_t get_num_atlas_tiles(av::gua::lod::Aux const& aux) { 
  return aux.get_num_atlas_tiles(); 
}

void init_Aux()
{
  register_field<av::gua::lod::SFAux>("SFAux");
  register_multifield<av::gua::lod::MFAux>("MFAux");

  class_< av::gua::lod::Aux,
          av::Link<av::gua::lod::Aux>,
          bases<av::FieldContainer>, boost::noncopyable>("Aux", "docstring", no_init)
          .def("test_wrapping", &test_wrapping)
          .def("load_aux_file", &load_aux_file)
          .def("get_filename", &get_filename)
          .def("get_num_views", &get_num_views)
          .def("get_num_sparse_points", &get_num_sparse_points)
          .def("get_num_atlas_tiles", &get_num_atlas_tiles)

    ;

  // register_field<av::gua::lod::SFAux>("SFAux");
  // register_multifield<av::gua::lod::MFAux>("MFAux");
}
