#include "ScreenshotPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/ScreenshotPassDescription.hpp>

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

void init_ScreenshotPassDescription()
{
  register_field<av::gua::SFScreenshotPassDescription>("SFScreenshotPassDescription");
  register_multifield<av::gua::MFScreenshotPassDescription>("MFScreenshotPassDescription");

  class_<av::gua::ScreenshotPassDescription,
         av::Link<av::gua::ScreenshotPassDescription>,
         bases<av::gua::PipelinePassDescription>,
         boost::noncopyable
    >("ScreenshotPassDescription", "docstring", no_init)
       ;
}
