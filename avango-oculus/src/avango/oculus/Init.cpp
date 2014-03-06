#include <avango/oculus/Init.hpp>

#include <avango/Logger.h>

AV_TYPED_DEFINE_ABSTRACT(av::oculus::Init);

/* static */ void
av::oculus::Init::initClass()
{
    if (!isTypeInitialized())
    {
	    av::oculus::OculusWindow::initClass();

      AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::oculus::Init", true);
    }
}
