#include "NRPNode.hpp"
#include "NRPPass.hpp"

#include <avango/gua/nrp/NRPNode.hpp>
#include <avango/gua/nrp/NRPPass.hpp>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

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
}
}

BOOST_PYTHON_MODULE(_nrp)
{
    av::gua::nrp::NRPNode::initClass();
    av::gua::nrp::NRPPassDescription::initClass();
    init_NRPNode();
    init_NRPPassDescription();
}
