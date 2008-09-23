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

#if !defined(AV_BASE_H)
#define AV_BASE_H

/**
 * \file
 * \ingroup av
 */

// includes, system

// includes, project

#include <avango/Typed.h>
#include <avango/Type.h>
#include <avango/Create.h>

namespace av
{

  class InputStream;
  class OutputStream;
  class Action;
  class WriteAction;

  // types, exported (class, enum, struct, union, typedef)

  /**
   * This class provides reference counting, type-safe casting and
   * facilities for Action traversals and streaming.
   *
   * Base adds a public interface for streaming. An object can be
   * written to or read from a stream using the member functions read()
   * and write() or the provided global stream operators.
   *
   * Objects are automatically deleted when their reference count
   * goes to zero. This can be override by setting the
   * AVANGO_NO_DELETE_ON_UNREF environment variable.
   *
   * You cannot allocate instances of Base on the stack, but they can be
   * created with new. They should be stored in Link smart pointers,
   * so that they are automatically deleted when the reference count goes
   * to zero.
   *
   * \ingroup av
   *
   */
  class AV_DLL Base : public Typed
  {

    AV_TYPED_DECLARE_ABSTRACT();

  public:

    /**
     * Constructor
     */
    Base();

  protected:

    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Base();

  public:

    /**
     * Increment the reference count.
     */
    void reference();

    /**
     * Decrement the reference count. If the reference count becomes 0,
     * the object is destroyed. This can be overriden by setting the
     * AVANGO_NO_DELETE_ON_UNREF environment variable.
     */
    void unreference();

    /**
     * Decrement the reference count, but do not delete object if the
     * reference count becomes 0. Use with care.
     */
    void unreferenceWithoutDeletion();

    /**
     * Return the current reference count of the object.
     */
    int  referenceCount();

    /**
     * Try to find an inheritance path to the requested Type.
     *
     * Returns 0 if no such path exists. You can use the result to savely
     * cast to the requested Type if not 0.
     */
    Base* castTo(Type type);

    /**
     * A static version of castTo().
     */
    static Base* castTo(Type type, Base* objectToCast);

    /**
     * Overload this function to implement special behaviour on Action
     * traverals.
     *
     * Beware: If you overload this function and want to preserve the
     * behaviour of the parent class, you have to explicitly call
     * parentClass::doAction() from within.
     */
    virtual void doAction(Action& action);

    /**
     * Overload this function to implement special behaviour on
     * WriteAction traversals.
     *
     * Beware: If you overload this function and want to preserve
     * the behaviour of the parent class, you have to explicitly call
     * parentClass::doWriteAction() from within.
     */
    virtual void doWriteAction(WriteAction& writeAction);

    /**
     * Read the internal state of this object from the stream is.
     * Must be overloaded, this implementation does nothing.
     */
    virtual void read(InputStream& is);

    /**
     * Write the internal state of this object to the stream os.
     * Must be overloaded, this implementation does nothing.
     */
    virtual void write(OutputStream& os);

  protected:

    /**
     * Internally increment reference count.
     *
     * This virtual function implements part of the reference
     * counting mechanism.
     */
    virtual void refImpl();

    /**
     * Internally decrement reference count.
     *
     * This virtual function implements part of the reference
     * counting mechanism.
     */
    virtual void unrefImpl();

    /**
     * Internally decrement reference count.
     *
     * This virtual function implements part of the reference
     * counting mechanism.
     */
    virtual void unrefWithoutDeletionImpl();

    /**
     * Internally return reference count.
     *
     * This virtual function implements part of the reference
     * counting mechanism.
     */
    virtual int refCountImpl();

  private:

    int mRefCount;

    void writeToBinaryStream(WriteAction& action);
    void writeToASCIIStream(WriteAction& action);

  };

  // variables, exported (extern)

  // functions, inlined (inline)

  // functions, exported (extern)

  /**
   * Read the internal state of the object obj from the stream os.
   */
  extern AV_DLL InputStream& operator>>(InputStream& is, Base* obj);

  /**
   * Write the internal state of the object obj to the stream os.
   */
  extern AV_DLL OutputStream& operator<<(OutputStream& os, Base* obj);

}

// subclassing macros

/**
 * This macro must be called in the definition of all concrete classes
 * derived from av::Base.
 *
 * \ingroup av
 */
#define AV_BASE_DECLARE() \
  AV_TYPED_DECLARE()

/**
 * This macro must be called in the definition of all abstract classes
 * derived from av::Base.
 *
 * \ingroup av
 */
#define AV_BASE_DECLARE_ABSTRACT() \
  AV_TYPED_DECLARE_ABSTRACT()

/**
 * This macro must be called at toplevel for all abstract classes
 * derived from av::Base.
 *
 * \pre AV_BASE_DECLARE_ABSTRACT() must have been called prior to this.
 *
 * \ingroup av
 */
#define AV_BASE_DEFINE_ABSTRACT(thisClass) \
  AV_TYPED_DEFINE_ABSTRACT(thisClass)

/**
 * This macro must be called at toplevel for all concrete classes
 * derived from av::Base.
 *
 * \pre AV_BASE_DECLARE() must have been called prior to this.
 *
 * \ingroup av
 */
#define AV_BASE_DEFINE(thisClass) \
  AV_TYPED_DEFINE(thisClass)

/**
 * This macro must be called in DerivedClass::initClass() for all
 * abstract classes derived from av::Base.
 *
 * \pre AV_BASE_DECLARE_ABSTRACT() must have been called prior to this.
 * \pre All parent classes must have been initialized. You can ensure this
 *      by calling ParentClass::initClass().
 *
 * \ingroup av
 */
#define AV_BASE_INIT_ABSTRACT(parentClass, thisClass, isPublic) \
  AV_TYPED_INIT_ABSTRACT(parentClass, thisClass, isPublic)

/**
 * This macro must be called in DerivedClass::initClass() for all
 * concrete classes derived from av::Base.
 *
 * \pre AV_BASE_DECLARE() must have been called prior to this.
 * \pre All parent classes must have been initialized. You can ensure this
 *      by calling ParentClass::initClass().
 *
 * \ingroup av
 */
#define AV_BASE_INIT(parentClass, thisClass, isPublic) \
  AV_TYPED_INIT(parentClass, thisClass, \
                new ::av::CreateA<thisClass>, isPublic)

#endif // #if !defined(AV_BASE_H)
