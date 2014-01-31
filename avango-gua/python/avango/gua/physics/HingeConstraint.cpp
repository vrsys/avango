#include "HingeConstraint.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/HingeConstraint.hpp>

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

void init_HingeConstraint()
{
  register_field<av::gua::SFHingeConstraint>("SFHingeConstraint");
  register_multifield<av::gua::MFHingeConstraint>("MFHingeConstraint");

  class_<av::gua::HingeConstraint,
         av::Link<av::gua::HingeConstraint>,
         bases<av::gua::Constraint>, boost::noncopyable >("HingeConstraint",
                                                    "docstring", no_init);
}
