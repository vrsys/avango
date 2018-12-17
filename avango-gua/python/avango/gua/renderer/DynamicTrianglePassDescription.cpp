#include "DynamicTrianglePassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/DynamicTrianglePassDescription.hpp>

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


void init_DynamicTrianglePassDescription()
 {
  register_ptr_to_python<av::Link<av::gua::DynamicTrianglePassDescription> >();
  register_field<av::gua::SFDynamicTrianglePassDescription>("SFDynamicTrianglePassDescription");
  register_multifield<av::gua::MFDynamicTrianglePassDescription>("MFDynamicTrianglePassDescription");
  class_<av::gua::DynamicTrianglePassDescription,
         av::Link<av::gua::DynamicTrianglePassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("DynamicTrianglePassDescription", "docstring", no_init)
         ;
 }

