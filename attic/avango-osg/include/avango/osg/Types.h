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

#if !defined(AVANGO_OSG_TYPES_H)
#define AVANGO_OSG_TYPES_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Matrix>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>

#include <avango/osg/windows_specific_osg.h>
#include <avango/Config.h>
#include <avango/InputStream.h>
#include <avango/OutputStream.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/Msg.h>
#endif //#if defined(AVANGO_DISTRIBUTION_SUPPORT)

namespace av
{
AV_OSG_DLL OutputStream& operator<<(OutputStream&, const ::osg::Matrixf&);
AV_OSG_DLL InputStream& operator>>(InputStream&, ::osg::Matrixf&);

AV_OSG_DLL OutputStream& operator<<(OutputStream&, const ::osg::Matrixd&);
AV_OSG_DLL InputStream& operator>>(InputStream&, ::osg::Matrixd&);

AV_OSG_DLL OutputStream& operator<<(OutputStream&, const ::osg::Vec2f&);
AV_OSG_DLL InputStream& operator>>(InputStream&, ::osg::Vec2f&);

AV_OSG_DLL OutputStream& operator<<(OutputStream&, const ::osg::Vec2d&);
AV_OSG_DLL InputStream& operator>>(InputStream&, ::osg::Vec2d&);

AV_OSG_DLL OutputStream& operator<<(OutputStream&, const ::osg::Vec3f&);
AV_OSG_DLL InputStream& operator>>(InputStream&, ::osg::Vec3f&);

AV_OSG_DLL OutputStream& operator<<(OutputStream&, const ::osg::Vec3d&);
AV_OSG_DLL InputStream& operator>>(InputStream&, ::osg::Vec3d&);

AV_OSG_DLL OutputStream& operator<<(OutputStream&, const ::osg::Vec4f&);
AV_OSG_DLL InputStream& operator>>(InputStream&, ::osg::Vec4f&);

AV_OSG_DLL OutputStream& operator<<(OutputStream&, const ::osg::Vec4d&);
AV_OSG_DLL InputStream& operator>>(InputStream&, ::osg::Vec4d&);

AV_OSG_DLL OutputStream& operator<<(OutputStream&, const ::osg::Quat&);
AV_OSG_DLL InputStream& operator>>(InputStream&, ::osg::Quat&);

} // namespace av

/// @cond DOXYGEN_SHOULD_SKIP_THIS
namespace osg
{
// these functions need to be put into the ::osg namespace,
// as they are really a part of the interface for the corresponding OSG types.
// ADL (Argument Dependant Loopup) takes care that these functions are found in the right namespace
// This makes these functions visible to SIngleField<T> and MultiField<T> at the point of template instantiation,
// not just definition ( see C++ ISO Standard 14882 from '98; Chapter 14.6.4 )
#if defined(AVANGO_DISTRIBUTION_SUPPORT)

AV_OSG_DLL void av_pushMsg(av::Msg& netMsg, const ::osg::Matrixf& buf);
AV_OSG_DLL void av_popMsg(av::Msg& netMsg, ::osg::Matrixf& buf);

AV_OSG_DLL void av_pushMsg(av::Msg& netMsg, const ::osg::Matrixd& buf);
AV_OSG_DLL void av_popMsg(av::Msg& netMsg, ::osg::Matrixd& buf);

AV_OSG_DLL void av_pushMsg(av::Msg& netMsg, const ::osg::Vec2f& buf);
AV_OSG_DLL void av_popMsg(av::Msg& netMsg, ::osg::Vec2f& buf);

AV_OSG_DLL void av_pushMsg(av::Msg& netMsg, const ::osg::Vec2d& buf);
AV_OSG_DLL void av_popMsg(av::Msg& netMsg, ::osg::Vec2d& buf);

AV_OSG_DLL void av_pushMsg(av::Msg& netMsg, const ::osg::Vec3f& buf);
AV_OSG_DLL void av_popMsg(av::Msg& netMsg, ::osg::Vec3f& buf);

AV_OSG_DLL void av_pushMsg(av::Msg& netMsg, const ::osg::Vec3d& buf);
AV_OSG_DLL void av_popMsg(av::Msg& netMsg, ::osg::Vec3d& buf);

AV_OSG_DLL void av_pushMsg(av::Msg& netMsg, const ::osg::Vec4f& buf);
AV_OSG_DLL void av_popMsg(av::Msg& netMsg, ::osg::Vec4f& buf);

AV_OSG_DLL void av_pushMsg(av::Msg& netMsg, const ::osg::Vec4d& buf);
AV_OSG_DLL void av_popMsg(av::Msg& netMsg, ::osg::Vec4d& buf);

AV_OSG_DLL void av_pushMsg(av::Msg& netMsg, const ::osg::Quat& buf);
AV_OSG_DLL void av_popMsg(av::Msg& netMsg, ::osg::Quat& buf);
#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)
} // namespace osg
/// @endcond

#endif
