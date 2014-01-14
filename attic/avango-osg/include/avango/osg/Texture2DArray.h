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

#if !defined(AVANGO_OSG_TEXTURE2DARRAY_H)
#define AVANGO_OSG_TEXTURE2DARRAY_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/osg/Texture.h>
#include <osg/Texture2DArray>
#include <avango/osg/Image.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Texture2DArray OsgTexture2DArray;

    /**
     * Abstract Wrapper for ::osg::Texture
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Texture2DArray : public Texture
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      Texture2DArray(OsgTexture2DArray* osgtexture = new OsgTexture2DArray());
      // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Texture2DArray();

    public:

      MFImage Image;

      SFInt TextureWidth;
      SFInt TextureHeight;

      /**
       * Get the wrapped ::osg::Texture2DArray object.
       * \return a pointer to the texture object
       */
      ::osg::Texture2DArray* getOsgTexture2DArray() const;

    private:

      ::osg::Texture2DArray* mOsgTexture2DArray;

    protected:

      virtual void getImageCB(const av::osg::MFImage::GetValueEvent& event);
      virtual void setImageCB(const av::osg::MFImage::SetValueEvent& event);
      virtual void getTextureWidthCB(const av::SFInt::GetValueEvent& event);
      virtual void setTextureWidthCB(const av::SFInt::SetValueEvent& event);
      virtual void getTextureHeightCB(const av::SFInt::GetValueEvent& event);
      virtual void setTextureHeightCB(const av::SFInt::SetValueEvent& event);

    };

    typedef SingleField<Link<Texture2DArray> > SFTexture2DArray;
    typedef MultiField<Link<Texture2DArray> > MFTexture2DArray;
  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Texture2DArray> >;
  template class AV_OSG_DLL MultiField<Link<osg::Texture2DArray> >;
#endif

} // namespace av

#endif
