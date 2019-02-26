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

#if !defined(AV_WINDOWS_SPECIFIC_UTILS_H)
#define AV_WINDOWS_SPECIFIC_UTILS_H

/**
 * \file
 * \ingroup av_utils
 */

#if defined(_MSC_VER)
#if defined(AV_UTILS_LIBRARY)
#define AV_UTILS_DLL __declspec(dllexport)
#else
#define AV_UTILS_DLL __declspec(dllimport)
#endif
#else
#define AV_UTILS_DLL
#endif // #if defined(_MSC_VER)

#endif // #if !defined(AV_WINDOWS_SPECIFIC_UTILS_H)
