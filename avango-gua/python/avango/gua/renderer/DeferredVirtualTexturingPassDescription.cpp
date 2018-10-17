#include "DeferredVirtualTexturingPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/DeferredVirtualTexturingPassDescription.hpp>

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

void init_DeferredVirtualTexturingPassDescription()
{
  register_field<av::gua::SFDeferredVirtualTexturingPassDescription>("SFDeferredVirtualTexturingPassDescription");
  register_multifield<av::gua::MFDeferredVirtualTexturingPassDescription>("MFDeferredVirtualTexturingPassDescription");
  class_<av::gua::DeferredVirtualTexturingPassDescription,
         av::Link<av::gua::DeferredVirtualTexturingPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("DeferredVirtualTexturingPassDescription", "docstring", no_init)
         ;
}

