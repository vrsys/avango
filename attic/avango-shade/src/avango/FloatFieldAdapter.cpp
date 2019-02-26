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

#include "FloatFieldAdapter.h"
#include <avango/StandardFields.h>
#include "../../core/include/shade/types/FloatAccessor.h"
#include "../../core/include/shade/types/float.h"

namespace
{
class FloatGetter
{
  public:
    FloatGetter(shade::types::FloatAccessor* accessor) : mAccessor(accessor) {}

    void operator()(const av::SFFloat::GetValueEvent& event) { mAccessor->get(*event.getValuePtr()); }

  private:
    shade::types::FloatAccessor* mAccessor;
};

class FloatSetter
{
  public:
    FloatSetter(shade::types::FloatAccessor* accessor) : mAccessor(accessor) {}

    void operator()(const av::SFFloat::SetValueEvent& event) { mAccessor->set(event.getValue()); }

  private:
    shade::types::FloatAccessor* mAccessor;
};
} // namespace

::shade::types::TypeAccessor::HashType av::shade::FloatFieldAdapter::hash(void) const
{
    static ::shade::float_<> value;
    return value.hash();
}

void av::shade::FloatFieldAdapter::bindField(::shade::Type* type, const std::string& name, av::FieldContainer* container) const
{
    ::shade::types::FloatAccessor* float_accessor(dynamic_cast<::shade::types::FloatAccessor*>(type));
    if(float_accessor == 0)
        return;

    av::SFFloat* field = new av::SFFloat;
    field->bind(container, name, true, FloatGetter(float_accessor), FloatSetter(float_accessor));
}
