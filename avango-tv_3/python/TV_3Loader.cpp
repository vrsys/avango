#include "TV_3Loader.hpp"

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <avango/python/register_field.h>
#include <gua/math.hpp>
#include <gua/renderer/TV_3.hpp>
#include <avango/gua/tv_3/renderer/TV_3Loader.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#include <avango/gua/scenegraph/PickResult.hpp>
#include <avango/gua/utils/Ray.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
namespace python
{
template <class T>
struct pointee<av::Link<T>>
{
    using type = T;
};
} // namespace python
} // namespace boost

av::Link<av::gua::Node> load(av::gua::tv_3::TV_3Loader const& loader, std::string const& nodeName, std::string const& fileName, int flags, int const cpu_budget, int const gpu_budget)
{
    return loader.load(nodeName, fileName, static_cast<av::gua::tv_3::TV_3Loader::Flags>(flags), cpu_budget, gpu_budget);
}

av::Link<av::gua::Node> load2(av::gua::tv_3::TV_3Loader const& loader, std::string const& nodeName, std::string const& fileName, int flags, int const cpu_budget)
{
    return loader.load(nodeName, fileName, static_cast<av::gua::tv_3::TV_3Loader::Flags>(flags), cpu_budget, 1024);
}

av::Link<av::gua::Node> load3(av::gua::tv_3::TV_3Loader const& loader, std::string const& nodeName, std::string const& fileName, int flags)
{
    return loader.load(nodeName, fileName, static_cast<av::gua::tv_3::TV_3Loader::Flags>(flags), 1024, 1024);
}

av::Link<av::gua::Node> load4(av::gua::tv_3::TV_3Loader const& loader, std::string const& nodeName, std::string const& fileName)
{
    return loader.load(nodeName, fileName, static_cast<av::gua::tv_3::TV_3Loader::Flags>(0), 1024, 1024);
}

av::Link<av::gua::Node> load5(
    av::gua::tv_3::TV_3Loader const& loader, std::string const& nodeName, std::string const& fileName, av::gua::Material const& fallbackMaterial, int flags, int const cpu_budget, int const gpu_budget)
{
    return loader.load(nodeName, fileName, fallbackMaterial, static_cast<av::gua::tv_3::TV_3Loader::Flags>(flags), cpu_budget, gpu_budget);
}

av::Link<av::gua::Node>
load6(av::gua::tv_3::TV_3Loader const& loader, std::string const& nodeName, std::string const& fileName, av::gua::Material const& fallbackMaterial, int flags, int const cpu_budget)
{
    return loader.load(nodeName, fileName, fallbackMaterial, static_cast<av::gua::tv_3::TV_3Loader::Flags>(flags), cpu_budget, 1024);
}

av::Link<av::gua::Node> load7(av::gua::tv_3::TV_3Loader const& loader, std::string const& nodeName, std::string const& fileName, av::gua::Material const& fallbackMaterial, int flags)
{
    return loader.load(nodeName, fileName, fallbackMaterial, static_cast<av::gua::tv_3::TV_3Loader::Flags>(flags), 1024, 1024);
}

av::Link<av::gua::Node> load8(av::gua::tv_3::TV_3Loader const& loader, std::string const& nodeName, std::string const& fileName, av::gua::Material const& fallbackMaterial)
{
    return loader.load(nodeName, fileName, fallbackMaterial, static_cast<av::gua::tv_3::TV_3Loader::Flags>(0), 1024, 1024);
}

bool is_supported(av::gua::tv_3::TV_3Loader const& loader, std::string const& file) { return loader.is_supported(file); }

/*
boost::python::tuple pick_plod2(
                       av::gua::tv_3::PLODLoader const& loader,
                       ::gua::math::vec3 const& ray_origin,
                       ::gua::math::vec3 const& ray_forward,
                       float max_distance,
                       boost::python::list model_files_list,
                       float aabb_scale) {

  std::set<std::string> model_filenames;
  boost::python::stl_input_iterator<std::string> a(model_files_list);
  boost::python::stl_input_iterator<std::string> b;
  model_filenames.insert(a, b);

  std::pair<std::string, ::gua::math::vec3> result = loader.pick_plod_bvh(
                                                            ray_origin,
                                                            ray_forward,
                                                            max_distance,
                                                            model_filenames,
                                                            aabb_scale);

  return boost::python::make_tuple(result.first, result.second);

}

av::gua::tv_3::MFPickResult* pick_plod(av::gua::tv_3::PLODLoader const& loader,
                                 ::gua::math::vec3 const& bundle_origin,
                                 ::gua::math::vec3 const& bundle_forward,
                                 ::gua::math::vec3 const& bundle_up,
                                 float bundle_radius,
                                 float max_distance,
                                 unsigned int max_depth,
                                 unsigned int surfel_skip,
                                 float aabb_scale) {

  return loader.pick_plod_interpolate(bundle_origin,
                                      bundle_forward,
                                      bundle_up,
                                      bundle_radius,
                                      max_distance,
                                      max_depth,
                                      surfel_skip,
                                      aabb_scale);

}
*/
void init_TV_3Loader()
{
    register_ptr_to_python<av::Link<av::gua::tv_3::TV_3Loader>>();

    class_<av::gua::tv_3::TV_3Loader, av::Link<av::gua::tv_3::TV_3Loader>, bases<av::FieldContainer>, boost::noncopyable>("TV_3Loader", "docstring", no_init)
        .def("load", &load)
        .def("load", &load2)
        .def("load", &load3)
        .def("load", &load4)
        .def("load", &load5)
        .def("load", &load6)
        .def("load", &load7)
        .def("load", &load8)
        .def("create_geometry_from_file", &load)
        .def("create_geometry_from_file", &load2)
        .def("create_geometry_from_file", &load3)
        .def("create_geometry_from_file", &load4)
        .def("create_geometry_from_file", &load5)
        .def("create_geometry_from_file", &load6)
        .def("create_geometry_from_file", &load7)
        .def("create_geometry_from_file", &load8)
        .def("is_supported", &is_supported)
        //.def("pick_plod_bvh", &pick_plod2)
        //.def("pick_plod_interpolate", &pick_plod, return_value_policy<manage_new_object>())
        ;

    enum_<av::gua::tv_3::TV_3Loader::Flags>("LoaderFlags")
        .value("DEFAULTS", av::gua::tv_3::TV_3Loader::DEFAULTS)
        .value("MAKE_PICKABLE", av::gua::tv_3::TV_3Loader::MAKE_PICKABLE)
        .value("NORMALIZE_POSITION", av::gua::tv_3::TV_3Loader::NORMALIZE_POSITION)
        .value("NORMALIZE_SCALE", av::gua::tv_3::TV_3Loader::NORMALIZE_SCALE)
        .value("USE_SURFACE_MODE", av::gua::tv_3::TV_3Loader::USE_SURFACE_MODE);

    register_field<av::gua::tv_3::SFTV_3Loader>("SFTV_3Loader");
    register_multifield<av::gua::tv_3::MFTV_3Loader>("MFTV_3Loader");
}
