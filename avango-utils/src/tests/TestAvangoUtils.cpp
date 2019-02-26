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

#include <avango/osg/Types.h>
#include <avango/osg/Init.h>
#include <avango/Application.h>
#include <avango/Logger.h>
#include <avango/StandardFields.h>
#include <avango/utils/MultiValueField.h>

#include <avango/UnitTest++/UnitTest++.h>

int main()
{
    av::getRootLogger().addConsoleAppender();
    av::ApplicationInstance::get();
    av::osg::Init::initClass();
    av::utils::initMultiValueFields();
    av::utils::initMultiValueOSGFields();

    return UnitTest::RunAllTests();
}
