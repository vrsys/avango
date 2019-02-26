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

#include "Vec4FieldAdapter.h"
#include <avango/osg/Fields.h>
#include "../../core/include/shade/types/Vec4Accessor.h"
#include "../../core/include/shade/types/vec4.h"

namespace
{
class Vec4Getter
{
  public:
    Vec4Getter(shade::types::Vec4Accessor* accessor) : mAccessor(accessor) {}

    void operator()(const av::osg::SFVec4::GetValueEvent& event)
    {
        ::osg::Vec4 result;
        mAccessor->get(result.x(), result.y(), result.z(), result.w());
        *event.getValuePtr() = result;
    }

  private:
    shade::types::Vec4Accessor* mAccessor;
};

class Vec4Setter
{
  public:
    Vec4Setter(shade::types::Vec4Accessor* accessor) : mAccessor(accessor) {}

    void operator()(const av::osg::SFVec4::SetValueEvent& event)
    {
        ::osg::Vec4 value(event.getValue());
        mAccessor->set(value.x(), value.y(), value.z(), value.w());
    }

  private:
    shade::types::Vec4Accessor* mAccessor;
};
} // namespace

::shade::types::TypeAccessor::HashType av::shade::Vec4FieldAdapter::hash(void) const
{
    static ::shade::vec4<> value;
    return value.hash();
}

void av::shade::Vec4FieldAdapter::bindField(::shade::Type* type, const std::string& name, av::FieldContainer* container) const
{
    ::shade::types::Vec4Accessor* vec4_accessor(dynamic_cast<::shade::types::Vec4Accessor*>(type));
    if(vec4_accessor == 0)
        return;

    av::osg::SFVec4* field = new av::osg::SFVec4;
    field->bind(container, name, true, Vec4Getter(vec4_accessor), Vec4Setter(vec4_accessor));
}
