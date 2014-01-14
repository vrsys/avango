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

#include "Vec2FieldAdapter.h"
#include <avango/osg/Fields.h>
#include "../../core/include/shade/types/Vec2Accessor.h"
#include "../../core/include/shade/types/vec2.h"

namespace
{
  class Vec2Getter
  {
  public:

    Vec2Getter(shade::types::Vec2Accessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::osg::SFVec2::GetValueEvent& event)
    {
      ::osg::Vec2 result;
      mAccessor->get(result.x(), result.y());
      *event.getValuePtr() = result;
    }

  private:
    shade::types::Vec2Accessor* mAccessor;
  };

  class Vec2Setter
  {
  public:

    Vec2Setter(shade::types::Vec2Accessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::osg::SFVec2::SetValueEvent& event)
    {
      ::osg::Vec2 value(event.getValue());
      mAccessor->set(value.x(), value.y());
    }

  private:
    shade::types::Vec2Accessor* mAccessor;
  };
}

::shade::types::TypeAccessor::HashType
av::shade::Vec2FieldAdapter::hash(void) const
{
  static ::shade::vec2<> value;
  return value.hash();
}

void
av::shade::Vec2FieldAdapter::bindField(::shade::Type* type, const std::string& name, av::FieldContainer* container) const
{
  ::shade::types::Vec2Accessor* vec2_accessor(dynamic_cast< ::shade::types::Vec2Accessor*>(type));
  if (vec2_accessor == 0)
    return;

  av::osg::SFVec2* field= new av::osg::SFVec2;
  field->bind(container, name, true, Vec2Getter(vec2_accessor), Vec2Setter(vec2_accessor));
}
