#include "OcclusionSlaveResolvePassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/OcclusionSlaveResolvePassDescription.hpp>

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


void init_OcclusionSlaveResolvePassDescription()
 {
  register_ptr_to_python<av::Link<av::gua::OcclusionSlaveResolvePassDescription> >();
  register_field<av::gua::SFOcclusionSlaveResolvePassDescription>("SFOcclusionSlaveResolvePassDescription");
  register_multifield<av::gua::MFOcclusionSlaveResolvePassDescription>("MFOcclusionSlaveResolvePassDescription");
  class_<av::gua::OcclusionSlaveResolvePassDescription,
         av::Link<av::gua::OcclusionSlaveResolvePassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("OcclusionSlaveResolvePassDescription", "docstring", no_init)
         ;

 }

