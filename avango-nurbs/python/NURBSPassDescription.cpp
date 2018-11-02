#include "NURBSPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/nurbs/renderer/NURBSPassDescription.hpp>

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


void init_NURBSPassDescription()
 {

  register_ptr_to_python<av::Link<av::gua::nurbs::NURBSPassDescription> >();
  register_field<av::gua::nurbs::SFNURBSPassDescription>("SFNURBSPassDescription");
  register_multifield<av::gua::nurbs::MFNURBSPassDescription>("MFNURBSPassPassDescription");
  class_<av::gua::nurbs::NURBSPassDescription,
         av::Link<av::gua::nurbs::NURBSPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("NURBSPassDescription", "docstring", no_init)
         ;
 }

