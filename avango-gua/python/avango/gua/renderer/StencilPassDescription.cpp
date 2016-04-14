#include "StencilPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/StencilPassDescription.hpp>

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


void init_StencilPassDescription()
 {
   register_ptr_to_python<av::Link<av::gua::StencilPassDescription> >();
  register_field<av::gua::SFStencilPassDescription>("SFStencilPassDescription");
  register_multifield<av::gua::MFStencilPassDescription>("MFStencilPassDescription");
  class_<av::gua::StencilPassDescription,
         av::Link<av::gua::StencilPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("StencilPassDescription", "docstring", no_init)
         ;
 }

