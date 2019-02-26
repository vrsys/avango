#include "NURBSLoader.hpp"

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <avango/python/register_field.h>
#include <gua/math.hpp>
#include <gua/renderer/NURBS.hpp>
#include <avango/gua/nurbs/renderer/NURBSLoader.hpp>
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

av::Link<av::gua::Node> load(av::gua::nurbs::NURBSLoader const& loader, std::string const& nodeName, std::string const& fileName, av::gua::Material const& fallbackMaterial, int flags)
{
    return loader.load(nodeName, fileName, fallbackMaterial, static_cast<av::gua::nurbs::NURBSLoader::Flags>(flags));
}

/*
boost::python::tuple pick_plod2(
                       av::gua::nurbs::PLODLoader const& loader,
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

av::gua::nurbs::MFPickResult* pick_plod(av::gua::nurbs::PLODLoader const& loader,
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
void init_NURBSLoader()
{
    register_ptr_to_python<av::Link<av::gua::nurbs::NURBSLoader>>();

    class_<av::gua::nurbs::NURBSLoader, av::Link<av::gua::nurbs::NURBSLoader>, bases<av::FieldContainer>, boost::noncopyable>("NURBSLoader", "docstring", no_init)
        .def("load", &load)
        .def("create_geometry_from_file", &load)
        //.def("pick_plod_bvh", &pick_plod2)
        //.def("pick_plod_interpolate", &pick_plod, return_value_policy<manage_new_object>())
        ;

    enum_<av::gua::nurbs::NURBSLoader::Flags>("LoaderFlags")
        .value("DEFAULTS", av::gua::nurbs::NURBSLoader::DEFAULTS)
        .value("MAKE_PICKABLE", av::gua::nurbs::NURBSLoader::MAKE_PICKABLE)
        .value("NORMALIZE_POSITION", av::gua::nurbs::NURBSLoader::NORMALIZE_POSITION)
        .value("NORMALIZE_SCALE", av::gua::nurbs::NURBSLoader::NORMALIZE_SCALE);

    register_field<av::gua::nurbs::SFNURBSLoader>("SFNURBSLoader");
    register_multifield<av::gua::nurbs::MFNURBSLoader>("MFNURBSLoader");
}
