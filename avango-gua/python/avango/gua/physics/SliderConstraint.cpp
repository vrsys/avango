#include "SliderConstraint.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/SliderConstraint.hpp>

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

void init_SliderConstraint()
{
  register_field<av::gua::SFSliderConstraint>("SFSliderConstraint");
  register_multifield<av::gua::MFSliderConstraint>("MFSliderConstraint");

  class_<av::gua::SliderConstraint,
         av::Link<av::gua::SliderConstraint>,
         bases<av::gua::Constraint>, boost::noncopyable >("SliderConstraint",
                                                    "docstring", no_init);
}
