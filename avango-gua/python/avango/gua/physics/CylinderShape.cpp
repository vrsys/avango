#include "CylinderShape.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/CylinderShape.hpp>

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

void init_CylinderShape()
{
  register_field<av::gua::SFCylinderShape>("SFCylinderShape");
  register_multifield<av::gua::MFCylinderShape>("MFCylinderShape");

  class_<av::gua::CylinderShape,
         av::Link<av::gua::CylinderShape>,
         bases<av::gua::CollisionShape>, boost::noncopyable >("CylinderShape",
                                                              "docstring",
                                                              no_init);
}
