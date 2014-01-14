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

#ifndef AVANGO_SHADE_LIGHTSOURCE_H
#define AVANGO_SHADE_LIGHTSOURCE_H

/**
 * \file
 * \ingroup av_shade
 */

#include <osg/LightSource>

#include <avango/osg/Node.h>
#include <avango/osg/Fields.h>
#include <avango/StandardFields.h>

#include <avango/shade/windows_specific_shade.h>

namespace av
{
  namespace shade
  {
    class Shader;
    typedef ::osg::LightSource OsgLightSource;

    /**
     * Wrapper for ::osg::LightSource and ::osg::Light
     *
     * \ingroup av_shade
     */
    class AV_SHADE_DLL LightSource : public av::osg::Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::LightSource is created.
       * Otherwise, the given ::osg::LightSource is used.
       */
      LightSource(OsgLightSource* osglightsource = new ::osg::LightSource());
      // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~LightSource();

    public:

      /**
       * Light source number
       */
      SFInt LightNum;

      /**
       * Light source position or direction
       */
      av::osg::SFVec4 Position;

      /**
       * Light source position or direction gettable from a shader
       */
      av::SFContainer Shader;

      /**
       * Get the wrapped ::osg::LightSource.
       */
      ::osg::LightSource* getOsgLightSource() const;

    protected:

      virtual void getLightNumCB(const av::SFInt::GetValueEvent& event);
      virtual void setLightNumCB(const av::SFInt::SetValueEvent& event);

      virtual void getPositionCB(const av::osg::SFVec4::GetValueEvent& event);
      virtual void setPositionCB(const av::osg::SFVec4::SetValueEvent& event);

    private:

      int getLightNum(void);
      void setLightNum(int num);

      ::osg::LightSource *mOsgLightSource;
      av::Link<av::shade::Shader> mGLLightPosition;
    };

    typedef SingleField<Link<LightSource> > SFLightSource;
    typedef MultiField<Link<LightSource> > MFLightSource;
  } // namespace shade

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<shade::LightSource> >;
  template class AV_OSG_DLL MultiField<Link<shade::LightSource> >;
#endif

} // namespace av

#endif
