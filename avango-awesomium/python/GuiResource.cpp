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

void register_on_loaded(av::gua::gui::GuiResource& gui, boost::python::object& func) {
  gui.on_loaded([func]() {
    func();
  });
}

void call_javascript(av::gua::gui::GuiResource& gui, std::string const& method, list const& args) {
    std::vector<std::string> args_vec;

    for (int i(0); i < len(args); ++i) {
      args_vec.push_back(extract<std::string>(args[i]));
    }

    gui.call_javascript(method, args_vec);
}

void register_on_javascript_callback(av::gua::gui::GuiResource& gui, boost::python::object& func) {
  gui.on_javascript_callback([func](std::string const& method, std::vector<std::string> const& args) {
    list args_list;
    for (auto const& arg : args){
      args_list.append(arg);
    }
    func(method, args_list);
  });
}

void add_javascript_getter(av::gua::gui::GuiResource& gui, std::string const& name, boost::python::object& func) {
  auto getter = [func]() -> std::string {
    return extract<std::string>(func());
  };

  gui.add_javascript_getter(name, getter);
}

void init_GuiResource()
{
  register_field<av::gua::gui::SFGuiResource>("SFGuiResource");
  register_multifield<av::gua::gui::MFGuiResource>("MFGuiResource");
  class_<av::gua::gui::GuiResource, av::Link<av::gua::gui::GuiResource>,
    bases<av::FieldContainer>,
    boost::noncopyable >("GuiResource", "docstring", no_init)
    .def("init", &av::gua::gui::GuiResource::init)
    .def("go_forward", &av::gua::gui::GuiResource::go_forward)
    .def("go_back", &av::gua::gui::GuiResource::go_back)
    .def("go_to_history_offset", &av::gua::gui::GuiResource::go_to_history_offset)
    .def("reload", &av::gua::gui::GuiResource::reload)
    .def("focus", &av::gua::gui::GuiResource::focus)
    .def("inject_keyboard_event", &av::gua::gui::GuiResource::inject_keyboard_event)
    .def("inject_char_event", &av::gua::gui::GuiResource::inject_char_event)
    .def("inject_mouse_position_relative", &av::gua::gui::GuiResource::inject_mouse_position_relative)
    .def("inject_mouse_position", &av::gua::gui::GuiResource::inject_mouse_position)
    .def("inject_mouse_button", &av::gua::gui::GuiResource::inject_mouse_button)
    .def("inject_mouse_wheel", &av::gua::gui::GuiResource::inject_mouse_wheel)
    .def("on_loaded", register_on_loaded)
    .def("call_javascript", call_javascript)
    .def("on_javascript_callback", register_on_javascript_callback)
    .def("add_javascript_callback", &av::gua::gui::GuiResource::add_javascript_callback)
    .def("add_javascript_getter", add_javascript_getter)
    ;
}
