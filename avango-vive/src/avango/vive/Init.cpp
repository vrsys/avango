#include <avango/vive/Init.hpp>

#include <avango/Logger.h>

AV_TYPED_DEFINE_ABSTRACT(av::vive::Init);

/* static */ void
av::vive::Init::initClass()
{
    if (!isTypeInitialized())
    {
      av::vive::ViveWindow::initClass();

      AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::vive::Init", true);
    }
}
