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
      typedef T type;
     };
   }
 }

void init_BlenderViewer()
 {

  class_<av::gua::BlenderViewer,
         av::Link<av::gua::BlenderViewer>,
         bases<av::FieldContainer>, boost::noncopyable> ("BlenderViewer", "docstring", no_init)
         .def("frame", &av::gua::BlenderViewer::frame)
         ;

    // register as a field
    register_field<av::gua::SFBlenderViewer>("SFBlenderViewer");
    register_multifield<av::gua::MFBlenderViewer>("MFBlenderViewer");
 }
