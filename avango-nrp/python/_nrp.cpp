#include <boost/python.hpp>

#include "NRPNode.hpp"
#include "NRPCameraNode.hpp"
#include "NRPPass.hpp"

#include <avango/gua/nrp/NRPNode.hpp>
#include <avango/gua/nrp/NRPCameraNode.hpp>
#include <avango/gua/nrp/NRPPass.hpp>

BOOST_PYTHON_MODULE(_nrp)
{
    av::gua::nrp::NRPNode::initClass();
    av::gua::nrp::NRPCameraNode::initClass();
    av::gua::nrp::NRPPassDescription::initClass();
    init_NRPNode();
    init_NRPCameraNode();
    init_NRPPassDescription();
}
