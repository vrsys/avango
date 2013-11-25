#include "Tools.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>

#include <avango/gua/tools/Tool.hpp>
#include <avango/gua/tools/DragTool.hpp>
#include <avango/gua/tools/ScaledGrabTool.hpp>



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

void init_Tools()
{
  register_field<av::gua::SFTool>("SFTool");
  register_multifield<av::gua::MFTool>("MFTool");
  register_field<av::gua::SFDragTool>("SFDragTool");
  register_multifield<av::gua::MFDragTool>("MFDragTool");
  register_field<av::gua::SFScaledGrabTool>("SFScaledGrabTool");
  register_multifield<av::gua::MFScaledGrabTool>("MFScaledGrabTool");


  class_<av::gua::Tool, av::Link<av::gua::Tool>, bases<av::FieldContainer>, boost::noncopyable >("Tool", "Tool base class", no_init);
  class_<av::gua::DragTool, av::Link<av::gua::DragTool>, bases<av::FieldContainer>, boost::noncopyable >("DragTool", "DragTool base class", no_init);
  class_<av::gua::ScaledGrabTool, av::Link<av::gua::ScaledGrabTool>, bases<av::FieldContainer>, boost::noncopyable >("ScaledGrabTool", "ScaledGrabTool base class", no_init);
}
