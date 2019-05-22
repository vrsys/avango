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

#if !defined(AVANGO_OSG_CAMERA_ATTACHMENT_H)
#define AVANGO_OSG_CAMERA_ATTACHMENT_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/FieldContainer.h>
#include <avango/StandardFields.h>
#include <avango/osg/Texture.h>
#include <avango/osg/Image.h>

namespace av
{
namespace osg
{
/**
 * Data structure for camera attachments, e.g textures
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL CameraAttachment : public FieldContainer
{
    AV_FC_DECLARE();

  public:
    CameraAttachment(void);

    /**
     * Buffer component to map attachment to
     */
    SFInt BufferComponent;

    SFInt InternalFormat;
    SFTexture Texture;
    SFImage Image;
    SFUInt Level;
    SFUInt Face;
    SFBool MIPMapGeneration;
    SFUInt MultisampleSamples;
    SFUInt MultisampleColorSamples;

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~CameraAttachment();
};

typedef SingleField<Link<CameraAttachment>> SFCameraAttachment;
typedef MultiField<Link<CameraAttachment>> MFCameraAttachment;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::CameraAttachment>>;
template class AV_OSG_DLL MultiField<Link<osg::CameraAttachment>>;
#endif

} // namespace av

#endif
