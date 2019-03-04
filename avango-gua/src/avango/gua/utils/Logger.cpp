#include <avango/gua/utils/Logger.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::Logger);

AV_FIELD_DEFINE(av::gua::SFLogger);
AV_FIELD_DEFINE(av::gua::MFLogger);

av::gua::Logger::Logger() : av::FieldContainer()
{
    AV_FC_ADD_ADAPTOR_FIELD(EnableDebug, std::bind(&Logger::getEnableDebugCB, this, std::placeholders::_1), std::bind(&Logger::setEnableDebugCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableMessage, std::bind(&Logger::getEnableMessageCB, this, std::placeholders::_1), std::bind(&Logger::setEnableMessageCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableWarning, std::bind(&Logger::getEnableWarningCB, this, std::placeholders::_1), std::bind(&Logger::setEnableWarningCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableError, std::bind(&Logger::getEnableErrorCB, this, std::placeholders::_1), std::bind(&Logger::setEnableErrorCB, this, std::placeholders::_1));
}

// av::gua::Logger::~Logger()
// {}

void av::gua::Logger::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Logger, true);

        SFLogger::initClass("av::gua::SFLogger", "av::Field");
        MFLogger::initClass("av::gua::MFLogger", "av::Field");
    }
}

void av::gua::Logger::getEnableDebugCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = ::gua::Logger::enable_debug; }

void av::gua::Logger::setEnableDebugCB(const SFBool::SetValueEvent& event) { ::gua::Logger::enable_debug = event.getValue(); }

void av::gua::Logger::getEnableMessageCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = ::gua::Logger::enable_message; }

void av::gua::Logger::setEnableMessageCB(const SFBool::SetValueEvent& event) { ::gua::Logger::enable_message = event.getValue(); }

void av::gua::Logger::getEnableWarningCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = ::gua::Logger::enable_warning; }

void av::gua::Logger::setEnableWarningCB(const SFBool::SetValueEvent& event) { ::gua::Logger::enable_warning = event.getValue(); }

void av::gua::Logger::getEnableErrorCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = ::gua::Logger::enable_error; }

void av::gua::Logger::setEnableErrorCB(const SFBool::SetValueEvent& event) { ::gua::Logger::enable_error = event.getValue(); }
