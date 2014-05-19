#include "Video3DLoader.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Video3DLoader.hpp>

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

av::Link<av::gua::Node> load( av::gua::Video3DLoader const& loader,
                              std::string const& nodename,
                              std::string const& fileName) {

   return loader.load(nodename, fileName);
}

void init_Video3DLoader()
{
  class_<av::gua::Video3DLoader,
         av::Link<av::gua::Video3DLoader>,
         bases<av::FieldContainer>, boost::noncopyable> ("Video3DLoader", "docstring", no_init)
         .def("load", &load)
         ;

  register_field<av::gua::SFVideo3DLoader>("SFVideo3DLoader");
  register_multifield<av::gua::MFVideo3DLoader>("MFVideo3DLoader");

}
