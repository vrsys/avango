#include <avango/gua/gui/GuiResourceNode.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <gua/gui.hpp>

#include <sstream>

namespace
{
  const std::string callback_delimiter("§$&&$§");

  av::Logger& logger(av::getLogger("av::gua::gui::GuiResourceNode"));
}


AV_FC_DEFINE(av::gua::gui::GuiResourceNode);

AV_FIELD_DEFINE(av::gua::gui::SFGuiResourceNode);
AV_FIELD_DEFINE(av::gua::gui::MFGuiResourceNode);

av::gua::gui::GuiResourceNode::GuiResourceNode(std::shared_ptr< ::gua::GuiResource> guaGuiResource)
  : av::gua::TransformNode(std::make_shared<::gua::node::TransformNode>()),
    m_guaGuiResource(guaGuiResource),
    m_initialized(false),
    m_distributed(false),
    m_textureName(""),
    m_size(::gua::math::vec2(-1.f)),
    m_clearCallbackHandle()
{
  AV_FC_ADD_ADAPTOR_FIELD(TextureName,
                          boost::bind(&GuiResourceNode::getTextureNameCB, this, _1),
                          boost::bind(&GuiResourceNode::setTextureNameCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(URL,
                          boost::bind(&GuiResourceNode::getURLCB, this, _1),
                          boost::bind(&GuiResourceNode::setURLCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Size,
                          boost::bind(&GuiResourceNode::getSizeCB, this, _1),
                          boost::bind(&GuiResourceNode::setSizeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Interactive,
                          boost::bind(&GuiResourceNode::getInteractiveCB, this, _1),
                          boost::bind(&GuiResourceNode::setInteractiveCB, this, _1));

  AV_FC_ADD_FIELD(m_networkMousePositions, MFVec2::ContainerType());
  AV_FC_ADD_FIELD(m_networkMousePositionsRelative, MFVec2::ContainerType());
  AV_FC_ADD_FIELD(m_networkKeyboardEvent, MFVec4i::ContainerType());
  AV_FC_ADD_FIELD(m_networkCharEvent, MFUInt::ContainerType());
  AV_FC_ADD_FIELD(m_networkMouseButtons, MFVec3i::ContainerType());
  AV_FC_ADD_FIELD(m_networkMouseWheelDirections, MFVec2::ContainerType());
  AV_FC_ADD_FIELD(m_networkJavascriptCalls, MFString::ContainerType());
  AV_FC_ADD_FIELD(m_networkActionEvent, MFInt::ContainerType());
  AV_FC_ADD_FIELD(m_networkHistoryEvent, MFInt::ContainerType());

  m_clearCallbackHandle = ApplicationInstance::get().addRenderCallback(boost::bind(&GuiResourceNode::clearCallback, this));

}

av::gua::gui::GuiResourceNode::~GuiResourceNode()
{
  ApplicationInstance::get().removeCallback(m_clearCallbackHandle);
}

void
av::gua::gui::GuiResourceNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::TransformNode::initClass();

        AV_FC_INIT(av::gua::TransformNode, av::gua::gui::GuiResourceNode, true);

        SFGuiResourceNode::initClass("av::gua::gui::SFGuiResource", "av::Field");
        MFGuiResourceNode::initClass("av::gua::gui::MFGuiResource", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::GuiResource>
av::gua::gui::GuiResourceNode::getGuaGuiResource() const
{
    return m_guaGuiResource;
}

void
av::gua::gui::GuiResourceNode::getTextureNameCB(const SFString::GetValueEvent& event)
{
   *(event.getValuePtr()) = m_textureName;
}

void
av::gua::gui::GuiResourceNode::setTextureNameCB(const SFString::SetValueEvent& event)
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
av::gua::gui::GuiResourceNode::getURLCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaGuiResource->get_url();
}

void
av::gua::gui::GuiResourceNode::setURLCB(const SFString::SetValueEvent& event)
{
    m_guaGuiResource->set_url(event.getValue());
    if (check_completeness()) {
      init();
    }
}


void
av::gua::gui::GuiResourceNode::getSizeCB(const SFVec2::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_size;
}

void
av::gua::gui::GuiResourceNode::setSizeCB(const SFVec2::SetValueEvent& event)
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
av::gua::gui::GuiResourceNode::getInteractiveCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaGuiResource->is_interactive();
}

void
av::gua::gui::GuiResourceNode::setInteractiveCB(const SFBool::SetValueEvent& event)
{
    m_guaGuiResource->set_interactive(event.getValue());
}

void av::gua::gui::GuiResourceNode::fieldHasChangedLocalSideEffect(Field const& field) {
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
    } else if (field.getName() == "m_networkKeyboardEvent") {
        for (auto e : m_networkKeyboardEvent.getValue()) {
            m_guaGuiResource->inject_keyboard_event(::gua::Key(e.x), e.y, e.z, e.w);
        }
    } else if (field.getName() == "m_networkCharEvent") {
        for (auto e : m_networkCharEvent.getValue()) {
            m_guaGuiResource->inject_char_event(e);
        }
    } else if (field.getName() == "m_networkMouseButtons") {
        for (auto button : m_networkMouseButtons.getValue()) {
            m_guaGuiResource->inject_mouse_button(::gua::Button(button.x), button.y, button.z);
        }
    } else if (field.getName() == "m_networkMouseWheelDirections") {
        for (auto direction : m_networkMouseWheelDirections.getValue()) {
            m_guaGuiResource->inject_mouse_wheel(direction);
        }
    } else if (field.getName() == "m_networkJavascriptCalls") {
        for (auto callback : m_networkJavascriptCalls.getValue()) {
          size_t current_pos = 0;
          size_t last_pos = 0;
          std::vector<std::string> tokens;
          while ((current_pos = callback.find(callback_delimiter, last_pos)) != std::string::npos) {
              tokens.push_back(callback.substr(last_pos, current_pos - last_pos));
              last_pos = current_pos + callback_delimiter.length();
          }
          tokens.push_back(callback.substr(last_pos, std::string::npos));
          if (tokens.size() == 1) {
            m_guaGuiResource->call_javascript(tokens[0]);
          } else {
            m_guaGuiResource->call_javascript_arg_vector(tokens[0], std::vector<std::string>(tokens.begin()+1, tokens.end()));
          }
        }
    } else if (field.getName() == "m_networkActionEvent") {
        for (auto action : m_networkActionEvent.getValue()) {
            if      (action == 0) m_guaGuiResource->go_forward();
            else if (action == 1) m_guaGuiResource->go_back();
            else if (action == 2) m_guaGuiResource->reload();
            else if (action == 3) m_guaGuiResource->focus();
        }
    } else if (field.getName() == "m_networkHistoryEvent") {
        for (auto offset : m_networkHistoryEvent.getValue()) {
            m_guaGuiResource->go_to_history_offset(offset);
        }
    }
  }
}

bool
av::gua::gui::GuiResourceNode::check_completeness() const{

  return m_guaGuiResource->get_url() != "" &&
         m_textureName != "" &&
         m_size != ::gua::math::vec2(-1.f);
}

void
av::gua::gui::GuiResourceNode::init() {
  if(!m_initialized) {
    m_guaGuiResource->init(m_textureName, m_guaGuiResource->get_url(), m_size);
    m_initialized = true;
  }
}

void
av::gua::gui::GuiResourceNode::clearCallback() {
  if (m_distributed) {
    m_networkMousePositions.clear();
    m_networkMousePositionsRelative.clear();
    m_networkKeyboardEvent.clear();
    m_networkCharEvent.clear();
    m_networkMouseButtons.clear();
    m_networkMouseWheelDirections.clear();
    m_networkJavascriptCalls.clear();
    m_networkActionEvent.clear();
    m_networkHistoryEvent.clear();
  }
}

void
av::gua::gui::GuiResourceNode::on_loaded(std::function<void()> const& callback) const {
  m_guaGuiResource->on_loaded.connect(callback);
}

void
av::gua::gui::GuiResourceNode::on_javascript_callback(std::function<void(
                                              std::string const&,
                                              std::vector<std::string> const& )> const& callback) const {
  m_guaGuiResource->on_javascript_callback.connect(callback);
}

void
av::gua::gui::GuiResourceNode::add_javascript_callback(std::string const& name) {
  m_guaGuiResource->add_javascript_callback(name);
}

void
av::gua::gui::GuiResourceNode::add_javascript_getter(std::string const& name, std::function<std::string()> callback) {
  m_guaGuiResource->add_javascript_getter(name, callback);
}

void
av::gua::gui::GuiResourceNode::go_forward() {
  m_guaGuiResource->go_forward();

  if (m_distributed) {
    m_networkActionEvent.add1Value(0);
  }
}

void
av::gua::gui::GuiResourceNode::go_back() {
  m_guaGuiResource->go_back();

  if (m_distributed) {
    m_networkActionEvent.add1Value(1);
  }
}

void
av::gua::gui::GuiResourceNode::go_to_history_offset(int offset) {
  m_guaGuiResource->go_to_history_offset(offset);

  if (m_distributed) {
    m_networkHistoryEvent.add1Value(offset);
  }
}

void
av::gua::gui::GuiResourceNode::reload() {
  m_guaGuiResource->reload();

  if (m_distributed) {
    m_networkActionEvent.add1Value(2);
  }
}

void
av::gua::gui::GuiResourceNode::focus() {
  m_guaGuiResource->focus();

  if (m_distributed) {
    m_networkActionEvent.add1Value(3);
  }
}

void
av::gua::gui::GuiResourceNode::inject_keyboard_event(int key, int scancode, int action, int mods) const {
  m_guaGuiResource->inject_keyboard_event(::gua::Key(key), scancode, action, mods);

  if (m_distributed) {
    m_networkKeyboardEvent.add1Value(::gua::math::vec4i(key, scancode, action, mods));
  }
}

void
av::gua::gui::GuiResourceNode::inject_char_event(unsigned c) const {
  m_guaGuiResource->inject_char_event(c);

  if (m_distributed) {
    m_networkCharEvent.add1Value(c);
  }
}

void
av::gua::gui::GuiResourceNode::inject_mouse_position_relative(::gua::math::vec2 const& position) const {
  m_guaGuiResource->inject_mouse_position_relative(position);

  if (m_distributed) {
    m_networkMousePositionsRelative.add1Value(position);
  }
}

void
av::gua::gui::GuiResourceNode::inject_mouse_position(::gua::math::vec2 const& position) const {
  m_guaGuiResource->inject_mouse_position(position);

  if (m_distributed) {
    m_networkMousePositions.add1Value(position);
  }
}

void
av::gua::gui::GuiResourceNode::inject_mouse_button(int button, int action, int mods) const {
  m_guaGuiResource->inject_mouse_button(::gua::Button(button), action, mods);

  if (m_distributed) {
    m_networkMouseButtons.add1Value(::gua::math::vec3i(button, action, mods));
  }
}

void
av::gua::gui::GuiResourceNode::inject_mouse_wheel(::gua::math::vec2 const& direction) const {
  m_guaGuiResource->inject_mouse_wheel(direction);

  if (m_distributed) {
    m_networkMouseWheelDirections.add1Value(direction);
  }
}

void
av::gua::gui::GuiResourceNode::call_javascript(std::string const& method, std::vector<std::string> const& args) const {
  m_guaGuiResource->call_javascript_arg_vector(method, args);

  if (m_distributed) {
    std::stringstream callStringStream;

    callStringStream << method;

    for (auto const& arg : args) {
      callStringStream << callback_delimiter << arg;
    }

    m_networkJavascriptCalls.add1Value(callStringStream.str());
  }
}


void av::gua::gui::GuiResourceNode::on_distribute(av::gua::NetTransform& netNode)
{
  TransformNode::on_distribute(netNode);

  m_distributed = true;
}

void av::gua::gui::GuiResourceNode::on_undistribute(av::gua::NetTransform& netNode)
{
  TransformNode::on_undistribute(netNode);

  m_distributed = false;
}
