#include "CollisionShape.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/CollisionShape.hpp>

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

void init_CollisionShape()
{
  register_field<av::gua::SFCollisionShape>("SFCollisionShape");
  register_multifield<av::gua::MFCollisionShape>("MFCollisionShape");

  class_<av::gua::CollisionShape,
         av::Link<av::gua::CollisionShape>,
         bases<av::FieldContainer>, boost::noncopyable >("CollisionShape",
                                                    "docstring", no_init);
}
