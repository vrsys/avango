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

#if !defined(AV_CREATE_H)
#define AV_CREATE_H

/**
 * \file
 * \ingroup av
 */

// includes, system

// includes, project

#include "windows_specific.h"

namespace av
{
class Typed;

// types, exported (class, enum, struct, union, typedef)

/**
 * An abstract interface for creating instances of Typed derived types.
 *
 * Create is a minimal factory-like interface for dynamically creating
 * instances of types. The types that can be created are restricted to
 * Typed derived types. Create itself cannot be used in any useful way,
 * it is rather intended to be used internally by Type to store a
 * generic pointer of a type's factory w/o actually knowing that type.
 *
 * \ingroup av
 */
class AV_DLL Create
{
  public:
    /**
     * Destructor
     */
    virtual ~Create();

    /**
     * A placeholder for the actual allocator function of a Typed derived
     * type.
     */
    virtual av::Typed* makeInstance() const = 0;
};

/**
 * CreateA<T> implements the actual mechanism for creating instances of
 * Typed derived types as declared in Create.
 *
 * CreateA<T> is a minimal factory-like interface for dynamically
 * creating instances of types. It is implemented as a template for
 * compiler automated code generation.
 *
 * \ingroup av
 */
template <typename T>
class CreateA : public Create
{
  public:
    /**
     * Implements the actual allocation of a Typed derived type.
     *
     * \return A pointer to the newly created object
     * \note   The return value may be 0
     */
    virtual av::Typed* makeInstance() const { return new T; }
};

// variables, exported (extern)

// functions, inlined (inline)

// functions, exported (extern)

} // namespace av

#endif // #if !defined(AV_CREATE_H)
