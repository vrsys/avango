#include "SceneGraph.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#include <avango/gua/scenegraph/PickResult.hpp>
#include <avango/gua/scenegraph/RayNode.hpp>
#include <gua/scenegraph/GeometryNode.hpp>

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

av::gua::MFPickResult* rayTest1(av::gua::SceneGraph const& graph, av::gua::RayNode const& ray, int options, std::string const& mask) {
  auto gua_results(graph.getGuaSceneGraph()->ray_test(*(ray.getGuaNode()), static_cast< ::gua::PickResult::Options>(options), mask));
  auto results(new av::gua::MFPickResult());
  for (auto result : gua_results) {
    results->add1Value(new av::gua::PickResult(result));
  }

  return results;
}

av::gua::MFPickResult* rayTest2(av::gua::SceneGraph const& graph, av::gua::RayNode const& ray, int options) {
  return rayTest1(graph, ray, options, "");
}

av::Link<av::gua::Node> getNode(av::gua::SceneGraph const& graph, std::string const& path) {
    auto guaNode((*graph.getGuaSceneGraph())[path]);
    if (guaNode) {
      return av::Link<av::gua::Node>(
                static_cast<av::gua::Node*>(guaNode->get_user_data(0)));
    }
    return av::Link<av::gua::Node>();
}

void init_SceneGraph()
{
  register_field<av::gua::SFSceneGraph>("SFSceneGraph");
  register_multifield<av::gua::MFSceneGraph>("MFSceneGraph");
  class_<av::gua::SceneGraph, av::Link<av::gua::SceneGraph>,
         bases<av::FieldContainer>, boost::noncopyable >("SceneGraph", "docstring", no_init)
         .def("get_node", &getNode)
         .def("__getitem__", &getNode)
         .def("ray_test", &rayTest1, return_value_policy<manage_new_object>())
         .def("ray_test", &rayTest2, return_value_policy<manage_new_object>())
         .def("update_cache", &av::gua::SceneGraph::updateCache)
         ;
}
