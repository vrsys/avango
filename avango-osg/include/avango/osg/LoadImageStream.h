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

#if !defined(AVANGO_OSG_LOADIMAGESTREAM_H)
#define AVANGO_OSG_LOADIMAGESTREAM_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/Object.h>
#include <avango/StandardFields.h>
#include <avango/osg/ImageStream.h>
#include "windows_specific_osg.h"


namespace av
{
  namespace osg
  {
    /**
     * Convenience node supposed to load an image,
     * using the osgDB::readImageStreamFile() mechanism
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL LoadImageStream : public ::av::Object
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. Note that there is no counterpart in osg for an av::osg::LoadImageStream.
       */
      LoadImageStream();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~LoadImageStream();

    public:

      /**
       * File name of the image file to load.
       */
      SFString Filename;

      /**
       * Loaded image
       */
      SFImageStream ImageStream;

      /* virtual */ void evaluateLocalSideEffect();

    };

    typedef SingleField<Link<LoadImageStream> > SFLoadImageStream;
    typedef MultiField<Link<LoadImageStream> > MFLoadImageStream;

  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::LoadImageStream> >;
  template class AV_OSG_DLL MultiField<Link<osg::LoadImageStream> >;
#endif

} // namespace av

#endif
