// #include "PBRNode.hpp"

// #include <boost/python.hpp>
// #include <avango/python/register_field.h>
// #include <avango/gua/scenegraph/GeometryNode.hpp>
// #include <avango/gua/scenegraph/PBRNode.hpp>

// using namespace boost::python;
// using namespace av::python;

// namespace boost
//  {
//   namespace python
//    {
//     template <class T> struct pointee<av::Link<T> >
//      {
//       typedef T type;
//      };
//    }
//  }

// void init_PBRNode()
//  {
//   register_field<av::gua::SFPBRNode>("SFPBRNode");
//   register_multifield<av::gua::MFPBRNode>("MFPBRNode");
//   class_<av::gua::PBRNode, av::Link<av::gua::PBRNode>, bases<av::gua::GeometryNode>,
//     boost::noncopyable >("PBRNode", "docstring", no_init);
//  }
