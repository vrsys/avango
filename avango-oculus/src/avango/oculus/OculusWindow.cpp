#include <avango/oculus/OculusWindow.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

#include <iostream>

namespace
{
  av::Logger& logger(av::getLogger("av::oculus::OculusWindow"));
}

AV_FC_DEFINE(av::oculus::OculusWindow);

AV_FIELD_DEFINE(av::oculus::SFOculusWindow);
AV_FIELD_DEFINE(av::oculus::MFOculusWindow);

av::oculus::OculusWindow::OculusWindow(::gua::OculusWindow* guaOculusWindow)
  : av::gua::Window(guaOculusWindow),
    m_guaOculusWindow(reinterpret_cast< ::gua::OculusWindow*>(av::gua::Window::getGuaWindow()))
{
}

av::oculus::OculusWindow::~OculusWindow()
{}

void
av::oculus::OculusWindow::initClass()
{

  if (!isTypeInitialized())
  {
    av::gua::Window::initClass();
    
    AV_FC_INIT(av::gua::Window, av::oculus::OculusWindow, true);
    
    SFOculusWindow::initClass("av::oculus::SFOculusWindow", "av::Field");
    MFOculusWindow::initClass("av::oculus::MFOculusWindow", "av::Field");
  }
}

::gua::OculusWindow*
av::oculus::OculusWindow::getGuaOculusWindow() const {
  return m_guaOculusWindow;
}
