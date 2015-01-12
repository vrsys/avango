#include <avango/gua/gui/GuiResource.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <gua/gui.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::GuiResource"));
}

AV_FC_DEFINE(av::gua::GuiResource);

AV_FIELD_DEFINE(av::gua::SFGuiResource);
AV_FIELD_DEFINE(av::gua::MFGuiResource);

av::gua::GuiResource::GuiResource(std::shared_ptr< ::gua::GuiResource> guaGuiResource)
  : av::FieldContainer(),
    m_guaGuiResource(guaGuiResource)
{
  AV_FC_ADD_ADAPTOR_FIELD(URL,
                          boost::bind(&GuiResource::getURLCB, this, _1),
                          boost::bind(&GuiResource::setURLCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Interactive,
                          boost::bind(&GuiResource::getInteractiveCB, this, _1),
                          boost::bind(&GuiResource::setInteractiveCB, this, _1));

}

av::gua::GuiResource::~GuiResource()
{}

void
av::gua::GuiResource::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::GuiResource, true);

        SFGuiResource::initClass("av::gua::SFGuiResource", "av::Field");
        MFGuiResource::initClass("av::gua::MFGuiResource", "av::Field");
    }
}

std::shared_ptr< ::gua::GuiResource>
av::gua::GuiResource::getGuaGuiResource() const
{
    return m_guaGuiResource;
}


void
av::gua::GuiResource::getURLCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaGuiResource->get_url();
}

void
av::gua::GuiResource::setURLCB(const SFString::SetValueEvent& event)
{
    m_guaGuiResource->set_url(event.getValue());
}

void
av::gua::GuiResource::getInteractiveCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaGuiResource->is_interactive();
}

void
av::gua::GuiResource::setInteractiveCB(const SFBool::SetValueEvent& event)
{
    m_guaGuiResource->set_interactive(event.getValue());
}

void
av::gua::GuiResource::init(std::string const& name, std::string const& url,
                           ::gua::math::vec2 const& size) {
  m_guaGuiResource->init(name, url, size);
}

void
av::gua::GuiResource::on_loaded(std::function<void()> const& callback) const {
  m_guaGuiResource->on_loaded.connect(callback);
}

void
av::gua::GuiResource::on_javascript_callback(std::function<void(
                                              std::string const&,
                                              std::vector<std::string> const& )> const& callback) const {
  m_guaGuiResource->on_javascript_callback.connect(callback);
}

void
av::gua::GuiResource::add_javascript_callback(std::string const& name) {
  m_guaGuiResource->add_javascript_callback(name);
}

void
av::gua::GuiResource::add_javascript_getter(std::string const& name, std::function<std::string()> callback) {
  m_guaGuiResource->add_javascript_getter(name, callback);
}

void
av::gua::GuiResource::go_forward() {
  m_guaGuiResource->go_forward();
}

void
av::gua::GuiResource::go_back() {
  m_guaGuiResource->go_back();
}

void
av::gua::GuiResource::go_to_history_offset(int offset) {
  m_guaGuiResource->go_to_history_offset(offset);
}

void
av::gua::GuiResource::reload() {
  m_guaGuiResource->reload();
}

void
av::gua::GuiResource::focus() {
  m_guaGuiResource->focus();
}

void
av::gua::GuiResource::inject_keyboard_event(int key, int scancode, int action, int mods) const {
  m_guaGuiResource->inject_keyboard_event(::gua::Key(key), scancode, action, mods);
}

void
av::gua::GuiResource::inject_char_event(unsigned c) const {
  m_guaGuiResource->inject_char_event(c);
}

void
av::gua::GuiResource::inject_mouse_position_relative(::gua::math::vec2 const& position) const {
  m_guaGuiResource->inject_mouse_position_relative(position);
}

void
av::gua::GuiResource::inject_mouse_position(::gua::math::vec2 const& position) const {
  m_guaGuiResource->inject_mouse_position(position);
}

void
av::gua::GuiResource::inject_mouse_button(int button, int action, int mods) const {
  m_guaGuiResource->inject_mouse_button(::gua::Button(button), action, mods);
}

void
av::gua::GuiResource::inject_mouse_wheel(::gua::math::vec2 const& direction) const {
  m_guaGuiResource->inject_mouse_wheel(direction);
}

void
av::gua::GuiResource::call_javascript(std::string const& method, std::vector<std::string> const& args) const {
  m_guaGuiResource->call_javascript(method, args);
}
