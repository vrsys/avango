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

// include i/f header

#include "avango/Action.h"

// includes, system

#include <cstdlib>

// includes, project

#include <avango/logging/Logger.h>

// internal unnamed namespace

namespace
{
// types, internal

// variables, internal

av::Logger& logger(av::getLogger("av::Action"));

// functions, internal

} // namespace

// variables, exported

// functions, exported

AV_BASE_DEFINE(av::Action);

av::Action::Action() {}

/* virtual */
av::Action::~Action() {}

/* static */ void av::Action::initClass()
{
    if(!isTypeInitialized())
    {
        Base::initClass();
        AV_BASE_INIT_ABSTRACT(av::Base, av::Action, true);
    }
}

/* virtual */ void av::Action::apply(Link<Base> node) { traverse(node); }

/* virtual */ void av::Action::traverse(Link<Base> node) { node->doAction(*this); }
