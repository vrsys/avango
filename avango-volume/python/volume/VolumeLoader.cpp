#include "VolumeLoader.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/volume/VolumeLoader.hpp>

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

av::Link<av::gua::Node> load(
    av::gua::VolumeLoader const& loader,
    std::string const& nodename,
    std::string const& fileName,
    int flags) {
  return loader.load(nodename, fileName,
       static_cast<av::gua::VolumeLoader::Flags>(flags));
}

void init_VolumeLoader()
{
  class_<av::gua::VolumeLoader,
         av::Link<av::gua::VolumeLoader>,
         bases<av::FieldContainer>, boost::noncopyable> ("VolumeLoader", "docstring", no_init)
         .def("load", &load)
         .def("create_geometry_from_file", &load)
         ;

  enum_<av::gua::VolumeLoader::Flags>("VolumeLoaderFlags")
        .value("DEFAULTS", av::gua::VolumeLoader::DEFAULTS)
        .value("MAKE_PICKABLE", av::gua::VolumeLoader::MAKE_PICKABLE)
        .value("NORMALIZE_SCALE", av::gua::VolumeLoader::NORMALIZE_SCALE)
        .value("NORMALIZE_POSITION", av::gua::VolumeLoader::NORMALIZE_POSITION)
        ;
  register_field<av::gua::SFVolumeLoader>("SFVolumeLoader");
  register_multifield<av::gua::MFVolumeLoader>("MFVolumeLoader");
}
