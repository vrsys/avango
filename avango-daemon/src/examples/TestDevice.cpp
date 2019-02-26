// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include <avango/daemon/DeviceService.h>
#include <avango/daemon/DeviceSensor.h>
#include <avango/Logger.h>
#include <avango/logging/FileAppender.h>
#include <avango/logging/ConsoleAppender.h>
#include <iostream>

int main()
{
    std::cout << "Reading mouse values ..." << std::endl;

    av::Logger& logger = av::Logger::getLogger("av::daemon::SharedMemorySegment");

    boost::shared_ptr<av::logging::Appender> console_app = av::logging::ConsoleAppender::getInstanceSharedPtr();
    logger.addAppender(console_app);

    osg::Matrixf* identity(new osg::Matrixf());
    av::daemon::DeviceService* serv = new av::daemon::DeviceService;
    av::daemon::DeviceSensor* sens = new av::daemon::DeviceSensor();
    sens->DeviceService.setValue(serv);
    sens->Station.setValue("mouse-station");

    while(true)
    {
        sens->evaluate();

        // output directly from DeviceService
        // std::cout << "Service (Value X):  "<< serv->getValue("mouse-station", 0) << std::endl;
        // std::cout << "Service (Value Y):  "<< serv->getValue("mouse-station", 1) << std::endl;

        // output coming from DeviceSensor
        std::cout << "Sensor (Value X):  " << sens->Value0.getValue() << std::endl;
        std::cout << "Sensor (Value Y):  " << sens->Value1.getValue() << std::endl;
    }

    return 1;
}
