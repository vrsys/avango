#include "TexturedScreenSpaceQuadPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/TexturedScreenSpaceQuadPassDescription.hpp>

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


void init_TexturedScreenSpaceQuadPassDescription()
 {
  register_field<av::gua::SFTexturedScreenSpaceQuadPassDescription>("SFTexturedScreenSpaceQuadPassDescription");
  register_multifield<av::gua::MFTexturedScreenSpaceQuadPassDescription>("MFTexturedScreenSpaceQuadPassDescription");
  class_<av::gua::TexturedScreenSpaceQuadPassDescription,
         av::Link<av::gua::TexturedScreenSpaceQuadPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("TexturedScreenSpaceQuadPassDescription", "docstring", no_init)
         ;

 }

