#include "TV_3SurfacePassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/tv_3/renderer/TV_3SurfacePassDescription.hpp>

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


void init_TV_3SurfacePassDescription()
 {

  register_ptr_to_python<av::Link<av::gua::tv_3::TV_3SurfacePassDescription> >();
  register_field<av::gua::tv_3::SFTV_3SurfacePassDescription>("SFTV_3SurfacePassDescription");
  register_multifield<av::gua::tv_3::MFTV_3SurfacePassDescription>("MFTV_3SurfacePassDescription");
  class_<av::gua::tv_3::TV_3SurfacePassDescription,
         av::Link<av::gua::tv_3::TV_3SurfacePassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("TV_3SurfacePassDescription", "docstring", no_init)
         ;
 }

