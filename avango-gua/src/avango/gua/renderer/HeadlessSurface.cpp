#include <avango/gua/renderer/HeadlessSurface.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::HeadlessSurface"));
}

AV_FC_DEFINE(av::gua::HeadlessSurface);

AV_FIELD_DEFINE(av::gua::SFHeadlessSurface);
AV_FIELD_DEFINE(av::gua::MFHeadlessSurface);

av::gua::HeadlessSurface::HeadlessSurface(std::shared_ptr< ::gua::HeadlessSurface> const& guaWindow)
    : av::gua::WindowBase(guaWindow)
    , m_guaWindow(guaWindow)
{}

void
av::gua::HeadlessSurface::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::WindowBase::initClass();

        AV_FC_INIT(av::gua::WindowBase, av::gua::HeadlessSurface, true);

        SFHeadlessSurface::initClass("av::gua::SFHeadlessSurface", "av::Field");
        MFHeadlessSurface::initClass("av::gua::MFHeadlessSurface", "av::Field");
    }
}

std::shared_ptr< ::gua::HeadlessSurface> const&
av::gua::HeadlessSurface::getGuaWindow() const
{
    return m_guaWindow;
}
