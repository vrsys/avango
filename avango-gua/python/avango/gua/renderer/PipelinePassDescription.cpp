#include "PipelinePassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/PipelinePassDescription.hpp>

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


void init_PipelinePassDescription()
 {
   register_ptr_to_python<av::Link<av::gua::PipelinePassDescription> >();
  register_field<av::gua::SFPipelinePassDescription>("SFPipelinePassDescription");
  register_multifield<av::gua::MFPipelinePassDescription>("MFPipelinePassDescription");
  class_<av::gua::PipelinePassDescription,
         av::Link<av::gua::PipelinePassDescription>,
         bases<av::FieldContainer>, boost::noncopyable >("PipelinePassDescription", "docstring", no_init)
         ;
 }

