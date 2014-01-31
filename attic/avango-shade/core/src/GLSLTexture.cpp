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

#include <shade/GLSLTexture.h>
using namespace shade;


GLSLTexture::GLSLTexture(GLenum target) :
  m_enable_target(target),
  m_bind_target(target)
{
  glGenTextures(1, &m_name);
}

GLSLTexture::GLSLTexture(GLenum enable_target, GLenum bind_target) :
  m_enable_target(enable_target),
  m_bind_target(bind_target)
{
  glGenTextures(1, &m_name);
}

GLSLTexture::~GLSLTexture(void)
{
  glDeleteTextures(1, &m_name);
}

void GLSLTexture::bind(void) const
{
  glBindTexture(m_bind_target, m_name);
}

void GLSLTexture::enable(void) const
{
  glEnable(m_enable_target);
  bind();
}

