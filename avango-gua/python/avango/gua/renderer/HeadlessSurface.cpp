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
      typedef T type;
    };
  }
}

boost::python::object image_data(::gua::HeadlessSurface::Image const& img)
{
  return boost::python::object(
      boost::python::handle<>(PyByteArray_FromStringAndSize(img.data.data(),
          img.data.size())));
}


void init_HeadlessSurface()
{

  class_<::gua::HeadlessSurface::Image>("Image", no_init)
      .def_readwrite("width", &::gua::HeadlessSurface::Image::width)
      .def_readwrite("height", &::gua::HeadlessSurface::Image::height)
      .def_readwrite("bpp", &::gua::HeadlessSurface::Image::bpp)
      .def_readwrite("gl_internal_format", &::gua::HeadlessSurface::Image::gl_internal_format)
      .def_readwrite("gl_base_format", &::gua::HeadlessSurface::Image::gl_base_format)
      .def_readwrite("gl_type", &::gua::HeadlessSurface::Image::gl_type)
      .def("data", &image_data)
      .def("size", &::gua::HeadlessSurface::Image::size)
      .def("size_header", &::gua::HeadlessSurface::Image::size_header)
      //.def_readwrite("data", &av::gua::HeadlessSurface::Image::data)
      ;

  register_field<av::gua::SFHeadlessSurface>("SFHeadlessSurface");
  register_multifield<av::gua::MFHeadlessSurface>("MFHeadlessSurface");
  class_<
      av::gua::HeadlessSurface,
      av::Link<av::gua::HeadlessSurface>,
      bases<av::gua::WindowBase>,
      boost::noncopyable
  >("HeadlessSurface", "docstring", no_init)
      .def("get_image", &av::gua::HeadlessSurface::get_image)
      ;
}
