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

#if !defined(AVANGO_OSG_IMAGE_H)
#define AVANGO_OSG_IMAGE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Image>
#include <avango/osg/Object.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Image OsgImage;

    /**
     * Wrapper for ::osg::Image
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Image : public Object
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      Image(OsgImage* osgimage = new OsgImage);
      // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Image();

    public:

      /**
       * Width directly sets and gets the width of the wrapped ::osg::Image.
       */
      SFUInt Width;

      /**
       * Height directly sets and gets the height of the wrapped ::osg::Image.
       */
      SFUInt Height;

      /**
       * Internal (i.e. GPU) pixel format if used as a texture
       */
      SFInt InternalTextureFormat;

      /**
       * Get the wrapped ::osg::Image.
       */
      ::osg::Image* getOsgImage() const;

    protected:

      virtual void getWidthCB(const SFUInt::GetValueEvent& event);
      virtual void setWidthCB(const SFUInt::SetValueEvent& event);
      virtual void getHeightCB(const SFUInt::GetValueEvent& event);
      virtual void setHeightCB(const SFUInt::SetValueEvent& event);
      virtual void getInternalTextureFormatCB(const SFInt::GetValueEvent& event);
      virtual void setInternalTextureFormatCB(const SFInt::SetValueEvent& event);

    private:

      ::osg::Image *mOsgImage;
    };

    typedef SingleField<Link<Image> > SFImage;
    typedef MultiField<Link<Image> > MFImage;

  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Image> >;
  template class AV_OSG_DLL MultiField<Link<osg::Image> >;
#endif

} // namespace av

#endif
