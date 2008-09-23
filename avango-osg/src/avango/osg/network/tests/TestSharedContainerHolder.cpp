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

#include <avango/UnitTest++/UnitTest++.h>
#include <avango/osg/network/SharedContainerHolder.h>

namespace
{
  class InitContainerFixture
  {
  public:
    InitContainerFixture()
    {
      av::osg::SharedContainerHolder::initClass();
    }
  };

  TEST_FIXTURE(InitContainerFixture, createSharedContainerHolder)
  {
    av::Link<av::osg::SharedContainerHolder> holder(new av::osg::SharedContainerHolder);
    CHECK(holder.isValid());
  }

  TEST_FIXTURE(InitContainerFixture, createSharedContainerHolderByName)
  {
    av::Link<av::osg::SharedContainerHolder>
      holder(dynamic_cast<av::osg::SharedContainerHolder*>(av::Type::createInstanceOfType("av::osg::SharedContainerHolder")));
    CHECK(holder.isValid());
  }

}
