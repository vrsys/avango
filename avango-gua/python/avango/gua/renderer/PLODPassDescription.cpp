#include "PLODPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/PLODPassDescription.hpp>

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


void init_PLODPassDescription()
 {
  register_field<av::gua::SFPLODPassDescription>("SFPLODPassDescription");
  register_multifield<av::gua::MFPLODPassDescription>("MFPLODPassDescription");
  class_<av::gua::PLODPassDescription,
         av::Link<av::gua::PLODPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("PLODPassDescription", "docstring", no_init)
         ;
 }

