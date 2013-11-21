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

#if !defined(AV_TYPED_H)
#define AV_TYPED_H

/**
 * \file
 * \ingroup av
 */

// includes, system

// includes, project

#include <avango/Assert.h>
#include <avango/Type.h>
#include <avango/windows_specific.h>

namespace av
{

  // types, exported (class, enum, struct, union, typedef)

  /**
   * Abstract base class for all classes who wish to run under the
   * runtime type system. The Typed class is an abstract base class.
   * It provides runtime typing for derived classes.
   *
   * The Typed class provides a complex macro package which must
   * be used by derived classes in order for runtime typing to work.
   *
   * \see AV_TYPED_DECLARE()
   *
   * \todo Use ::abort, ::exit, libfp::clean_exit or exceptions?
   * \bug AV_TYPED_DECLAREd functions are undocumented
   *
   * \ingroup av
   */
  class AV_DLL Typed
  {

  public:

    /**
     * Constructor
     */
    Typed();

    /**
     * Destructor
     */
    virtual ~Typed();

    /**
     * Type initialization. Must be called before instances of a typed
     * class can be created.
     */
    static void initClass();

    /**
     * Returns the type identifier for this class.
     */
    static Type getClassTypeId();

    /**
     * Returns the type identifier for a specific instance.
     * This method must be overloaded by derived classes, which should
     * be done by calling AV_TYPED_DEFINE().
     */
    virtual Type getTypeId() const = 0;

  private:

    static Type sClassTypeId;

  };

  // variables, exported (extern)

  // functions, inlined (inline)

  // functions, exported (extern)

}

// subclassing macros

/**
 * This macro must be called in the definition of all concrete classes
 * derived from av::Typed.
 *
 * \ingroup av
 */
#define AV_TYPED_DECLARE()                      \
  public:                                       \
  static void initClass();                      \
  static ::av::Type getClassTypeId();             \
  static bool isTypeInitialized();              \
  ::av::Type    getTypeId() const;                \
private:                                        \
static ::av::Type sClassTypeId

/**
 * This macro must be called in the definition of all abstract classes
 * derived from av::Typed.
 *
 * \ingroup av
 */
#define AV_TYPED_DECLARE_ABSTRACT()             \
  public:                                       \
  static void      initClass();                 \
  static ::av::Type  getClassTypeId();            \
  static bool      isTypeInitialized();         \
private:                                        \
static ::av::Type sClassTypeId

/**
 * This macro must be called at toplevel for all abstract classes
 * derived from av::Typed.
 *
 * \pre AV_TYPED_DECLARE_ABSTRACT() must have been called prior to this.
 *
 * \ingroup av
 */
#define AV_TYPED_DEFINE_ABSTRACT(thisClass)                     \
  ::av::Type                                                      \
  thisClass::getClassTypeId()                                   \
  {                                                             \
    AV_ASSERT(::av::Type::badType() != sClassTypeId);             \
    return sClassTypeId;                                        \
  }                                                             \
  bool                                                          \
  thisClass::isTypeInitialized()                                \
  {                                                             \
    return ::av::Type::badType() != sClassTypeId;                 \
  }                                                             \
  ::av::Type thisClass::sClassTypeId = ::av::Type::badType();

/**
 * This macro must be called at toplevel for all concrete classes
 * derived from av::Typed.
 *
 * \pre AV_TYPED_DECLARE() must have been called prior to this.
 *
 * \ingroup av
 */
#define AV_TYPED_DEFINE(thisClass)                      \
  ::av::Type                                              \
  thisClass::getTypeId() const                          \
  {                                                     \
    AV_ASSERT(::av::Type::badType() != sClassTypeId);     \
    return sClassTypeId;                                \
  }                                                     \
  AV_TYPED_DEFINE_ABSTRACT(thisClass)

/**
 * This macro must be called in DerivedClass::initClass() for all
 * concrete classes derived from av::Typed.
 *
 * \pre AV_TYPED_DECLARE() must have been called prior to this.
 * \pre All parent classes must have been initialized. You can ensure this
 *      by calling ParentClass::initClass().
 *
 * \ingroup av
 */
#define AV_TYPED_INIT(parentClass, thisClass, create, isPublic) \
  do {                                                          \
    if (::av::Type::badType() == sClassTypeId) {                  \
      sClassTypeId =                                            \
        ::av::Type::createType(::av::Type::getByName(#parentClass), \
                               #thisClass,                        \
                               create,                            \
                               isPublic);                         \
      AV_ASSERT(::av::Type::badType() != sClassTypeId);           \
    }                                                           \
  } while (0)

/**
 * This macro must be called in DerivedClass::initClass() for all
 * abstract classes derived from av::Typed.
 *
 * \pre AV_TYPED_DECLARE_ABSTRACT() must have been called prior to this.
 * \pre All parent classes must have been initialized. You can ensure this
 *      by calling ParentClass::initClass().
 *
 * \ingroup av
 */
#define AV_TYPED_INIT_ABSTRACT(parentClass, thisClass, isPublic)        \
  do {                                                                  \
    if (::av::Type::badType() == sClassTypeId) {                          \
      sClassTypeId =                                                    \
        ::av::Type::createAbstractType(::av::Type::getByName(#parentClass), \
                                       #thisClass,                        \
                                       isPublic);                         \
      AV_ASSERT(::av::Type::badType() != sClassTypeId);                   \
    }                                                                   \
  } while (0)

#endif // #if !defined(AV_TYPED_H)
