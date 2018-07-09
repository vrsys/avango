#include <avango/gua/utils/NamedSharedMemoryController.hpp>
#include <avango/Base.h>
#include <functional>


std::shared_ptr< ::gua::NamedSharedMemoryController> 
av::gua::NamedSharedMemoryController::m_guaNamedSharedMemoryController   
  = ::gua::NamedSharedMemoryController::instance_shared_ptr();

AV_FC_DEFINE(av::gua::NamedSharedMemoryController);

AV_FIELD_DEFINE(av::gua::SFNamedSharedMemoryController);
AV_FIELD_DEFINE(av::gua::MFNamedSharedMemoryController);

av::gua::NamedSharedMemoryController::NamedSharedMemoryController(/*std::shared_ptr< ::gua::NamedSharedMemoryController> guaNamedSharedMemoryController)
    : m_guaNamedSharedMemoryController(guaNamedSharedMemoryController*/)
{
  /*
    AV_FC_ADD_ADAPTOR_FIELD(Origin,
                          std::bind(&NamedSharedMemoryController::getOriginCB, this, std::placeholders::_1),
                          std::bind(&NamedSharedMemoryController::setOriginCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(Direction,
                          std::bind(&NamedSharedMemoryController::getDirectionCB, this, std::placeholders::_1),
                          std::bind(&NamedSharedMemoryController::setDirectionCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(TMax,
                          std::bind(&NamedSharedMemoryController::getTMaxCB, this, std::placeholders::_1),
                          std::bind(&NamedSharedMemoryController::setTMaxCB, this, std::placeholders::_1));
  */
}


void
av::gua::NamedSharedMemoryController::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::NamedSharedMemoryController, true);

        SFNamedSharedMemoryController::initClass("av::gua::SFNamedSharedMemoryController", "av::Field");
        MFNamedSharedMemoryController::initClass("av::gua::MFNamedSharedMemoryController", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::NamedSharedMemoryController>
av::gua::NamedSharedMemoryController::getGuaNamedSharedMemoryController() const
{
    return m_guaNamedSharedMemoryController;
}

void 
av::gua::NamedSharedMemoryController::add_memory_segment(std::string const& segment_name, uint64_t size_in_byte) {
  m_guaNamedSharedMemoryController->add_memory_segment(segment_name, size_in_byte);
}
/*
av::gua::NamedSharedMemorySegment
av::gua::NamedSharedMemoryController::get_memory_segment(std::string const& segment_name) {
  auto retrieved_memory_segment = m_guaNamedSharedMemoryController->get_memory_segment(segment_name);

  return av::gua::NamedSharedMemorySegment(retrieved_memory_segment);
}
*/
/*
void
av::gua::NamedSharedMemoryController::getOriginCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNamedSharedMemoryController->origin_;
}

void
av::gua::NamedSharedMemoryController::setOriginCB(const SFVec3::SetValueEvent& event)
{
    m_guaNamedSharedMemoryController->origin_ = event.getValue();
}

void
av::gua::NamedSharedMemoryController::getDirectionCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNamedSharedMemoryController->direction_;
}

void
av::gua::NamedSharedMemoryController::setDirectionCB(const SFVec3::SetValueEvent& event)
{
    m_guaNamedSharedMemoryController->direction_ = event.getValue();
}

void
av::gua::NamedSharedMemoryController::getTMaxCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaNamedSharedMemoryController->t_max_;
}

void
av::gua::NamedSharedMemoryController::setTMaxCB(const SFFloat::SetValueEvent& event)
{
    m_guaNamedSharedMemoryController->t_max_ = event.getValue();
}
*/