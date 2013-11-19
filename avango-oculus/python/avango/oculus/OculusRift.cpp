#include "OculusRift.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/oculus/OculusRift.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
	namespace python
	{
		template<class T> struct pointee<av::Link<T> >
		{
			typedef T type;
		};
	}
}

void init_OculusRift()
{
	register_field<av::oculus::SFOculusRift>("SFOculusRift");
	register_multifield<av::oculus::MFOculusRift>("MFOculusRift");

	class_<av::oculus::OculusRift,
		   av::Link<av::oculus::OculusRift>,
		   bases<av::gua::Window>, boost::noncopyable >("OculusRift","docstring",no_init)
        ;

        def("initOVR", &av::oculus::OculusRift::initOVR);
}
