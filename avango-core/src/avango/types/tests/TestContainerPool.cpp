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

#include <avango/Application.h>
#include <avango/ContainerPool.h>
#include <avango/Link.h>
#include <avango/ObjectValue.h>

#include <avango/UnitTest++/UnitTest++.h>

namespace
{
TEST(registerContainers)
{
    av::BoolObject::initClass();
    av::Link<av::BoolObject> obj1(new av::BoolObject);
    av::FieldContainer::IDType obj1_id(obj1->getId());
    CHECK_EQUAL(obj1.getPtr(), av::ContainerPool::getContainerById(obj1_id));
    obj1.clear();
    CHECK_EQUAL(obj1.getPtr(), av::ContainerPool::getContainerById(obj1_id));
}

} // namespace

int main()
{
    av::getRootLogger().addConsoleAppender();
    av::ApplicationInstance::get();
    return UnitTest::RunAllTests();
}
