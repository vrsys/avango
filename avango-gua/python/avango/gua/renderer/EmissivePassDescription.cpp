#include "EmissivePassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/EmissivePassDescription.hpp>

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


void init_EmissivePassDescription()
 {
   register_ptr_to_python<av::Link<av::gua::EmissivePassDescription> >();

  register_field<av::gua::SFEmissivePassDescription>("SFEmissivePassDescription");
  register_multifield<av::gua::MFEmissivePassDescription>("MFEmissivePassDescription");
  class_<av::gua::EmissivePassDescription,
         av::Link<av::gua::EmissivePassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("EmissivePassDescription", "docstring", no_init)
         ;

 }

