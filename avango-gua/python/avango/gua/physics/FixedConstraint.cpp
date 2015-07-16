#include "FixedConstraint.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/FixedConstraint.hpp>

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

void init_FixedConstraint()
{
  register_field<av::gua::SFFixedConstraint>("SFFixedConstraint");
  register_multifield<av::gua::MFFixedConstraint>("MFFixedConstraint");

  class_<av::gua::FixedConstraint,
         av::Link<av::gua::FixedConstraint>,
         bases<av::gua::Constraint>, boost::noncopyable >("FixedConstraint",
                                                    "docstring", no_init);
}
