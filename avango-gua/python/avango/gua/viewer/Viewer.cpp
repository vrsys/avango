#include "Viewer.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/viewer/Viewer.hpp>

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

void init_Viewer()
 {

  class_<av::gua::Viewer,
         av::Link<av::gua::Viewer>,
         bases<av::FieldContainer>, boost::noncopyable> ("Viewer", "docstring", no_init)
         .def("run", &av::gua::Viewer::run)
         ;

    // register as a field
    register_field<av::gua::SFViewer>("SFViewer");
    register_multifield<av::gua::MFViewer>("MFViewer");
 }
