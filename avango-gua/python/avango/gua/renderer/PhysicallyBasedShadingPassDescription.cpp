#include "PhysicallyBasedShadingPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/PhysicallyBasedShadingPassDescription.hpp>

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


void init_PhysicallyBasedShadingPassDescription()
 {
   register_ptr_to_python<av::Link<av::gua::PhysicallyBasedShadingPassDescription> >();
  register_field<av::gua::SFPhysicallyBasedShadingPassDescription>("SFPhysicallyBasedShadingPassDescription");
  register_multifield<av::gua::MFPhysicallyBasedShadingPassDescription>("MFPhysicallyBasedShadingPassDescription");
  class_<av::gua::PhysicallyBasedShadingPassDescription,
         av::Link<av::gua::PhysicallyBasedShadingPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("PhysicallyBasedShadingPassDescription", "docstring", no_init)
         ;

 }

