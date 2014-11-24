// #include "NURBSNode.hpp"

// #include <boost/python.hpp>
// #include <avango/python/register_field.h>
// #include <avango/gua/scenegraph/GeometryNode.hpp>
// #include <avango/gua/scenegraph/NURBSNode.hpp>

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

// void init_NURBSNode()
// {
//   register_field<av::gua::SFNURBSNode>("SFNURBSNode");
//   register_multifield<av::gua::MFNURBSNode>("MFNURBSNode");
//   class_<av::gua::NURBSNode, av::Link<av::gua::NURBSNode>,
//     bases<av::gua::GeometryNode>,
//     boost::noncopyable >("NURBSNode", "docstring", no_init);
// }
