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

#include "ListFieldAdapter.h"
#include "../include/avango/shade/Shader.h"
#include <avango/FieldContainer.h>
#include "../../core/include/shade/types/ListAccessor.h"
#include "../../core/include/shade/Shader.h"
#include "../../core/include/shade/types/list.h"

namespace
{
  class ListGetter
  {
  public:

    ListGetter(shade::types::ListAccessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::MFContainer::GetValueEvent& event)
    {
      av::MFContainer::ContainerType& container(*event.getValuePtr());
      ::shade::types::ListAccessor::Range range(mAccessor->get());
      for (::shade::types::ListAccessor::InIterator in = range.first; in != range.second; ++in)
      {
        container.push_back(av::shade::Shader::getWrapper((*in).get()));
      }
    }

  private:
    shade::types::ListAccessor* mAccessor;
  };

  class ListSetter
  {
  public:

    ListSetter(shade::types::ListAccessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::MFContainer::SetValueEvent& event)
    {
      const av::MFContainer::ContainerType& container(event.getValue());
      ::shade::types::ListAccessor::OutIterator out(mAccessor->set());
      for (av::MFContainer::ContainerType::const_iterator in = container.begin(); in != container.end(); ++in, ++out)
      {
        av::shade::Shader* shader(dynamic_cast<av::shade::Shader*>(in->getBasePtr()));
        if (!shader)
          continue;
        *out = shader->getShader();
      }
    }

  private:
    shade::types::ListAccessor* mAccessor;
  };
}

::shade::types::TypeAccessor::HashType
av::shade::ListFieldAdapter::hash(void) const
{
  static ::shade::list<std::vector<boost::shared_ptr< ::shade::Shader> > > value;
  return value.hash();
}

void
av::shade::ListFieldAdapter::bindField(::shade::Type* type, const std::string& name, av::FieldContainer* container) const
{
  ::shade::types::ListAccessor* list_accessor(dynamic_cast< ::shade::types::ListAccessor*>(type));
  if (list_accessor == 0)
    return;

  av::MFContainer* field= new av::MFContainer;
  field->bind(container, name, true, ListGetter(list_accessor), ListSetter(list_accessor));
}
