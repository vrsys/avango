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

#if !defined(AVANGO_OSG_IMAGESTREAM_H)
#define AVANGO_OSG_IMAGESTREAM_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/ImageStream>
#include <avango/osg/Image.h>

namespace av
{
namespace osg
{
typedef ::osg::ImageStream OsgImageStream;

/**
 * Abstract Wrapper for ::osg::ImageStream
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL ImageStream : public Image
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor.
     */
    ImageStream(OsgImageStream* osgimage = new OsgImageStream);
    // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~ImageStream();

  public:
    /**
     * Should the video be looped
     */
    SFBool Loop;

    /**
     * Status might be INVALID, PLAYING, PAUSED, or REWINDING
     */
    SFInt Status;

    /**
     * Get the wrapped ::osg::ImageStream object.
     * \return a pointer to the imagestream object
     */
    ::osg::ImageStream* getOsgImageStream() const;

  private:
    ::osg::ImageStream* mOsgImageStream;

  protected:
    virtual void getLoopCB(const av::SFBool::GetValueEvent& event);
    virtual void setLoopCB(const av::SFBool::SetValueEvent& event);
    virtual void getStatusCB(const av::SFInt::GetValueEvent& event);
    virtual void setStatusCB(const av::SFInt::SetValueEvent& event);
};

typedef SingleField<Link<ImageStream>> SFImageStream;
typedef MultiField<Link<ImageStream>> MFImageStream;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::ImageStream>>;
template class AV_OSG_DLL MultiField<Link<osg::ImageStream>>;
#endif

} // namespace av

#endif
