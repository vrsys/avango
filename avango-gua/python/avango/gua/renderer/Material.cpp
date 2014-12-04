#include "Material.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Material.hpp>

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


void init_Material()
 {

  register_field<av::gua::SFMaterial>("SFMaterial");
  register_multifield<av::gua::MFMaterial>("MFMaterial");
  class_<av::gua::Material,
         av::Link<av::gua::Material>,
         bases<av::FieldContainer>, boost::noncopyable >("Material", "docstring", no_init)
         ;
 }
