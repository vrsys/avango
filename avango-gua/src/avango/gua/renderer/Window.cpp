#include <avango/gua/renderer/Window.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Window"));
}

AV_FC_DEFINE(av::gua::Window);

AV_FIELD_DEFINE(av::gua::SFWindow);
AV_FIELD_DEFINE(av::gua::MFWindow);

av::gua::Window::Window(std::shared_ptr< ::gua::Window> const& guaWindow)
    : av::gua::WindowBase(guaWindow)
    , m_guaWindow(guaWindow)
{

  AV_FC_ADD_ADAPTOR_FIELD(SwapGroup,
                      std::bind(&Window::getSwapGroupCB, this,std::placeholders::_1),
                      std::bind(&Window::setSwapGroupCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(SwapBarrier,
                      std::bind(&Window::getSwapBarrierCB, this,std::placeholders::_1),
                      std::bind(&Window::setSwapBarrierCB, this,std::placeholders::_1));
}

void
av::gua::Window::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::WindowBase::initClass();

        AV_FC_INIT(av::gua::WindowBase, av::gua::Window, true);

        SFWindow::initClass("av::gua::SFWindow", "av::Field");
        MFWindow::initClass("av::gua::MFWindow", "av::Field");
    }
}

std::shared_ptr< ::gua::Window> const&
av::gua::Window::getGuaWindow() const
{
    return m_guaWindow;
}

void
av::gua::Window::getSwapGroupCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaWindow->get_swap_group());
}

void
av::gua::Window::setSwapGroupCB(const SFUInt::SetValueEvent& event)
{
  m_guaWindow->join_swap_group(event.getValue());
}

void
av::gua::Window::getSwapBarrierCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaWindow->get_swap_barrier());
}

void
av::gua::Window::setSwapBarrierCB(const SFUInt::SetValueEvent& event)
{
  m_guaWindow->bind_swap_barrier(event.getValue());
}

