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

#if not defined(__WIN32__) && not defined(_WIN32) && not defined(_WIN64)
void 
av::gua::NamedSharedMemoryController::add_memory_segment(std::string const& segment_name, uint64_t size_in_byte) {
  m_guaNamedSharedMemoryController->add_memory_segment(segment_name, size_in_byte);
}

void 
av::gua::NamedSharedMemoryController::add_read_only_memory_segment(std::string const& segment_name) {
  m_guaNamedSharedMemoryController->add_read_only_memory_segment(segment_name);
}

void 
av::gua::NamedSharedMemoryController::register_remotely_constructed_object_on_segment(std::string const& segment_name,
                                                                                      std::string const& object_name) {
  m_guaNamedSharedMemoryController->register_remotely_constructed_object_on_segment(segment_name, object_name);
}
#endif