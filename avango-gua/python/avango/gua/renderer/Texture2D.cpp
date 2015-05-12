#include "Texture2D.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Texture2D.hpp>

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

void init_Texture2D()
 {

  register_field<av::gua::SFTexture2D>("SFTexture2D");
  register_multifield<av::gua::MFTexture2D>("MFTexture2D");
  class_<av::gua::Texture2D,
         av::Link<av::gua::Texture2D>,
         bases<av::FieldContainer>, boost::noncopyable >("Texture2D", "docstring", no_init)
         ;
 }
