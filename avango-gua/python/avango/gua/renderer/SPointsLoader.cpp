#include "SPointsLoader.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/SPointsLoader.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
  namespace python
  {
    template <class T> struct pointee<av::Link<T> >
    {
      using type = T;
    };
  }
}

av::Link<av::gua::Node> load(
    av::gua::SPointsLoader const& loader,
    std::string const& nodename,
    std::string const& fileName) {
   return loader.load(nodename, fileName);
}

void init_SPointsLoader()
{
  class_<av::gua::SPointsLoader,
         av::Link<av::gua::SPointsLoader>,
         bases<av::FieldContainer>, boost::noncopyable> ("SPointsLoader", "docstring", no_init)
         .def("load", &load)
         .def("create_geometry_from_file", &load)
         ;

  register_field<av::gua::SFSPointsLoader>("SFSPointsLoader");
  register_multifield<av::gua::MFSPointsLoader>("MFSPointsLoader");
}
