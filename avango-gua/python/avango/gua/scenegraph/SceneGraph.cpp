#include "SceneGraph.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#include <avango/gua/scenegraph/PickResult.hpp>
#include <avango/gua/scenegraph/RayNode.hpp>
#include <avango/gua/utils/Ray.hpp>
#include <gua/node/TriMeshNode.hpp>

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

av::gua::MFPickResult* rayTest1(av::gua::SceneGraph const& graph,
                                av::gua::Ray const& ray,
                                int options,
                                std::vector<std::string> const& whitelist,
                                std::vector<std::string> const& blacklist,
                                bool allow_untagged) {

  ::gua::Mask mask;
  for (auto tag: whitelist) {
    mask.whitelist.add_tag(tag);
  }

  for (auto tag: blacklist) {
    mask.blacklist.add_tag(tag);
  }

  mask.allow_untagged = allow_untagged;

  auto gua_results(graph.getGuaSceneGraph()->ray_test(
                                    *(ray.getGuaRay()),
                                    static_cast< ::gua::PickResult::Options>(options),
                                    mask));
  auto results(new av::gua::MFPickResult());
  for (auto result : gua_results) {
    results->add1Value(new av::gua::PickResult(result));
  }

  return results;
}

av::gua::MFPickResult* rayTest2(av::gua::SceneGraph const& graph,
                                av::gua::Ray const& ray,
                                int options,
                                std::vector<std::string> const& whitelist,
                                std::vector<std::string> const& blacklist) {

  return rayTest1(graph, ray, options, whitelist, blacklist, false);
}


av::gua::MFPickResult* rayTest3(av::gua::SceneGraph const& graph,
                                av::gua::Ray const& ray,
                                int options,
                                list const& whitelist,
                                list const& blacklist,
                                bool allow_untagged) {

  ::gua::Mask mask;
  for (int i(0); i < len(whitelist); ++i) {
    mask.whitelist.add_tag(extract<std::string>(whitelist[i]));
  }

  for (int i(0); i < len(blacklist); ++i) {
    mask.blacklist.add_tag(extract<std::string>(blacklist[i]));
  }

  mask.allow_untagged = allow_untagged;

  auto gua_results(graph.getGuaSceneGraph()->ray_test(
                                    *(ray.getGuaRay()),
                                    static_cast< ::gua::PickResult::Options>(options),
                                    mask));
  auto results(new av::gua::MFPickResult());
  for (auto result : gua_results) {
    results->add1Value(new av::gua::PickResult(result));
  }

  return results;
}

av::gua::MFPickResult* rayTest4(av::gua::SceneGraph const& graph,
                                av::gua::Ray const& ray,
                                int options,
                                list const& whitelist,
                                list const& blacklist) {


  return rayTest3(graph, ray, options, whitelist, blacklist, false);
}


av::gua::MFPickResult* rayTest5(av::gua::SceneGraph const& graph,
                                av::gua::Ray const& ray,
                                int options) {

  return rayTest3(graph, ray, options, list(), list(), false);
}




av::gua::MFPickResult* rayNodeTest1(av::gua::SceneGraph const& graph,
                                    av::gua::RayNode const& ray,
                                    int options,
                                    std::vector<std::string> const& whitelist,
                                    std::vector<std::string> const& blacklist,
                                    bool allow_untagged) {

  ::gua::Mask mask;
  for (auto tag: whitelist) {
    mask.whitelist.add_tag(tag);
  }

  for (auto tag: blacklist) {
    mask.blacklist.add_tag(tag);
  }

  mask.allow_untagged = allow_untagged;

  auto gua_results(graph.getGuaSceneGraph()->ray_test(
                                    *(ray.getGuaNode()),
                                    static_cast< ::gua::PickResult::Options>(options),
                                    mask));
  auto results(new av::gua::MFPickResult());
  for (auto result : gua_results) {
    results->add1Value(new av::gua::PickResult(result));
  }

  return results;
}

av::gua::MFPickResult* rayNodeTest2(av::gua::SceneGraph const& graph,
                                    av::gua::RayNode const& ray,
                                    int options,
                                    std::vector<std::string> const& whitelist,
                                    std::vector<std::string> const& blacklist) {


  return rayNodeTest1(graph, ray, options, whitelist, blacklist, false);
}

av::gua::MFPickResult* rayNodeTest3(av::gua::SceneGraph const& graph,
                                    av::gua::RayNode const& ray,
                                    int options,
                                    list const& whitelist,
                                    list const& blacklist,
                                    bool allow_untagged) {

  ::gua::Mask mask;
  for (int i(0); i < len(whitelist); ++i) {
    mask.whitelist.add_tag(extract<std::string>(whitelist[i]));
  }

  for (int i(0); i < len(blacklist); ++i) {
    mask.blacklist.add_tag(extract<std::string>(blacklist[i]));
  }

  mask.allow_untagged = allow_untagged;

  auto gua_results(graph.getGuaSceneGraph()->ray_test(
                                    *(ray.getGuaNode()),
                                    static_cast< ::gua::PickResult::Options>(options),
                                    mask));
  auto results(new av::gua::MFPickResult());
  for (auto result : gua_results) {
    results->add1Value(new av::gua::PickResult(result));
  }

  return results;
}

av::gua::MFPickResult* rayNodeTest4(av::gua::SceneGraph const& graph,
                                    av::gua::RayNode const& ray,
                                    int options,
                                    list const& whitelist,
                                    list const& blacklist) {

  return rayNodeTest3(graph, ray, options, whitelist, blacklist, false);
}


av::gua::MFPickResult* rayNodeTest5(av::gua::SceneGraph const& graph,
                                    av::gua::RayNode const& ray,
                                    int options) {

  return rayNodeTest3(graph, ray, options, list(), list(), false);
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
         .def("ray_test", &rayTest3, return_value_policy<manage_new_object>())
         .def("ray_test", &rayTest4, return_value_policy<manage_new_object>())
         .def("ray_test", &rayTest5, return_value_policy<manage_new_object>())
         .def("ray_test", &rayNodeTest1, return_value_policy<manage_new_object>())
         .def("ray_test", &rayNodeTest2, return_value_policy<manage_new_object>())
         .def("ray_test", &rayNodeTest3, return_value_policy<manage_new_object>())
         .def("ray_test", &rayNodeTest4, return_value_policy<manage_new_object>())
         .def("ray_test", &rayNodeTest5, return_value_policy<manage_new_object>())
         .def("update_cache", &av::gua::SceneGraph::updateCache)
         ;
}
