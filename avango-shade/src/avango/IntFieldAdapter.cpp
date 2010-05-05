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

#include "IntFieldAdapter.h"
#include <avango/StandardFields.h>
#include "../../core/include/shade/types/IntAccessor.h"
#include "../../core/include/shade/types/int.h"

namespace
{
  class IntGetter
  {
  public:

    IntGetter(shade::types::IntAccessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::SFInt::GetValueEvent& event)
    {
      mAccessor->get(*event.getValuePtr());
    }

  private:
    shade::types::IntAccessor* mAccessor;
  };

  class IntSetter
  {
  public:

    IntSetter(shade::types::IntAccessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::SFInt::SetValueEvent& event)
    {
      mAccessor->set(event.getValue());
    }

  private:
    shade::types::IntAccessor* mAccessor;
  };
}

::shade::types::TypeAccessor::HashType
av::shade::IntFieldAdapter::hash(void) const
{
  static ::shade::int_<> value;
  return value.hash();
}

void
av::shade::IntFieldAdapter::bindField(::shade::Type* type, const std::string& name, av::FieldContainer* container) const
{
  ::shade::types::IntAccessor* int_accessor(dynamic_cast< ::shade::types::IntAccessor*>(type));
  if (int_accessor == 0)
    return;

  av::SFInt* field= new av::SFInt;
  field->bind(container, name, true, IntGetter(int_accessor), IntSetter(int_accessor));
}
