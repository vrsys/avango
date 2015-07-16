#include "PlaneShape.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/PlaneShape.hpp>

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

void init_PlaneShape()
{
  register_field<av::gua::SFPlaneShape>("SFPlaneShape");
  register_multifield<av::gua::MFPlaneShape>("MFPlaneShape");

  class_<av::gua::PlaneShape,
         av::Link<av::gua::PlaneShape>,
         bases<av::gua::CollisionShape>, boost::noncopyable >("PlaneShape",
                                                              "docstring",
                                                              no_init);
}
