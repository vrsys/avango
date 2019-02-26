// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <boost/python.hpp>

#include <avango/Link.h>
#include <avango/ContainerPool.h>
//#include <avango/osg/viewer/View.h>

#include <gua/math.hpp>
//#include <osgViewer/View>
//#include <osgViewer/ViewerEventHandlers>
//#include <osg/Version>

#ifdef PCL_SUPPORT
#include "UTILSPcd.h"
#endif

#include "../include/avango/utils/Init.h"
#include "../include/avango/utils/Bool2Or.h"
#include "../include/avango/utils/Bool2And.h"
#include "../include/avango/utils/Bool3Or.h"
#include "../include/avango/utils/Bool3And.h"
#include "../include/avango/utils/Trackball.h"
#include "../include/avango/utils/MultiValueField.h"
#include "../include/avango/utils/register_multivaluefield.h"

#include <iostream>
#include <iomanip>

#include <map>

using namespace boost::python;

// TODO can we move this somewhere else?
namespace boost
{
namespace python
{
template <class T>
struct pointee<av::Link<T>>
{
    using type = T;
};
} // namespace python
} // namespace boost

template <typename A, typename B>
std::pair<B, A> flip(std::pair<A, B> const& p)
{
    return std::make_pair(p.second, p.first);
}

template <typename T, typename U>
std::multimap<U, T> converseMap(const std::map<T, U>& o)
{
    std::multimap<U, T> result;
    for(auto const& pair : o)
        result.insert(flip(pair));
    return result;
}

gua::math::vec3 CalcHpr(const gua::math::mat4& mat)
{
    // if(mat(1,0)>0.998) { // (row, column)
    if(mat.row(1)[0] > 0.998f)
    {
        float heading = atan2(mat.row(0)[2], mat.row(2)[2]);
        float attitude = 3.14f / 2.0f;
        float bank = 0;
        return gua::math::vec3(heading, attitude, bank);
    }

    if(mat.row(1)[0] < -0.998f)
    {
        float heading = atan2(mat.row(0)[2], mat.row(2)[2]);
        float attitude = -3.14f / 2.0f;
        float bank = 0;
        return gua::math::vec3(heading, attitude, bank);
    }

    float heading = atan2(-mat.row(2)[0], mat.row(0)[0]);
    float attitude = atan2(-mat.row(1)[2], mat.row(1)[1]);
    float bank = sin(mat.row(1)[0]);
    return gua::math::vec3(heading, attitude, bank);
}

void print_actual_registered_field_containers()
{
    const std::map<av::FieldContainer::IDType, av::FieldContainer*>& containers = av::ContainerPool::getContainerPool();
    std::map<std::string, int> m;
    int maxLength = 0;
    for(auto const& c : containers)
    {
        //    std::string type = ::av::ContainerPool::getNameByInstance(iter->second);
        //    std::string type = iter->second->Name.getValue();
        std::string type = c.second->getTypeId().getName();

        int l = type.length();
        if(l > maxLength)
            maxLength = l;
        m[type]++;
    }

    std::ostringstream ss;
    std::multimap<int, std::string> conversedMap = converseMap(m);
    for(auto const& pair : conversedMap)
    {
        int l = maxLength - pair.second.length();
        ss << "\"" << pair.second << "\" ";
        for(int n = 0; n < l; ++n)
            ss << " ";
        ss << pair.first << std::endl;
    }
    for(int n = 0; n < maxLength + 2; ++n)
        ss << " ";
    ss << "-------" << std::endl;
    std::string total_desc = "Total number of containers:";
    int l = maxLength - total_desc.length();
    ss << "\"" << total_desc << "\" ";
    for(int n = 0; n < l; ++n)
        ss << " ";
    ss << av::ContainerPool::getNumberOfContainers() << std::endl;
    std::cout << ss.str();
}

void init_MultiValueFields()
{
    av::python::register_multivaluefield<av::utils::MVFBool>("MVFBool");
    av::python::register_multivaluefield<av::utils::MVFDouble>("MVFDouble");
    av::python::register_multivaluefield<av::utils::MVFFloat>("MVFFloat");
    av::python::register_multivaluefield<av::utils::MVFInt>("MVFInt");
    av::python::register_multivaluefield<av::utils::MVFLong>("MVFLong");
    av::python::register_multivaluefield<av::utils::MVFUInt>("MVFUInt");
    av::python::register_multivaluefield<av::utils::MVFULong>("MVFULong");
    av::python::register_multivaluefield<av::utils::MVFString>("MVFString");

    // gua related multi value fields
    av::python::register_multivaluefield<av::utils::MVFMatrix>("MVFMatrix");
    av::python::register_multivaluefield<av::utils::MVFVec2>("MVFVec2");
    av::python::register_multivaluefield<av::utils::MVFVec3>("MVFVec3");
    av::python::register_multivaluefield<av::utils::MVFVec4>("MVFVec4");
    av::python::register_multivaluefield<av::utils::MVFQuat>("MVFQuat");
}

// #if OSG_VERSION_MAJOR == 3
// void addScreenCaptureHandler(av::osg::viewer::View * avView, std::string folder, std::string filename, int numFrames) {
//   ::osgViewer::View * view = avView->getOsgView();
//   osgViewer::ScreenCaptureHandler* scnsvr = new osgViewer::ScreenCaptureHandler(new osgViewer::ScreenCaptureHandler::WriteToFile(folder+"/"+filename,"png"));
//   scnsvr->setFramesToCapture(numFrames);
//   scnsvr->startCapture();
//   view->addEventHandler(scnsvr);
// }
// elif OSG_VERSION_MAJOR == 2
// void addScreenCaptureHandler(av::osg::viewer::View * avView, std::string folder, std::string filename, int numFrames) {
//   std::cerr << "addScreenCaptureHandler::ERROR Screen capture functionality is currently only supported with osg3.x" << std::endl;
// }
// #endif

BOOST_PYTHON_MODULE(_utils)
{
    av::utils::Init::initClass();

    class_<av::utils::Bool2Or, av::Link<av::utils::Bool2Or>, bases<av::FieldContainer>, boost::noncopyable>("Bool2Or", "docstring", no_init);
    class_<av::utils::Bool2And, av::Link<av::utils::Bool2And>, bases<av::FieldContainer>, boost::noncopyable>("Bool2And", "docstring", no_init);
    class_<av::utils::Bool3Or, av::Link<av::utils::Bool3Or>, bases<av::FieldContainer>, boost::noncopyable>("Bool3Or", "docstring", no_init);
    class_<av::utils::Bool3And, av::Link<av::utils::Bool3And>, bases<av::FieldContainer>, boost::noncopyable>("Bool3And", "docstring", no_init);

    class_<av::utils::Trackball, av::Link<av::utils::Trackball>, bases<av::FieldContainer>, boost::noncopyable>("Trackball", "docstring", no_init);

    def("calc_hpr", CalcHpr);

    def("print_registered_field_containers", print_actual_registered_field_containers);

    // def("add_screen_capture_handler",addScreenCaptureHandler);

    av::utils::initMultiValueFields();
    av::utils::initMultiValueOSGFields();
    init_MultiValueFields();

#ifdef PCL_SUPPORT
    init_PCD();
#endif
}
