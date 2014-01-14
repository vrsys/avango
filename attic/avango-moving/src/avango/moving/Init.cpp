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

#include <avango/moving/Init.h>

#include <avango/Logger.h>

#include <avango/moving/Trackball.h>
#include <avango/moving/DesktopFlyer.h>

namespace
{
  av::Logger& logger(av::getLogger("av::moving::Init"));
}

AV_TYPED_DEFINE_ABSTRACT(av::moving::Init);

/* static */ void
av::moving::Init::initClass()
{
  if (!isTypeInitialized())
  {
    av::moving::Trackball::initClass();
    av::moving::DesktopFlyer::initClass();

    AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::moving::Init", true);
  }
}
