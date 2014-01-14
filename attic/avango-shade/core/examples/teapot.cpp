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

#include "teapot.h"
#include "render-teapot.h"
#include <GL/glew.h>

void render_teapot(void)
{
  glBegin(GL_TRIANGLES);
  for (int i = 0; i != TEAPOT_NUM_INDICES; ++i)
  {
    int offset = 16*teapot_indices[i];

    glTexCoord2f(
        teapot_verts[offset+4],
        teapot_verts[offset+5]
        );

    glMultiTexCoord3f(
        GL_TEXTURE1,
        teapot_verts[offset+6],
        teapot_verts[offset+7],
        teapot_verts[offset+8]
        );

    glMultiTexCoord3f(
        GL_TEXTURE2,
        teapot_verts[offset+9],
        teapot_verts[offset+10],
        teapot_verts[offset+11]
        );

    glNormal3f(
        teapot_verts[offset+12],
        teapot_verts[offset+13],
        teapot_verts[offset+14]
        );

    glVertex3f(
        teapot_verts[offset+0],
        teapot_verts[offset+1],
        teapot_verts[offset+2]
        );
  }
  glEnd();
}
