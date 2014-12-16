#include "PipelineDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/PipelineDescription.hpp>

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

av::Link<av::gua::PipelineDescription> create_default_pipeline_description() {
  return av::Link<av::gua::PipelineDescription>(
          new av::gua::PipelineDescription(
            std::make_shared<::gua::PipelineDescription>(::gua::PipelineDescription::make_default())
          )
        );
}

list get_tri_mesh_passes(av::gua::PipelineDescription& desc) {
    list result;
    auto passes = desc.get_tri_mesh_passes();

    for (auto& pass : passes){
      result.append(pass);
    }
    return result;
}

list get_background_passes(av::gua::PipelineDescription& desc) {
    list result;
    auto passes = desc.get_background_passes();

    for (auto& pass : passes){
      result.append(pass);
    }
    return result;
}

list get_bbox_passes(av::gua::PipelineDescription& desc) {
    list result;
    auto passes = desc.get_bbox_passes();

    for (auto& pass : passes){
      result.append(pass);
    }
    return result;
}

list get_emissive_passes(av::gua::PipelineDescription& desc) {
    list result;
    auto passes = desc.get_emissive_passes();

    for (auto& pass : passes){
      result.append(pass);
    }
    return result;
}

list get_textured_screen_space_quad_passes(av::gua::PipelineDescription& desc) {
    list result;
    auto passes = desc.get_textured_screen_space_quad_passes();

    for (auto& pass : passes){
      result.append(pass);
    }
    return result;
}

list get_textured_quad_passes(av::gua::PipelineDescription& desc) {
    list result;
    auto passes = desc.get_textured_quad_passes();

    for (auto& pass : passes){
      result.append(pass);
    }
    return result;
}

list get_fullscreen_passes(av::gua::PipelineDescription& desc) {
    list result;
    auto passes = desc.get_fullscreen_passes();

    for (auto& pass : passes){
      result.append(pass);
    }
    return result;
}

void init_PipelineDescription()
 {

  register_field<av::gua::SFPipelineDescription>("SFPipelineDescription");
  register_multifield<av::gua::MFPipelineDescription>("MFPipelineDescription");
  class_<av::gua::PipelineDescription,
         av::Link<av::gua::PipelineDescription>,
         bases<av::FieldContainer>, boost::noncopyable >("PipelineDescription", "docstring", no_init)
         .def("add_tri_mesh_pass", &av::gua::PipelineDescription::add_tri_mesh_pass)
         .def("get_tri_mesh_pass", &av::gua::PipelineDescription::get_tri_mesh_pass)
         .def("get_tri_mesh_passes", get_tri_mesh_passes)
         .def("add_textured_quad_pass", &av::gua::PipelineDescription::add_textured_quad_pass)
         .def("get_textured_quad_pass", &av::gua::PipelineDescription::get_textured_quad_pass)
         .def("get_textured_quad_passes", get_textured_quad_passes)
         .def("add_background_pass", &av::gua::PipelineDescription::add_background_pass)
         .def("get_background_pass", &av::gua::PipelineDescription::get_background_pass)
         .def("get_background_passes", &av::gua::PipelineDescription::get_background_passes)
         .def("add_bbox_pass", &av::gua::PipelineDescription::add_bbox_pass)
         .def("get_bbox_pass", &av::gua::PipelineDescription::get_bbox_pass)
         .def("get_bbox_passes", get_bbox_passes)
         .def("add_emissive_pass", &av::gua::PipelineDescription::add_emissive_pass)
         .def("get_emissive_pass", &av::gua::PipelineDescription::get_emissive_pass)
         .def("get_emissive_passes", get_emissive_passes)
         .def("add_textured_screen_space_quad_pass", &av::gua::PipelineDescription::add_textured_screen_space_quad_pass)
         .def("get_textured_screen_space_quad_pass", &av::gua::PipelineDescription::get_textured_screen_space_quad_pass)
         .def("get_textured_screen_space_quad_passes", get_textured_screen_space_quad_passes)
         .def("add_fullscreen_pass", &av::gua::PipelineDescription::add_fullscreen_pass)
         .def("get_fullscreen_pass", &av::gua::PipelineDescription::get_fullscreen_pass)
         .def("get_fullscreen_passes", get_fullscreen_passes)
         ;

  def("create_default_pipeline_description", &create_default_pipeline_description);
 }
