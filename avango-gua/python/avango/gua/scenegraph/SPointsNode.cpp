#include "SPointsNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/scenegraph/SPointsNode.hpp>

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

/*
av::gua::MFPickResult* pick_plod(av::gua::PLODLoader const& loader,
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

void fetch_statistics(av::gua::SPointsNode& node) { node.fetchLatestSPointsStats(); }

int get_stats_num_triangles(av::gua::SPointsNode const& node) { return node.getStatNumTrianglesReceived(); }

float get_stats_timestamp(av::gua::SPointsNode const& node) { return node.getStatTimestampReceived(); }

float get_stats_reconstruction_time(av::gua::SPointsNode const& node) { return node.getStatReconTimeReceived(); }

float get_stats_request_reply_latency(av::gua::SPointsNode const& node) { return node.getStatRequestReplyLatencyReceived(); }

int get_stats_total_message_payload_in_byte(av::gua::SPointsNode const& node) { return node.getStatTotaMessagePayloadInByteReceived(); }

void init_SPointsNode()
{
    register_field<av::gua::SFSPointsNode>("SFSPointsNode");
    register_multifield<av::gua::MFSPointsNode>("MFSPointsNode");
    class_<av::gua::SPointsNode, av::Link<av::gua::SPointsNode>, bases<av::gua::GeometryNode>, boost::noncopyable>("SPointsNode", "docstring", no_init)
        .def("fetch_stats", &fetch_statistics)
        .def("get_stats_num_triangles", &get_stats_num_triangles)
        .def("get_stats_timestamp", &get_stats_timestamp)
        .def("get_stats_reconstruction_time", &get_stats_reconstruction_time)
        .def("get_stats_request_reply_latency", &get_stats_request_reply_latency)
        .def("get_stats_total_message_payload_in_byte", &get_stats_total_message_payload_in_byte);
}
