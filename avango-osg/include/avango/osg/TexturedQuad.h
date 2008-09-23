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

#if !defined(AVANGO_OSG_TEXTUREDQUAD_H)
#define AVANGO_OSG_TEXTUREDQUAD_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/osg/Geometry.h>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <avango/osg/Fields.h>
#include <avango/StandardFields.h>

namespace av
{
  namespace osg
  {
    /**
     * Node that creates a textured Quad as Drawable of a Geode
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL TexturedQuad : public Geometry
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      TexturedQuad();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~TexturedQuad();

    public:

      SFFloat Width;
      SFFloat Height;
      SFVec4 Color;
      SFVec3 Position;
      SFString Filename;
      SFInt MinFilter;
      SFInt MagFilter;
      SFFloat MaxAnisotropy;

      /* virtual */ void fieldHasChangedLocalSideEffect(const av::Field& field);
      /* virtual */ void evaluateLocalSideEffect();


      /**
       * Get the ::osg::Texture object.
       * \return an osg::ref pointer to the texture object
       */
      ::osg::ref_ptr< ::osg::Texture2D> getOsgTexture() const;

    private:

      bool mGeometryChanged;
      bool mColorChanged;
      bool mFilenameChanged;
      ::osg::ref_ptr< ::osg::Vec3Array> mVertexArray;
      ::osg::ref_ptr< ::osg::Vec3Array> mNormals;
      ::osg::ref_ptr< ::osg::Vec4Array> mColors;
      ::osg::ref_ptr< ::osg::Vec2Array> mTexCoords;
      ::osg::ref_ptr< ::osg::Texture2D> mOsgTexture;

      void updateGeometry();
      void updateTexture();
      void updateColor();

    protected:

      virtual void getMinFilterCB(const av::SFInt::GetValueEvent& event);
      virtual void setMinFilterCB(const av::SFInt::SetValueEvent& event);
      virtual void getMagFilterCB(const av::SFInt::GetValueEvent& event);
      virtual void setMagFilterCB(const av::SFInt::SetValueEvent& event);
      virtual void getMaxAnisotropyCB(const av::SFFloat::GetValueEvent& event);
      virtual void setMaxAnisotropyCB(const av::SFFloat::SetValueEvent& event);

    };

    typedef SingleField<Link<TexturedQuad> > SFTexturedQuad;
    typedef MultiField<Link<TexturedQuad> > MFTexturedQuad;
  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::TexturedQuad> >;
  template class AV_OSG_DLL MultiField<Link<osg::TexturedQuad> >;
#endif

} // namespace av

#endif
