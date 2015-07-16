#include "Constraint.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/Constraint.hpp>

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

void init_Constraint()
{
  register_field<av::gua::SFConstraint>("SFConstraint");
  register_multifield<av::gua::MFConstraint>("MFConstraint");

  class_<av::gua::Constraint,
         av::Link<av::gua::Constraint>,
         bases<av::FieldContainer>, boost::noncopyable >("Constraint",
                                                    "docstring", no_init);
}
