#include <boost/python.hpp>

#include <avango/Init.h>
#include <avango/Config.h>

#include <avango/vive/Init.hpp>
#include <avango/python/register_field.h>

#include "ViveWindow.hpp"

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

BOOST_PYTHON_MODULE(_vive)
{
    PyEval_InitThreads();

    av::vive::Init::initClass();

    init_ViveWindow();
}
