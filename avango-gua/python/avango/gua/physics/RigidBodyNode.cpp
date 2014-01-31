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

void applyForce(av::gua::RigidBodyNode const& node,
                 ::gua::math::vec3 const& force,
                 ::gua::math::vec3 const& relative_position) {

  node.getGuaNode()->apply_force(force, relative_position);
}

void applyCentralForce(av::gua::RigidBodyNode const& node,
                 ::gua::math::vec3 const& force) {

  node.getGuaNode()->apply_central_force(force);
}

void applyTorque(av::gua::RigidBodyNode const& node,
                 ::gua::math::vec3 const& torque) {

  node.getGuaNode()->apply_torque(torque);
}

void applyTorqueImpulse(av::gua::RigidBodyNode const& node,
                        ::gua::math::vec3 const& torque) {

  node.getGuaNode()->apply_torque_impulse(torque);
}

void applyImpulse(av::gua::RigidBodyNode const& node,
                  ::gua::math::vec3 const& impulse,
                  ::gua::math::vec3 const& relative_position) {

  node.getGuaNode()->apply_impulse(impulse, relative_position);
}

void applyCentralImpulse(av::gua::RigidBodyNode const& node,
                        ::gua::math::vec3 const& impulse) {

  node.getGuaNode()->apply_central_impulse(impulse);
}

void clearForces(av::gua::RigidBodyNode const& node) {

  node.getGuaNode()->clear_forces();
}

void init_RigidBodyNode()
{
  register_field<av::gua::SFRigidBodyNode>("SFRigidBodyNode");
  register_multifield<av::gua::MFRigidBodyNode>("MFRigidBodyNode");

  class_<av::gua::RigidBodyNode,
         av::Link<av::gua::RigidBodyNode>,
         bases<av::gua::Node>, boost::noncopyable >("RigidBodyNode", "docstring", no_init)
         .def("apply_force", &applyForce)
         .def("apply_central_force", &applyCentralForce)
         .def("apply_torque", &applyTorque)
         .def("apply_torque_impulse", &applyTorqueImpulse)
         .def("apply_impulse", &applyImpulse)
         .def("apply_central_impulse", &applyCentralImpulse)
         .def("clear_forces", &clearForces)
         ;

  enum_<av::gua::RigidBodyNode::CollisionFilterGroups>("LoaderFlags")
        .value("DEFAULT_FILTER", av::gua::RigidBodyNode::CollisionFilterGroups::DEFAULT_FILTER)
        .value("STATIC_FILTER", av::gua::RigidBodyNode::CollisionFilterGroups::STATIC_FILTER)
        .value("KINEMATIC_FILTER", av::gua::RigidBodyNode::CollisionFilterGroups::KINEMATIC_FILTER)
        .value("DEBRIS_FILTER", av::gua::RigidBodyNode::CollisionFilterGroups::DEBRIS_FILTER)
        .value("SENSOR_TRIGGER", av::gua::RigidBodyNode::CollisionFilterGroups::SENSOR_TRIGGER)
        .value("CHARACTER_FILTER", av::gua::RigidBodyNode::CollisionFilterGroups::CHARACTER_FILTER)
        .value("ALL_FILTER", av::gua::RigidBodyNode::CollisionFilterGroups::ALL_FILTER)
        ;
}
