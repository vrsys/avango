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

#include "ObjrefFieldAdapter.h"
#include "../include/avango/shade/Shader.h"
#include <avango/FieldContainer.h>
#include "../../core/include/shade/types/ObjrefAccessor.h"
#include "../../core/include/shade/types/objref.h"

namespace
{
class ObjrefGetter
{
  public:
    ObjrefGetter(shade::types::ObjrefAccessor* accessor) : mAccessor(accessor) {}

    void operator()(const av::SFContainer::GetValueEvent& event)
    {
        boost::shared_ptr<::shade::Shader> shader(mAccessor->get_generic());
        *event.getValuePtr() = av::shade::Shader::getWrapper(shader.get());
    }

  private:
    shade::types::ObjrefAccessor* mAccessor;
};

class ObjrefSetter
{
  public:
    ObjrefSetter(shade::types::ObjrefAccessor* accessor) : mAccessor(accessor) {}

    void operator()(const av::SFContainer::SetValueEvent& event)
    {
        av::shade::Shader* value(dynamic_cast<av::shade::Shader*>(event.getValue().getBasePtr()));
        if(!value)
            return; // FIXME throw exception
        mAccessor->set_generic(value->getShader());
    }

  private:
    shade::types::ObjrefAccessor* mAccessor;
};
} // namespace

::shade::types::TypeAccessor::HashType av::shade::ObjrefFieldAdapter::hash(void) const
{
    static ::shade::objref<boost::shared_ptr<::shade::Shader>> value;
    return value.hash();
}

void av::shade::ObjrefFieldAdapter::bindField(::shade::Type* type, const std::string& name, av::FieldContainer* container) const
{
    ::shade::types::ObjrefAccessor* objref_accessor(dynamic_cast<::shade::types::ObjrefAccessor*>(type));
    if(objref_accessor == 0)
        return;

    av::SFContainer* field = new av::SFContainer;
    field->bind(container, name, true, ObjrefGetter(objref_accessor), ObjrefSetter(objref_accessor));
}
