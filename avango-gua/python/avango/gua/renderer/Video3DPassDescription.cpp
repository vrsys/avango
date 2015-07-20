#include "Video3DPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Video3DPassDescription.hpp>

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


void init_Video3DPassDescription()
{
  register_field<av::gua::SFVideo3DPassDescription>("SFVideo3DPassDescription");
  register_multifield<av::gua::MFVideo3DPassDescription>("MFVideo3DPassDescription");
  class_<av::gua::Video3DPassDescription,
         av::Link<av::gua::Video3DPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("Video3DPassDescription", "docstring", no_init)
         ;
}

