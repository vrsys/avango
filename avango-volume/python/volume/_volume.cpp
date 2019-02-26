#include <boost/python.hpp>

#include <avango/Init.h>
#include <avango/Config.h>

#include <avango/gua/volume/Init.hpp>
#include <avango/python/register_field.h>

#include "VolumeLoader.hpp"
#include "VolumeNode.hpp"
#include "VolumePassDescription.hpp"

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

BOOST_PYTHON_MODULE(_volume)
{
    PyEval_InitThreads();

    av::volume::Init::initClass();

    init_VolumeNode();
    init_VolumePassDescription();
    init_VolumeLoader();
}
