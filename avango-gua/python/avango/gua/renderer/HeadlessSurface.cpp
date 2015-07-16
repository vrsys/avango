#include "HeadlessSurface.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/HeadlessSurface.hpp>

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

void init_HeadlessSurface()
{
  register_field<av::gua::SFHeadlessSurface>("SFHeadlessSurface");
  register_multifield<av::gua::MFHeadlessSurface>("MFHeadlessSurface");
  class_<
      av::gua::HeadlessSurface,
      av::Link<av::gua::HeadlessSurface>,
      bases<av::gua::WindowBase>,
      boost::noncopyable
  >("HeadlessSurface", "docstring", no_init)
      ;
}
