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

#if !defined(AVANGO_OSG_TEXTURE2D_H)
#define AVANGO_OSG_TEXTURE2D_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/osg/Texture.h>
#include <osg/Texture2D>
#include <avango/osg/Image.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Texture2D OsgTexture2D;

    /**
     * Abstract Wrapper for ::osg::Texture
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Texture2D : public Texture
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      Texture2D(OsgTexture2D* osgtexture = new OsgTexture2D());
      // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Texture2D();

    public:

      SFImage Image;

      SFInt TextureWidth;
      SFInt TextureHeight;

      /**
       * Get the wrapped ::osg::Texture2D object.
       * \return a pointer to the texture object
       */
      ::osg::Texture2D* getOsgTexture2D() const;

    private:

      ::osg::Texture2D* mOsgTexture2D;

    protected:

      virtual void getImageCB(const av::osg::SFImage::GetValueEvent& event);
      virtual void setImageCB(const av::osg::SFImage::SetValueEvent& event);
      virtual void getTextureWidthCB(const av::SFInt::GetValueEvent& event);
      virtual void setTextureWidthCB(const av::SFInt::SetValueEvent& event);
      virtual void getTextureHeightCB(const av::SFInt::GetValueEvent& event);
      virtual void setTextureHeightCB(const av::SFInt::SetValueEvent& event);

    };

    typedef SingleField<Link<Texture2D> > SFTexture2D;
    typedef MultiField<Link<Texture2D> > MFTexture2D;
  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Texture2D> >;
  template class AV_OSG_DLL MultiField<Link<osg::Texture2D> >;
#endif

} // namespace av

#endif
