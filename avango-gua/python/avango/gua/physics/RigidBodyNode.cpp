#include "RigidBodyNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/RigidBodyNode.hpp>

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

void init_RigidBodyNode()
{
  register_field<av::gua::SFRigidBodyNode>("SFRigidBodyNode");
  register_multifield<av::gua::MFRigidBodyNode>("MFRigidBodyNode");

  class_<av::gua::RigidBodyNode,
         av::Link<av::gua::RigidBodyNode>,
         bases<av::gua::Node>, boost::noncopyable >("RigidBodyNode", "docstring", no_init)
         .def("apply_force", &::gua::physics::RigidBodyNode::apply_force)
         .def("apply_central_force", &::gua::physics::RigidBodyNode::apply_central_force)
         .def("apply_torque", &::gua::physics::RigidBodyNode::apply_torque)
         .def("apply_torque_impulse", &::gua::physics::RigidBodyNode::apply_torque_impulse)
         .def("apply_impulse", &::gua::physics::RigidBodyNode::apply_impulse)
         .def("clear_forces", &::gua::physics::RigidBodyNode::clear_forces)
         ;
}
