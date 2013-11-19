#include <avango/oculus/Init.hpp>

#include <avango/Logger.h>

AV_TYPED_DEFINE_ABSTRACT(av::oculus::Init);

/* static */ void
av::oculus::Init::initClass()
{
    if (!isTypeInitialized())
    {
        char** argv;
        //::gua::init(0, argv);

        //av::gua::initFields();

	av::oculus::OculusRift::initClass();

        AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::oculus::Init", true);
    }
}
