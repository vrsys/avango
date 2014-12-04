#include "MaterialShaderMethod.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/MaterialShaderMethod.hpp>

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


void init_MaterialShaderMethod()
 {

  register_field<av::gua::SFMaterialShaderMethod>("SFMaterialShaderMethod");
  register_multifield<av::gua::MFMaterialShaderMethod>("MFMaterialShaderMethod");
  class_<av::gua::MaterialShaderMethod,
         av::Link<av::gua::MaterialShaderMethod>,
         bases<av::FieldContainer>, boost::noncopyable >("MaterialShaderMethod", "docstring", no_init)
         ;
 }
