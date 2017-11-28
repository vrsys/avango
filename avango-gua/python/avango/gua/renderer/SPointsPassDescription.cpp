#include "SPointsPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/SPointsPassDescription.hpp>

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


void init_SPointsPassDescription()
{
  register_field<av::gua::SFSPointsPassDescription>("SFSPoints3DPassDescription");
  register_multifield<av::gua::MFSPointsPassDescription>("MFSPoints3DPassDescription");
  class_<av::gua::SPointsPassDescription,
         av::Link<av::gua::SPointsPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("SPointsPassDescription", "docstring", no_init)
         ;
}

