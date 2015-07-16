#include <boost/python.hpp>

#include <avango/Init.h>
#include <avango/Config.h>

#include <avango/oculus/Init.hpp>
#include <avango/python/register_field.h>

#include "OculusWindow.hpp"

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

BOOST_PYTHON_MODULE(_oculus)
{
    PyEval_InitThreads();

    av::oculus::Init::initClass();

    init_OculusWindow();

}
