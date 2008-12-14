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

#if !defined(AVANGO_OSG_LOADIMAGE_H)
#define AVANGO_OSG_LOADIMAGE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/Object.h>
#include <avango/StandardFields.h>
#include <avango/osg/Image.h>
#include "windows_specific_osg.h"


namespace av
{
  namespace osg
  {
    /**
     * Convenience node supposed to load an image,
     * using the osgDB::readImageFile() mechanism
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL LoadImage : public ::av::Object
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. Note that there is no counterpart in osg for an av::osg::LoadImage.
       */
      LoadImage();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~LoadImage();

    public:

      /**
       * File name of the image file to load.
       */
      SFString Filename;

      /**
       * Loaded image
       */
      SFImage Image;

      /* virtual */ void evaluateLocalSideEffect();

    };

    typedef SingleField<Link<LoadImage> > SFLoadImage;
    typedef MultiField<Link<LoadImage> > MFLoadImage;

  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::LoadImage> >;
  template class AV_OSG_DLL MultiField<Link<osg::LoadImage> >;
#endif

} // namespace av

#endif
