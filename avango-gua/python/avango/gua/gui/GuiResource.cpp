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
    .def("go_forward", &av::gua::GuiResource::go_forward)
    .def("go_back", &av::gua::GuiResource::go_back)
    .def("go_to_history_offset", &av::gua::GuiResource::go_to_history_offset)
    .def("reload", &av::gua::GuiResource::reload)
    .def("focus", &av::gua::GuiResource::focus)
    .def("inject_keyboard_event", &av::gua::GuiResource::inject_keyboard_event)
    .def("inject_char_event", &av::gua::GuiResource::inject_char_event)
    .def("inject_mouse_position_relative", &av::gua::GuiResource::inject_mouse_position_relative)
    .def("inject_mouse_position", &av::gua::GuiResource::inject_mouse_position)
    .def("inject_mouse_button", &av::gua::GuiResource::inject_mouse_button)
    .def("inject_mouse_wheel", &av::gua::GuiResource::inject_mouse_wheel)
    ;
}
