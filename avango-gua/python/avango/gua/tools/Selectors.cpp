#include "Selectors.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>

#include <avango/gua/tools/ChangeSelector.hpp>
#include <avango/gua/tools/InjectSelector.hpp>
#include <avango/gua/tools/ObjectSelector.hpp>
#include <avango/gua/tools/NameSelector.hpp>
#include <avango/gua/tools/FieldSelector.hpp>
#include <avango/gua/tools/Selector.hpp>
#include <avango/gua/tools/SingleSelector.hpp>
#include <avango/gua/tools/PickSelector.hpp>
#include <avango/gua/tools/ProximitySelector.hpp>
#include <avango/gua/tools/TriggerSelector.hpp>
#include <avango/gua/tools/TypeSelector.hpp>
#include <avango/gua/tools/UnionSelector.hpp>


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

void init_Selectors()
{
  register_field<av::gua::SFChangeSelector>("SFChangeSelector");
  register_multifield<av::gua::MFChangeSelector>("MFChangeSelector");
  register_field<av::gua::SFInjectSelector>("SFInjectSelector");
  register_multifield<av::gua::MFInjectSelector>("MFInjectSelector");
  register_field<av::gua::SFObjectSelector>("SFObjectSelector");
  register_multifield<av::gua::MFObjectSelector>("MFObjectSelector");
  register_field<av::gua::SFNameSelector>("SFNameSelector");
  register_multifield<av::gua::MFNameSelector>("MFNameSelector");
  register_field<av::gua::SFFieldSelector>("SFFieldSelector");
  register_multifield<av::gua::MFFieldSelector>("MFFieldSelector");
  register_field<av::gua::SFSelector>("SFSelector");
  register_multifield<av::gua::MFSelector>("MFSelector");
  register_field<av::gua::SFSingleSelector>("SFSingleSelector");
  register_multifield<av::gua::MFSingleSelector>("MFSingleSelector");
  register_field<av::gua::SFPickSelector>("SFPickSelector");
  register_multifield<av::gua::MFPickSelector>("MFPickSelector");
  register_field<av::gua::SFProximitySelector>("SFProximitySelector");
  register_multifield<av::gua::MFProximitySelector>("MFProximitySelector");
  register_field<av::gua::SFTriggerSelector>("SFTriggerSelector");
  register_multifield<av::gua::MFTriggerSelector>("MFTriggerSelector");
  register_field<av::gua::SFTypeSelector>("SFTypeSelector");
  register_multifield<av::gua::MFTypeSelector>("MFTypeSelector");
  register_field<av::gua::SFUnionSelector>("SFUnionSelector");
  register_multifield<av::gua::MFUnionSelector>("MFUnionSelector");

  class_<av::gua::Selector, av::Link<av::gua::Selector>, bases<av::FieldContainer>, boost::noncopyable >("Selector", "Selector base class", no_init);
  class_<av::gua::ChangeSelector, av::Link<av::gua::ChangeSelector>, bases<av::gua::Selector>, boost::noncopyable >("ChangeSelector", "ChangeSelector class", no_init);
  class_<av::gua::InjectSelector, av::Link<av::gua::InjectSelector>, bases<av::gua::Selector>, boost::noncopyable >("InjectSelector", "InjectSelector class", no_init);
  class_<av::gua::ObjectSelector, av::Link<av::gua::ObjectSelector>, bases<av::gua::Selector>, boost::noncopyable >("ObjectSelector", "ObjectSelector class", no_init);
  class_<av::gua::NameSelector, av::Link<av::gua::NameSelector>, bases<av::gua::Selector>, boost::noncopyable >("NameSelector", "NameSelector class", no_init);
  class_<av::gua::FieldSelector, av::Link<av::gua::FieldSelector>, bases<av::gua::Selector>, boost::noncopyable >("FieldSelector", "FieldSelector class", no_init);
  class_<av::gua::SingleSelector, av::Link<av::gua::SingleSelector>, bases<av::gua::Selector>, boost::noncopyable >("SingleSelector", "SingleSelector class", no_init);
  class_<av::gua::PickSelector, av::Link<av::gua::PickSelector>, bases<av::gua::Selector>, boost::noncopyable >("PickSelector", "PickSelector class", no_init);
  class_<av::gua::ProximitySelector, av::Link<av::gua::ProximitySelector>, bases<av::gua::Selector>, boost::noncopyable >("ProximitySelector", "ProximitySelector class", no_init);
  class_<av::gua::TriggerSelector, av::Link<av::gua::TriggerSelector>, bases<av::gua::Selector>, boost::noncopyable >("TriggerSelector", "TriggerSelector class", no_init);
  class_<av::gua::TypeSelector, av::Link<av::gua::TypeSelector>, bases<av::gua::Selector>, boost::noncopyable >("TypeSelector", "TypeSelector class", no_init);
  class_<av::gua::UnionSelector, av::Link<av::gua::UnionSelector>, bases<av::gua::Selector>, boost::noncopyable >("UnionSelector", "UnionSelector class", no_init);
}
