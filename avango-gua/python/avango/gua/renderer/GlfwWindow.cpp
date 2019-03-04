#include "GlfwWindow.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/GlfwWindow.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
namespace python
{
template <class T>
struct pointee<av::Link<T>>
{
    using type = T;
};
} // namespace python
} // namespace boost

void register_on_resize(av::gua::GlfwWindow& window, boost::python::object& func)
{
    window.on_resize([func](::gua::math::vec2ui const& size) { func(size); });
}

void register_on_key_press(av::gua::GlfwWindow& window, boost::python::object& func)
{
    window.on_key_press([func](int key, int scancode, int action, int mods) { func(key, scancode, action, mods); });
}

void register_on_char(av::gua::GlfwWindow& window, boost::python::object& func)
{
    window.on_char([func](unsigned c) { func(c); });
}

void register_on_button_press(av::gua::GlfwWindow& window, boost::python::object& func)
{
    window.on_button_press([func](int mouse_button, int action, int mods) { func(mouse_button, action, mods); });
}

void register_on_move_cursor(av::gua::GlfwWindow& window, boost::python::object& func)
{
    window.on_move_cursor([func](::gua::math::vec2 const& position) { func(position); });
}

void register_on_scroll(av::gua::GlfwWindow& window, boost::python::object& func)
{
    window.on_scroll([func](::gua::math::vec2 const& scroll) { func(scroll); });
}

void register_on_enter(av::gua::GlfwWindow& window, boost::python::object& func)
{
    window.on_enter([func](bool mouse_entered) { func(mouse_entered); });
}

void init_GlfwWindow()
{
    register_ptr_to_python<av::Link<av::gua::GlfwWindow>>();

    register_field<av::gua::SFGlfwWindow>("SFGlfwWindow");
    register_multifield<av::gua::MFGlfwWindow>("MFGlfwWindow");
    class_<av::gua::GlfwWindow, av::Link<av::gua::GlfwWindow>, bases<av::gua::WindowBase>, boost::noncopyable>("GlfwWindow", "docstring", no_init)
        .def("on_resize", register_on_resize)
        .def("on_key_press", register_on_key_press)
        .def("on_char", register_on_char)
        .def("on_button_press", register_on_button_press)
        .def("on_move_cursor", register_on_move_cursor)
        .def("on_scroll", register_on_scroll)
        .def("on_enter", register_on_enter);

    enum_<av::gua::GlfwWindow::CursorModeEnum>("CursorMode").value("NORMAL", av::gua::GlfwWindow::NORMAL).value("HIDDEN", av::gua::GlfwWindow::HIDDEN).value("DISABLED", av::gua::GlfwWindow::DISABLED);
}
