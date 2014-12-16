#include "Mask.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/utils/Mask.hpp>

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

void init_Mask()
{
  register_field<av::gua::SFMask>("SFMask");
  register_multifield<av::gua::MFMask>("MFMask");
  class_<av::gua::Mask, av::Link<av::gua::Mask>,
    bases<av::FieldContainer>,
    boost::noncopyable >("Mask", "docstring", no_init);
}
