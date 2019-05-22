// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 2007 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include "../types/inout.h"

template <class Derived, class Base>
shade::TypeBase<Derived, Base>::TypeBase(void) : Base()
{
}

template <class Derived, class Base>
shade::TypeBase<Derived, Base>::TypeBase(formatter::Constants::Type type) : Base(type)
{
}

template <class Derived, class Base>
boost::shared_ptr<shade::Type> shade::TypeBase<Derived, Base>::clone(void) const
{
    boost::shared_ptr<Derived> retval(new Derived);
    return retval;
}
