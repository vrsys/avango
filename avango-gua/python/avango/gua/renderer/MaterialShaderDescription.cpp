#include "MaterialShaderDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/MaterialShaderDescription.hpp>

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


void init_MaterialShaderDescription()
 {

  register_field<av::gua::SFMaterialShaderDescription>("SFMaterialShaderDescription");
  register_multifield<av::gua::MFMaterialShaderDescription>("MFMaterialShaderDescription");
  class_<av::gua::MaterialShaderDescription,
         av::Link<av::gua::MaterialShaderDescription>,
         bases<av::FieldContainer>, boost::noncopyable >("MaterialShaderDescription", "docstring", no_init)
         ;
 }
