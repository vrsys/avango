#include "SSAOPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/SSAOPassDescription.hpp>

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


void init_SSAOPassDescription()
 {
   register_ptr_to_python<av::Link<av::gua::SSAOPassDescription> >();
  register_field<av::gua::SFSSAOPassDescription>("SFSSAOPassDescription");
  register_multifield<av::gua::MFSSAOPassDescription>("MFSSAOPassDescription");
  class_<av::gua::SSAOPassDescription,
         av::Link<av::gua::SSAOPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("SSAOPassDescription", "docstring", no_init)
         ;
 }

