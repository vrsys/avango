#include <avango/oculus/OculusRift.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
	av::Logger& logger(av::getLogger("av::oculus::OculusRift"));
}

AV_FC_DEFINE(av::oculus::OculusRift);

AV_FIELD_DEFINE(av::oculus::SFOculusRift);
AV_FIELD_DEFINE(av::oculus::MFOculusRift);

av::oculus::OculusRift::OculusRift(::gua::OculusRift* guaOculusRift)
	: av::gua::Window(guaOculusRift),
	  m_guaOculusRift(reinterpret_cast< ::gua::OculusRift*>(av::gua::Window::getGuaWindow()))
{
	AV_FC_ADD_ADAPTOR_FIELD(Transform,
							boost::bind(&OculusRift::getTransformCB, this, _1),
							boost::bind(&OculusRift::setTransformCB, this, _1));
}

av::oculus::OculusRift::~OculusRift()
{}

void
av::oculus::OculusRift::initClass()
{
	if (!isTypeInitialized())
	{
		av::gua::Window::initClass();
		
		AV_FC_INIT(av::gua::Window, av::oculus::OculusRift, true);
		
		SFOculusRift::initClass("av::oculus::SFOculusRift", "av::Field");
		MFOculusRift::initClass("av::oculus::MFOculusRift", "av::Field");
	}
}


::gua::OculusRift*
av::oculus::OculusRift::getGuaOculusRift() const {
	return m_guaOculusRift;
}

void
av::oculus::OculusRift::getTransformCB(const av::gua::SFMatrix::GetValueEvent& event)
{	
	*(event.getValuePtr()) = m_guaOculusRift->get_transform();	
}

void
av::oculus::OculusRift::setTransformCB(const av::gua::SFMatrix::SetValueEvent& event)
{
	//Do nothing as the transformation cannot be set (sensor input)
	return;
}
