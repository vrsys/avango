#include "PBRLoader.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/PBRLoader.hpp>

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

av::Link<av::gua::Node> load(
    av::gua::PBRLoader const& loader,
    std::string const& nodename,
    std::string const& fileName) {
   return loader.load(nodename, fileName);
}

bool is_supported(av::gua::PBRLoader const& loader, std::string const& file) {
   return loader.is_supported(file);
}

void init_PBRLoader()
{
  class_<av::gua::PBRLoader,
         av::Link<av::gua::PBRLoader>,
         bases<av::FieldContainer>, boost::noncopyable> ("PBRLoader", "docstring", no_init)
         .def("load", &load)
         .def("create_geometry_from_file", &load)
         .def("is_supported", &is_supported)
         ;

  register_field<av::gua::SFPBRLoader>("SFPBRLoader");
  register_multifield<av::gua::MFPBRLoader>("MFPBRLoader");
}
