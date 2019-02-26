// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#if !defined(AVANGO_SHADE_TYPES_H)
#define AVANGO_SHADE_TYPES_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Object>

#include <avango/shade/windows_specific_shade.h>
#include <avango/Config.h>
#include <avango/InputStream.h>
#include <avango/OutputStream.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/Msg.h>
#endif //#if defined(AVANGO_DISTRIBUTION_SUPPORT)

namespace av
{
AV_SHADE_DLL OutputStream& operator<<(OutputStream&, const ::osg::Object::DataVariance&);
AV_SHADE_DLL InputStream& operator>>(InputStream&, ::osg::Object::DataVariance&);
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

AV_OSG_DLL void av_pushMsg(av::Msg& netMsg, const ::osg::Object::DataVariance& buf);
AV_OSG_DLL void av_popMsg(av::Msg& netMsg, ::osg::Object::DataVariance& buf);
#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)
} // namespace osg
/// @endcond

#endif
