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

#include "TextureFieldAdapter.h"
#include "../include/avango/shade/Shader.h"
#include <avango/osg/Texture.h>
#include "../../core/include/shade/osg/Texture.h"
#include "../../core/include/shade/types/TextureAccessor.h"
#include "../../core/include/shade/types/sampler2D.h"

namespace
{
  class TextureGetter
  {
  public:

    TextureGetter(shade::types::TextureAccessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::osg::SFTexture::GetValueEvent& event)
    {
      boost::shared_ptr< ::shade::osg::Texture> shade_tex(boost::dynamic_pointer_cast< ::shade::osg::Texture>(mAccessor->get()));
      if (!shade_tex)
        return;
      ::osg::Texture* osg_tex = shade_tex->get();
      *event.getValuePtr() = av::osg::get_from_osg_object<av::osg::Texture>(osg_tex);
    }

  private:
    shade::types::TextureAccessor* mAccessor;
  };

  class TextureSetter
  {
  public:

    TextureSetter(shade::types::TextureAccessor* accessor) :
      mAccessor(accessor)
    {
    }

    void operator()(const av::osg::SFTexture::SetValueEvent& event)
    {
      mAccessor->set(boost::shared_ptr< ::shade::osg::Texture>(new ::shade::osg::Texture(event.getValue()->getOsgTexture())));
    }

  private:
    shade::types::TextureAccessor* mAccessor;
  };
}

::shade::types::TypeAccessor::HashType
av::shade::TextureFieldAdapter::hash(void) const
{
  static ::shade::sampler2D value;
  return value.hash();
}

void
av::shade::TextureFieldAdapter::bindField(::shade::Type* type, const std::string& name, av::FieldContainer* container) const
{
  ::shade::types::TextureAccessor* texture_accessor(dynamic_cast< ::shade::types::TextureAccessor*>(type));
  if (texture_accessor == 0)
    return;

  av::osg::SFTexture* field= new av::osg::SFTexture;
  field->bind(container, name, true, TextureGetter(texture_accessor), TextureSetter(texture_accessor));
}
