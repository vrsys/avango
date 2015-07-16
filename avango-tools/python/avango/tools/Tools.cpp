#include "Tools.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>

#include <avango/tools/Tool.hpp>
#include <avango/tools/DragTool.hpp>
#include <avango/tools/ScaledGrabTool.hpp>

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

void init_Tools()
{
  register_field<av::tools::SFTool>("SFTool");
  register_multifield<av::tools::MFTool>("MFTool");
  register_field<av::tools::SFDragTool>("SFDragTool");
  register_multifield<av::tools::MFDragTool>("MFDragTool");
  register_field<av::tools::SFScaledGrabTool>("SFScaledGrabTool");
  register_multifield<av::tools::MFScaledGrabTool>("MFScaledGrabTool");

  class_<av::tools::Tool, av::Link<av::tools::Tool>, bases<av::FieldContainer>, boost::noncopyable >("Tool", "Tool base class", no_init);
  class_<av::tools::DragTool, av::Link<av::tools::DragTool>, bases<av::FieldContainer>, boost::noncopyable >("DragTool", "DragTool base class", no_init);
  class_<av::tools::ScaledGrabTool, av::Link<av::tools::ScaledGrabTool>, bases<av::FieldContainer>, boost::noncopyable >("ScaledGrabTool", "ScaledGrabTool base class", no_init);
}
