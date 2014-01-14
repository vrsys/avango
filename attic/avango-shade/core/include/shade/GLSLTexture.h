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

#ifndef shade_GLSLTexture_H
#define shade_GLSLTexture_H shade_GLSLTexture_H

#include "Texture.h"
#include <GL/glew.h>

namespace shade
{
  class GLSLTexture : public Texture
  {
  public:
    GLSLTexture(GLenum target);
    GLSLTexture(GLenum enable_target, GLenum bind_target);
    ~GLSLTexture(void);

    void bind(void) const;
    void enable(void) const;

    GLuint get_name(void) { return m_name; }

  private:
    GLenum m_enable_target;
    GLenum m_bind_target;
    GLuint m_name;
  };
}

#endif /* shade_GLSLTexture_H */
