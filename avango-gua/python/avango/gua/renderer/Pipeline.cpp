#include "Pipeline.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Pipeline.hpp>
#include <avango/gua/Types.hpp>

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


void init_Pipeline()
{
  register_field<av::gua::SFPipeline>("SFPipeline");
  register_multifield<av::gua::MFPipeline>("MFPipeline");
  class_<av::gua::Pipeline,
         av::Link<av::gua::Pipeline>,
         bases<av::FieldContainer>,
         boost::noncopyable >("Pipeline", "docstring", no_init);

}
