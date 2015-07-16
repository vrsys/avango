#include "SSAAPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/SSAAPassDescription.hpp>

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


void init_SSAAPassDescription()
 {
  register_field<av::gua::SFSSAAPassDescription>("SFSSAAPassDescription");
  register_multifield<av::gua::MFSSAAPassDescription>("MFSSAAPassDescription");
  class_<av::gua::SSAAPassDescription,
         av::Link<av::gua::SSAAPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("SSAAPassDescription", "docstring", no_init)
         ;
 }

