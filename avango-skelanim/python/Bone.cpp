#include "Bone.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/skelanim/renderer/Bone.hpp>

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


void init_Bone()
 {
  register_ptr_to_python<av::Link<av::gua::Bone> >();
  register_field<av::gua::SFBone>("SFBone");
  register_multifield<av::gua::MFBone>("MFBone");
  class_<av::gua::Bone, av::Link<av::gua::Bone>, bases<av::FieldContainer>,
         boost::noncopyable >("Bone", "docstring", no_init)
  ;
 }
