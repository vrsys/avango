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
* but WITHOUT ANY WARRANTY without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#if !defined(AVANGO_CONFIG_H)
#define AVANGO_CONFIG_H

/**
 * \file
 * \ingroup av
 */

#if _AVANGO_DEBUG
#define AVANGO_DEBUG 1
#else
#undef AVANGO_DEBUG
#endif

#define AVANGO_LOG_LEVEL av::logging::WARN

#if _AVANGO_DISTRIBUTION_SUPPORT
#define AVANGO_DISTRIBUTION_SUPPORT 1
#else
#undef AVANGO_DISTRIBUTION_SUPPORT
#endif

#if _AVANGO_ZMQ_DISTRIBUTION_SUPPORT
#define ZMQ_DISTRIBUTION_SUPPORT 1
#else
#undef ZMQ_DISTRIBUTION_SUPPORT
#endif

#if _AVANGO_PCL_SUPPORT
#define PCL_SUPPORT 1
#else
#undef PCL_SUPPORT
#endif

#define AVANGO_VERSION_MAJOR _AVANGO_VERSION_MAJOR
#define AVANGO_VERSION_MINOR _AVANGO_VERSION_MINOR
#define AVANGO_VERSION_MAINT _AVANGO_VERSION_MAINT

#endif // #if !defined(AVANGO_CONFIG_H)

