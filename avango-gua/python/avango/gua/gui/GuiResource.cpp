#include "GuiResource.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/gui/GuiResource.hpp>

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

void init_GuiResource()
{
  register_field<av::gua::SFGuiResource>("SFGuiResource");
  register_multifield<av::gua::MFGuiResource>("MFGuiResource");
  class_<av::gua::GuiResource, av::Link<av::gua::GuiResource>,
    bases<av::FieldContainer>,
    boost::noncopyable >("GuiResource", "docstring", no_init)
    .def("init", &av::gua::GuiResource::init)
    ;
}
