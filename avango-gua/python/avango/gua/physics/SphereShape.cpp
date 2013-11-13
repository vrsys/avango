#include "SphereShape.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/SphereShape.hpp>

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

void init_SphereShape()
{
  register_field<av::gua::SFSphereShape>("SFSphereShape");
  register_multifield<av::gua::MFSphereShape>("MFSphereShape");

  class_<av::gua::SphereShape,
         av::Link<av::gua::SphereShape>,
         bases<av::gua::CollisionShape>, boost::noncopyable >("SphereShape",
                                                              "docstring",
                                                              no_init);
}
