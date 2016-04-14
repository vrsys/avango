#include "Logger.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/utils/Logger.hpp>

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

void init_Logger()
 {
   register_ptr_to_python<av::Link<av::gua::Logger> >();
  register_field<av::gua::SFLogger>("SFLogger");
  register_multifield<av::gua::MFLogger>("MFLogger");
  class_<av::gua::Logger, av::Link<av::gua::Logger>, bases<av::FieldContainer>, boost::noncopyable >("Logger", "docstring", no_init);
 }
