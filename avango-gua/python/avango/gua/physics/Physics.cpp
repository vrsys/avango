#include "Physics.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/Physics.hpp>

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

static boost::python::object last_func;
static std::mutex call_mutex;

void setLastFunc(boost::python::object const& function) {
  std::lock_guard<std::mutex> lock(call_mutex);
  last_func = function;
}

void callback() {
  std::lock_guard<std::mutex> lock(call_mutex);
  last_func();
}

/// TODO: Still not thread-safe
void callOnce(av::gua::Physics& physics, boost::python::object const& function) {
  setLastFunc(function);

  // auto fun = [&]() {
  //   std::lock_guard<std::mutex> lock(call_mutex);
  //   last_func();
  // };

  physics.getGuaPhysics()->call_once(callback);
}

void init_Physics()
{
  register_field<av::gua::SFPhysics>("SFPhysics");
  register_multifield<av::gua::MFPhysics>("MFPhysics");

  class_<av::gua::Physics,
         av::Link<av::gua::Physics>,
         bases<av::FieldContainer>, boost::noncopyable >("Physics",
                                                    "docstring", no_init)
         .def("synchronize", &av::gua::Physics::synchronize)
         .def("add_rigid_body", &av::gua::Physics::addRigidBody)
         .def("remove_rigid_body", &av::gua::Physics::removeRigidBody)
         .def("add_constraint", &av::gua::Physics::addConstraint)
         .def("remove_constraint", &av::gua::Physics::removeConstraint)
         .def("get_physics_fps", &av::gua::Physics::getPhysicsFPS)
         .def("call_once", &callOnce)
         ;

  enum_<av::gua::Physics::RunningState>("PhysicsRunningState")
        .value("RUNNING", av::gua::Physics::RunningState::RUNNING)
        .value("PAUSED", av::gua::Physics::RunningState::PAUSED)
        .value("STOPPED", av::gua::Physics::RunningState::STOPPED)
        ;
}
