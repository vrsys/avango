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

#if !defined(AVANGO_OSG_TEXTURE_CUBE_MAP_H)
#define AVANGO_OSG_TEXTURE_CUBE_MAP_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/osg/Texture.h>
#include <osg/TextureCubeMap>
#include <avango/osg/Image.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::TextureCubeMap OsgTextureCubeMap;

    /**
     * Abstract Wrapper for ::osg::TextureCubeMap
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL TextureCubeMap : public Texture
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      TextureCubeMap(OsgTextureCubeMap* osgtexture = new OsgTextureCubeMap());
      // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~TextureCubeMap();

    public:

      MFImage Images;

      SFInt TextureWidth;
      SFInt TextureHeight;

      /**
       * Get the wrapped ::osg::TextureCubeMap object.
       * \return a pointer to the texture object
       */
      ::osg::TextureCubeMap* getOsgTextureCubeMap() const;

    private:

      ::osg::TextureCubeMap* mOsgTextureCubeMap;

    protected:

      virtual void getImagesCB(const av::osg::MFImage::GetValueEvent& event);
      virtual void setImagesCB(const av::osg::MFImage::SetValueEvent& event);
      virtual void getTextureWidthCB(const av::SFInt::GetValueEvent& event);
      virtual void setTextureWidthCB(const av::SFInt::SetValueEvent& event);
      virtual void getTextureHeightCB(const av::SFInt::GetValueEvent& event);
      virtual void setTextureHeightCB(const av::SFInt::SetValueEvent& event);

    };

    typedef SingleField<Link<TextureCubeMap> > SFTextureCubeMap;
    typedef MultiField<Link<TextureCubeMap> > MFTextureCubeMap;
  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::TextureCubeMap> >;
  template class AV_OSG_DLL MultiField<Link<osg::TextureCubeMap> >;
#endif

} // namespace av

#endif
