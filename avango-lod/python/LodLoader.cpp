#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/lod/renderer/LodLoader.hpp>
#include <boost/python/stl_iterator.hpp>
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
    typedef T type;
};
} // namespace python
} // namespace boost

av::Link<av::gua::Node>
loadLodPointcloud1(av::gua::lod::LodLoader const& loader, std::string const& nodeName, std::string const& fileName, av::Link<av::gua::Material> const& fallbackMaterial, int flags)
{
    return loader.loadLodPointcloud(nodeName, fileName, fallbackMaterial, static_cast<av::gua::lod::LodLoader::Flags>(flags));
}

av::Link<av::gua::Node> loadLodPointcloud2(av::gua::lod::LodLoader const& loader, std::string const& fileName, int flags)
{
    return loader.loadLodPointcloud(fileName, static_cast<av::gua::lod::LodLoader::Flags>(flags));
}

av::Link<av::gua::Node> loadLodTrimesh1(av::gua::lod::LodLoader const& loader, std::string const& nodeName, std::string const& fileName, av::Link<av::gua::Material> const& fallbackMaterial, int flags)
{
    return loader.loadLodTrimesh(nodeName, fileName, fallbackMaterial, static_cast<av::gua::lod::LodLoader::Flags>(flags));
}

av::Link<av::gua::Node> loadLodTrimesh2(av::gua::lod::LodLoader const& loader, std::string const& fileName, int flags)
{
    return loader.loadLodTrimesh(fileName, static_cast<av::gua::lod::LodLoader::Flags>(flags));
}

boost::python::tuple pick_plod_bvh(
    av::gua::lod::LodLoader const& loader, ::gua::math::vec3 const& ray_origin, ::gua::math::vec3 const& ray_forward, float max_distance, boost::python::list model_files_list, float aabb_scale)
{
    std::set<std::string> model_filenames;
    boost::python::stl_input_iterator<std::string> a(model_files_list);
    boost::python::stl_input_iterator<std::string> b;
    model_filenames.insert(a, b);

    std::pair<std::string, ::gua::math::vec3> result = loader.pick_plod_bvh(ray_origin, ray_forward, max_distance, model_filenames, aabb_scale);

    return boost::python::make_tuple(result.first, result.second);
}

av::gua::MFPickResult* pick_plod_interp(av::gua::lod::LodLoader const& loader,
                                        ::gua::math::vec3 const& bundle_origin,
                                        ::gua::math::vec3 const& bundle_forward,
                                        ::gua::math::vec3 const& bundle_up,
                                        float bundle_radius,
                                        float max_distance,
                                        unsigned int max_depth,
                                        unsigned int surfel_skip,
                                        float aabb_scale)
{
    return loader.pick_plod_interpolate(bundle_origin, bundle_forward, bundle_up, bundle_radius, max_distance, max_depth, surfel_skip, aabb_scale);
}

void init_LodLoader()
{
    class_<av::gua::lod::LodLoader, av::Link<av::gua::lod::LodLoader>, bases<av::FieldContainer>, boost::noncopyable>("LodLoader", "docstring", no_init)
        .def("load_lod_pointcloud", &loadLodPointcloud1)
        .def("load_lod_pointcloud", &loadLodPointcloud2)
        .def("load_lod_trimesh", &loadLodTrimesh1)
        .def("load_lod_trimesh", &loadLodTrimesh2)
        .def("pick_plod_bvh", &pick_plod_bvh)
        .def("pick_plod_interpolate", &pick_plod_interp, return_value_policy<manage_new_object>());

    enum_<av::gua::lod::LodLoader::Flags>("LoaderFlags")
        .value("DEFAULTS", av::gua::lod::LodLoader::DEFAULTS)
        .value("MAKE_PICKABLE", av::gua::lod::LodLoader::MAKE_PICKABLE)
        .value("NORMALIZE_POSITION", av::gua::lod::LodLoader::NORMALIZE_POSITION)
        .value("NORMALIZE_SCALE", av::gua::lod::LodLoader::NORMALIZE_SCALE);

    register_field<av::gua::lod::SFLodLoader>("SFLodLoader");
    register_multifield<av::gua::lod::MFLodLoader>("MFLodLoader");
}
