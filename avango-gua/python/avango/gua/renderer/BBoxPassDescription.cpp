#include "BBoxPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/BBoxPassDescription.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
 {
  namespace python
   {
    template <class T> struct pointee<av::Link<T> >
     {
      typedef T type;
     };
   }
 }


void init_BBoxPassDescription()
 {
  register_field<av::gua::SFBBoxPassDescription>("SFBBoxPassDescription");
  register_multifield<av::gua::MFBBoxPassDescription>("MFBBoxPassDescription");
  class_<av::gua::BBoxPassDescription,
         av::Link<av::gua::BBoxPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("BBoxPassDescription", "docstring", no_init)
         ;

 }

