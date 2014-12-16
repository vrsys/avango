#include "TagList.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/utils/TagList.hpp>

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

void init_TagList()
{
  register_field<av::gua::SFTagList>("SFTagList");
  register_multifield<av::gua::MFTagList>("MFTagList");
  class_<av::gua::TagList, av::Link<av::gua::TagList>,
    bases<av::FieldContainer>,
    boost::noncopyable >("TagList", "docstring", no_init)
    .def("print", &av::gua::TagList::print)
    ;
}
