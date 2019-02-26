#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/skelanim/renderer/SkeletalAnimationLoader.hpp>

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

av::Link<av::gua::Node> createGeometryFromFile1(
    av::gua::skelanim::SkeletalAnimationLoader const& loader, std::string const& nodeName, std::string const& fileName, av::Link<av::gua::Material> const& fallbackMaterial, int flags)
{
    return loader.createGeometryFromFile(nodeName, fileName, fallbackMaterial, static_cast<av::gua::skelanim::SkeletalAnimationLoader::Flags>(flags));
}

av::Link<av::gua::Node> createGeometryFromFile2(av::gua::skelanim::SkeletalAnimationLoader const& loader, std::string const& nodeName, std::string const& fileName, int flags)
{
    return loader.createGeometryFromFile(nodeName, fileName, static_cast<av::gua::skelanim::SkeletalAnimationLoader::Flags>(flags));
}

void init_SkeletalAnimationLoader()
{
    class_<av::gua::skelanim::SkeletalAnimationLoader, av::Link<av::gua::skelanim::SkeletalAnimationLoader>, bases<av::FieldContainer>, boost::noncopyable>(
        "SkeletalAnimationLoader", "docstring", no_init)
        .def("create_geometry_from_file", &createGeometryFromFile1)
        .def("create_geometry_from_file", &createGeometryFromFile2);

    enum_<av::gua::skelanim::SkeletalAnimationLoader::Flags>("LoaderFlags")
        .value("DEFAULTS", av::gua::skelanim::SkeletalAnimationLoader::DEFAULTS)
        .value("LOAD_MATERIALS", av::gua::skelanim::SkeletalAnimationLoader::LOAD_MATERIALS)
        .value("OPTIMIZE_GEOMETRY", av::gua::skelanim::SkeletalAnimationLoader::OPTIMIZE_GEOMETRY)
        .value("MAKE_PICKABLE", av::gua::skelanim::SkeletalAnimationLoader::MAKE_PICKABLE)
        .value("NORMALIZE_SCALE", av::gua::skelanim::SkeletalAnimationLoader::NORMALIZE_SCALE)
        .value("NORMALIZE_POSITION", av::gua::skelanim::SkeletalAnimationLoader::NORMALIZE_POSITION);

    register_field<av::gua::skelanim::SFSkeletalAnimationLoader>("SFSkeletalAnimationLoader");
    register_multifield<av::gua::skelanim::MFSkeletalAnimationLoader>("MFSkeletalAnimationLoader");
}
