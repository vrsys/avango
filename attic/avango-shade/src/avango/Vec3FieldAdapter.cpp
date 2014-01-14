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

#include "Vec3FieldAdapter.h"
#include <avango/osg/Fields.h>
#include "../../core/include/shade/types/Vec3Accessor.h"
#include "../../core/include/shade/types/vec3.h"

namespace
{
  class Vec3Getter
  {
  public:

    Vec3Getter(shade::types::Vec3Accessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::osg::SFVec3::GetValueEvent& event)
    {
      ::osg::Vec3 result;
      mAccessor->get(result.x(), result.y(), result.z());
      *event.getValuePtr() = result;
    }

  private:
    shade::types::Vec3Accessor* mAccessor;
  };

  class Vec3Setter
  {
  public:

    Vec3Setter(shade::types::Vec3Accessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::osg::SFVec3::SetValueEvent& event)
    {
      ::osg::Vec3 value(event.getValue());
      mAccessor->set(value.x(), value.y(), value.z());
    }

  private:
    shade::types::Vec3Accessor* mAccessor;
  };
}

::shade::types::TypeAccessor::HashType
av::shade::Vec3FieldAdapter::hash(void) const
{
  static ::shade::vec3<> value;
  return value.hash();
}

void
av::shade::Vec3FieldAdapter::bindField(::shade::Type* type, const std::string& name, av::FieldContainer* container) const
{
  ::shade::types::Vec3Accessor* vec3_accessor(dynamic_cast< ::shade::types::Vec3Accessor*>(type));
  if (vec3_accessor == 0)
    return;

  av::osg::SFVec3* field= new av::osg::SFVec3;
  field->bind(container, name, true, Vec3Getter(vec3_accessor), Vec3Setter(vec3_accessor));
}
