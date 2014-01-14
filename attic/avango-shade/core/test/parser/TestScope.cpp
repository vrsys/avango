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

#include "../../src/parser/Scope.h"
#include "../../src/parser/ScopeLayer.h"
#include "../../src/parser/ReferenceValue.h"
#include <UnitTest++.h>

using namespace shade::parser;

SUITE(TestScope)
{
  TEST(SettingLocals)
  {
    std::string name("value");
    boost::shared_ptr<ReferenceValue> value(new ReferenceValue(42));

    ScopeLayer global;
    Scope scope(global);

    scope.add_value(name, value);

    const ReferenceValue* set_value = dynamic_cast<const ReferenceValue*>(scope.get_value(name).get());
    CHECK(set_value);
    if (set_value)
      CHECK_EQUAL(42u, set_value->get());

    const ReferenceValue* unset_value = dynamic_cast<const ReferenceValue*>(global.get_value(name).get());
    CHECK(!unset_value);
  }

  TEST(GettingGlobals)
  {
    std::string name("value");
    boost::shared_ptr<ReferenceValue> value(new ReferenceValue(42));

    ScopeLayer global;
    Scope scope(global);

    global.add_value(name, value);

    const ReferenceValue* set_value = dynamic_cast<const ReferenceValue*>(scope.get_value(name).get());
    CHECK(set_value);
    if (set_value)
      CHECK_EQUAL(42u, set_value->get());
  }
}
