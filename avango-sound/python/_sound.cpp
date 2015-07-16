// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include <avango/sound/SoundSource.h>
#include <avango/sound/SoundRenderer.h>
#include <avango/sound/SoundTraverser.h>
#include <avango/sound/SampleBuffer.h>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

using namespace boost::python;
using namespace av::python;


namespace boost
{
  namespace python
  {
    template <class T> struct pointee<av::Link<T> >
    {
      using type = T;
    };
  }
}


BOOST_PYTHON_MODULE(_sound)
{
  av::sound::SoundTraverser::initClass();
  av::sound::SoundRenderer::initClass();
  av::sound::SoundSource::initClass();
  av::sound::SampleBuffer::initClass();

  class_<av::sound::SoundTraverser, av::Link<av::sound::SoundTraverser>, bases<av::FieldContainer>, boost::noncopyable >("SoundTraverser", "docstring");
  class_<av::sound::SoundRenderer, av::Link<av::sound::SoundRenderer>, bases<av::FieldContainer>, boost::noncopyable >("SoundRenderer", "docstring", no_init);
  class_<av::sound::SampleBuffer, av::Link<av::sound::SampleBuffer>, bases<av::FieldContainer>, boost::noncopyable >("SampleBuffer", "docstring", no_init);
  //class_<av::sound::SoundSource, av::Link<av::sound::SoundSource>, bases<av::osg::Group>, boost::noncopyable >("SoundSource", "docstring");
  class_<av::sound::SoundSource, av::Link<av::sound::SoundSource>, bases<av::gua::TransformNode>, boost::noncopyable >("SoundSource", "docstring");

  register_field<av::sound::SFSoundRenderer>("SFSoundRenderer");
  register_multifield<av::sound::MFSoundRenderer>("MFSoundRenderer");

  register_field<av::sound::SFSampleBuffer>("SFSampleBuffer");
  register_multifield<av::sound::MFSampleBuffer>("MFSampleBuffer");
}
