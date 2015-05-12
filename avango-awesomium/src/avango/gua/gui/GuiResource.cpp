#include <avango/gua/gui/GuiResource.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <gua/gui.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::gui::GuiResource"));
}

AV_FC_DEFINE(av::gua::gui::GuiResource);

AV_FIELD_DEFINE(av::gua::gui::SFGuiResource);
AV_FIELD_DEFINE(av::gua::gui::MFGuiResource);

av::gua::gui::GuiResource::GuiResource(std::shared_ptr< ::gua::GuiResource> guaGuiResource)
  : av::gua::TransformNode(std::make_shared<::gua::node::TransformNode>()),
    m_guaGuiResource(guaGuiResource),
    m_initialized(false),
    m_distributed(false),
    m_textureName(""),
    m_size(::gua::math::vec2(-1.f)),
    m_clearCallbackHandle()
{
  AV_FC_ADD_ADAPTOR_FIELD(TextureName,
                          boost::bind(&GuiResource::getTextureNameCB, this, _1),
                          boost::bind(&GuiResource::setTextureNameCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(URL,
                          boost::bind(&GuiResource::getURLCB, this, _1),
                          boost::bind(&GuiResource::setURLCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Size,
                          boost::bind(&GuiResource::getSizeCB, this, _1),
                          boost::bind(&GuiResource::setSizeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Interactive,
                          boost::bind(&GuiResource::getInteractiveCB, this, _1),
                          boost::bind(&GuiResource::setInteractiveCB, this, _1));

  AV_FC_ADD_FIELD(m_networkMousePositions, MFVec2::ContainerType());
  AV_FC_ADD_FIELD(m_networkMousePositionsRelative, MFVec2::ContainerType());
  AV_FC_ADD_FIELD(m_networkMouseButtons, MFVec3i::ContainerType());
  AV_FC_ADD_FIELD(m_networkMouseWheelDirections, MFVec2::ContainerType());

  m_clearCallbackHandle = ApplicationInstance::get().addRenderCallback(boost::bind(&GuiResource::clearCallback, this));

}

av::gua::gui::GuiResource::~GuiResource()
{
  ApplicationInstance::get().removeCallback(m_clearCallbackHandle);
}

void
av::gua::gui::GuiResource::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::TransformNode::initClass();

        AV_FC_INIT(av::gua::TransformNode, av::gua::gui::GuiResource, true);

        SFGuiResource::initClass("av::gua::gui::SFGuiResource", "av::Field");
        MFGuiResource::initClass("av::gua::gui::MFGuiResource", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::GuiResource>
av::gua::gui::GuiResource::getGuaGuiResource() const
{
    return m_guaGuiResource;
}

void
av::gua::gui::GuiResource::getTextureNameCB(const SFString::GetValueEvent& event)
{
   *(event.getValuePtr()) = m_textureName;
}

void
av::gua::gui::GuiResource::setTextureNameCB(const SFString::SetValueEvent& event)
{
  if (m_textureName == "") {
    m_textureName = event.getValue();
    if (check_completeness()) {
      init();
    }
  } else {
    ::gua::Logger::LOG_WARNING << "TextureName can only be set once!" << std::endl;
  }
}

void
av::gua::gui::GuiResource::getURLCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaGuiResource->get_url();
}

void
av::gua::gui::GuiResource::setURLCB(const SFString::SetValueEvent& event)
{
    m_guaGuiResource->set_url(event.getValue());
    if (check_completeness()) {
      init();
    }
}


void
av::gua::gui::GuiResource::getSizeCB(const SFVec2::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_size;
}

void
av::gua::gui::GuiResource::setSizeCB(const SFVec2::SetValueEvent& event)
{
  if (m_size == ::gua::math::vec2(-1.f)) {
    m_size = event.getValue();
    if (check_completeness()) {
      init();
    }
  } else {
    ::gua::Logger::LOG_WARNING << "Size can only be set once!" << std::endl;
  }
}
void
av::gua::gui::GuiResource::getInteractiveCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaGuiResource->is_interactive();
}

void
av::gua::gui::GuiResource::setInteractiveCB(const SFBool::SetValueEvent& event)
{
    m_guaGuiResource->set_interactive(event.getValue());
}

void av::gua::gui::GuiResource::fieldHasChangedLocalSideEffect(Field const& field) {
  TransformNode::fieldHasChangedLocalSideEffect(field);

  if (!m_distributed) {
    if (field.getName() == "m_networkMousePositions") {
        for (auto position : m_networkMousePositions.getValue()) {
            m_guaGuiResource->inject_mouse_position(position);
        }
    } else if (field.getName() == "m_networkMousePositionsRelative") {
        for (auto position : m_networkMousePositionsRelative.getValue()) {
            m_guaGuiResource->inject_mouse_position_relative(position);
        }
    } else if (field.getName() == "m_networkMouseButtons") {
        for (auto button : m_networkMouseButtons.getValue()) {
            m_guaGuiResource->inject_mouse_button(::gua::Button(button.x), button.y, button.z);
        }
    } else if (field.getName() == "m_networkMouseWheelDirections") {
        for (auto direction : m_networkMouseWheelDirections.getValue()) {
            m_guaGuiResource->inject_mouse_wheel(direction);
        }
    }
  }
}

bool
av::gua::gui::GuiResource::check_completeness() const{

  return m_guaGuiResource->get_url() != "" &&
         m_textureName != "" &&
         m_size != ::gua::math::vec2(-1.f);
}

void
av::gua::gui::GuiResource::init() {
  if(!m_initialized) {
    m_guaGuiResource->init(m_textureName, m_guaGuiResource->get_url(), m_size);
    m_initialized = true;
    ::gua::Logger::LOG_DEBUG << "Initializing gui resource with name " << m_textureName
              << ", URL " << m_guaGuiResource->get_url() << " and size "
              << m_size << "." << std::endl;
  }
}

void
av::gua::gui::GuiResource::clearCallback() {
  if (m_distributed) {
    m_networkMousePositions.clear();
    m_networkMousePositionsRelative.clear();
    m_networkMouseButtons.clear();
    m_networkMouseWheelDirections.clear();
  }
}

void
av::gua::gui::GuiResource::on_loaded(std::function<void()> const& callback) const {
  m_guaGuiResource->on_loaded.connect(callback);
}

void
av::gua::gui::GuiResource::on_javascript_callback(std::function<void(
                                              std::string const&,
                                              std::vector<std::string> const& )> const& callback) const {
  m_guaGuiResource->on_javascript_callback.connect(callback);
}

void
av::gua::gui::GuiResource::add_javascript_callback(std::string const& name) {
  m_guaGuiResource->add_javascript_callback(name);
}

void
av::gua::gui::GuiResource::add_javascript_getter(std::string const& name, std::function<std::string()> callback) {
  m_guaGuiResource->add_javascript_getter(name, callback);
}

void
av::gua::gui::GuiResource::go_forward() {
  m_guaGuiResource->go_forward();
}

void
av::gua::gui::GuiResource::go_back() {
  m_guaGuiResource->go_back();
}

void
av::gua::gui::GuiResource::go_to_history_offset(int offset) {
  m_guaGuiResource->go_to_history_offset(offset);
}

void
av::gua::gui::GuiResource::reload() {
  m_guaGuiResource->reload();
}

void
av::gua::gui::GuiResource::focus() {
  m_guaGuiResource->focus();
}

void
av::gua::gui::GuiResource::inject_keyboard_event(int key, int scancode, int action, int mods) const {
  m_guaGuiResource->inject_keyboard_event(::gua::Key(key), scancode, action, mods);
}

void
av::gua::gui::GuiResource::inject_char_event(unsigned c) const {
  m_guaGuiResource->inject_char_event(c);
}

void
av::gua::gui::GuiResource::inject_mouse_position_relative(::gua::math::vec2 const& position) const {
  m_guaGuiResource->inject_mouse_position_relative(position);

  if (m_distributed) {
    m_networkMousePositionsRelative.add1Value(position);
  }
}

void
av::gua::gui::GuiResource::inject_mouse_position(::gua::math::vec2 const& position) const {
  m_guaGuiResource->inject_mouse_position(position);

  if (m_distributed) {
    m_networkMousePositions.add1Value(position);
  }
}

void
av::gua::gui::GuiResource::inject_mouse_button(int button, int action, int mods) const {
  m_guaGuiResource->inject_mouse_button(::gua::Button(button), action, mods);

  if (m_distributed) {
    m_networkMouseButtons.add1Value(::gua::math::vec3i(button, action, mods));
  }
}

void
av::gua::gui::GuiResource::inject_mouse_wheel(::gua::math::vec2 const& direction) const {
  m_guaGuiResource->inject_mouse_wheel(direction);

  if (m_distributed) {
    m_networkMouseWheelDirections.add1Value(direction);
  }
}

void
av::gua::gui::GuiResource::call_javascript(std::string const& method, std::vector<std::string> const& args) const {
  m_guaGuiResource->call_javascript(method, args);
}


void av::gua::gui::GuiResource::on_distribute(av::gua::NetTransform& netNode)
{
  TransformNode::on_distribute(netNode);

  m_distributed = true;
}

void av::gua::gui::GuiResource::on_undistribute(av::gua::NetTransform& netNode)
{
  TransformNode::on_undistribute(netNode);

  m_distributed = false;
}
