#include "BlenderViewer.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/viewer/BlenderViewer.hpp>

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

void init_BlenderViewer()
 {
   register_ptr_to_python<av::Link<av::gua::BlenderViewer> >();
   
  class_<av::gua::BlenderViewer,
         av::Link<av::gua::BlenderViewer>,
         bases<av::FieldContainer>, boost::noncopyable> ("BlenderViewer", "docstring", no_init)
         .def("frame", &av::gua::BlenderViewer::frame)
         .def("unregister_engine", &av::gua::BlenderViewer::unregister_engine)
         ;

    // register as a field
    register_field<av::gua::SFBlenderViewer>("SFBlenderViewer");
    register_multifield<av::gua::MFBlenderViewer>("MFBlenderViewer");
 }
