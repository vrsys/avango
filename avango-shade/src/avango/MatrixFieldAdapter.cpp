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

#include "MatrixFieldAdapter.h"
#include <avango/osg/Fields.h>
#include "../../core/include/shade/types/Matrix4Accessor.h"
#include "../../core/include/shade/types/matrix4.h"

namespace
{
  class MatrixGetter
  {
  public:

    MatrixGetter(shade::types::Matrix4Accessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::osg::SFMatrix::GetValueEvent& event)
    {
      float values[16];
      mAccessor->get(values);
      ::osg::Matrix result(values);
      *event.getValuePtr() = result;
    }

  private:
    shade::types::Matrix4Accessor* mAccessor;
  };

  class MatrixSetter
  {
  public:

    MatrixSetter(shade::types::Matrix4Accessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::osg::SFMatrix::SetValueEvent& event)
    {
      ::osg::Matrix value(event.getValue());
      float vs[16];
      for (int i = 0; i != 16; ++i)
        vs[i] = value.ptr()[i];
      mAccessor->set(vs);
    }

  private:
    shade::types::Matrix4Accessor* mAccessor;
  };
}

::shade::types::TypeAccessor::HashType
av::shade::MatrixFieldAdapter::hash(void) const
{
  static ::shade::matrix4<> value;
  return value.hash();
}

void
av::shade::MatrixFieldAdapter::bindField(::shade::Type* type, const std::string& name, av::FieldContainer* container) const
{
  ::shade::types::Matrix4Accessor* mat4_accessor(dynamic_cast< ::shade::types::Matrix4Accessor*>(type));
  if (mat4_accessor == 0)
    return;

  av::osg::SFMatrix* field = new av::osg::SFMatrix;
  field->bind(container, name, true, MatrixGetter(mat4_accessor), MatrixSetter(mat4_accessor));
}
