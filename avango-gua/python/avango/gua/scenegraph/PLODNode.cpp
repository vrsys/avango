// #include "PLODNode.hpp"

// #include <boost/python.hpp>
// #include <avango/python/register_field.h>
// #include <avango/gua/scenegraph/GeometryNode.hpp>
// #include <avango/gua/scenegraph/PLODNode.hpp>

// using namespace boost::python;
// using namespace av::python;

// namespace boost
// {
//   namespace python
//   {
//     template <class T> struct pointee<av::Link<T> >
//     {
//       typedef T type;
//     };
//   }
// }

// void init_PLODNode()
// {
//   register_field<av::gua::SFPLODNode>("SFPLODNode");
//   register_multifield<av::gua::MFPLODNode>("MFPLODNode");
//   class_<av::gua::PLODNode, av::Link<av::gua::PLODNode>, bases<av::gua::GeometryNode>,
//     boost::noncopyable >("PLODNode", "docstring", no_init);
// }
