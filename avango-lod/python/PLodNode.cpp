#include "PLodNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/lod/scenegraph/PLodNode.hpp>

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

void update_cursor_position(av::gua::lod::PLodNode const& plod_node,
					  	    float elapsed_frame_time_ms) {
	plod_node.update_cursor_position(elapsed_frame_time_ms);
}

av::gua::SFMatrix* get_active_time_series_transform(av::gua::lod::PLodNode const& plod_node) {
	return plod_node.get_active_time_series_transform();
}

av::SFFloat* get_current_time_step(av::gua::lod::PLodNode const& plod_node) {
	return plod_node.get_current_time_step();
}

av::SFInt* get_number_of_simulation_positions(av::gua::lod::PLodNode const& plod_node) {
	return plod_node.get_number_of_simulation_positions();
}

av::gua::MFVec3* get_current_simulation_positions(av::gua::lod::PLodNode const& plod_node) {
	return plod_node.get_current_simulation_positions();
}

void init_PLodNode()
{
    register_field<av::gua::lod::SFPLodNode>("SFPLodNode");
    register_multifield<av::gua::lod::MFPLodNode>("MFPLodNode");

    class_<av::gua::lod::PLodNode, av::Link<av::gua::lod::PLodNode>, bases<av::gua::GeometryNode>, boost::noncopyable>("PLodNode", "docstring", no_init)
        .def("update_cursor_position", &update_cursor_position)
        .def("get_active_time_series_transform", &get_active_time_series_transform, return_value_policy<manage_new_object>())
        .def("get_current_time_step", &get_current_time_step, return_value_policy<manage_new_object>())
        .def("get_number_of_simulation_positions", &get_number_of_simulation_positions, return_value_policy<manage_new_object>())
        .def("get_current_simulation_positions", &get_current_simulation_positions, return_value_policy<manage_new_object>())
        ;
}
