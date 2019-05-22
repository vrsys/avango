// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
 *  Avango Software, Copyright 2001 - 2009 Fraunhofer Gesellschaft        *
 *  zur Foerderung der angewandten Forschung (FhG), Munich, Germany.      *
 *                                                                        *
 *  Avango Software (both binary and source code - if released) is        *
 *  intellectual property owned by FhG and copyrighted by FhG worldwide.  *
 *                                                                        *
 *  Avango is a trademark owned by FhG.                                   *
 *                                                                        *
 \************************************************************************/

#include <avango/vrpn/Device.h>
#include <avango/vrpn/Service.h>
#include <avango/vrpn/Button.h>
#include <avango/vrpn/TrackerInformation.h>
#include <avango/vrpn/Wiimote.h>

#include <avango/python/register_field.h>
#include <boost/python.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
namespace python
{
template <class T>
struct pointee<av::Link<T>>
{
    typedef T type;
};
} // namespace python
} // namespace boost

BOOST_PYTHON_MODULE(_vrpn)
{
    // initialize Avango type, if necessary
    av::vrpn::Device::initClass();
    av::vrpn::Service::initClass();
    av::vrpn::Button::initClass();
    av::vrpn::TrackerInformation::initClass();
    av::vrpn::Wiimote::initClass();

    class_<av::vrpn::Device, av::Link<av::vrpn::Device>, bases<av::FieldContainer>, boost::noncopyable>("Device", "Docstring", no_init);
    register_field<av::vrpn::SFDevice>("SFDevice");
    register_multifield<av::vrpn::MFDevice>("MFDevice");

    class_<av::vrpn::Service, av::Link<av::vrpn::Service>, bases<av::FieldContainer>, boost::noncopyable>("Service", "Docstring", no_init);
    register_field<av::vrpn::SFService>("SFService");
    register_multifield<av::vrpn::MFService>("MFService");

    class_<av::vrpn::Button, av::Link<av::vrpn::Button>, bases<av::FieldContainer>, boost::noncopyable>("Button", "Docstring", no_init);
    register_field<av::vrpn::SFButton>("SFButton");
    register_multifield<av::vrpn::MFButton>("MFButton");

    class_<av::vrpn::TrackerInformation, av::Link<av::vrpn::TrackerInformation>, bases<av::FieldContainer>, boost::noncopyable>("TrackerTrackerInformation", "Docstring", no_init);
    register_field<av::vrpn::SFTrackerInformation>("SFTrackerInformation");
    register_multifield<av::vrpn::MFTrackerInformation>("MFTrackerInformation");

    class_<av::vrpn::Wiimote, av::Link<av::vrpn::Wiimote>, bases<av::FieldContainer>, boost::noncopyable>("Wiimote", "Docstring", no_init);
    register_field<av::vrpn::SFWiimote>("SFWiimote");
    register_multifield<av::vrpn::MFWiimote>("MFWiimote");
}
