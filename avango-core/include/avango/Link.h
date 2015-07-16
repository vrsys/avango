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

#if !defined(AV_LINK_H)
#define AV_LINK_H

/**
 * \file
 * \ingroup av
 */

// includes, system

#include <cstddef>

// includes, project

#include <avango/Assert.h>
#include <avango/Base.h>
#include <avango/logging/Logger.h>


namespace av
{

  class InputStream;
  class OutputStream;


  // types, exported (class, enum, struct, union, typedef)

  /**
   * Abstract base class for Link<> template class. All specializations of
   * Link<> can be accessed through this interface.
   *
   * \ingroup av
   */
  class AV_DLL AnyLink
  {

  protected:

    static av::logging::Logger& logger;

  public:

    /**
     * Hash Functor for using AnyLink in hash_map.
     */
    class Hasher
    {
    public:
      size_t operator()(const AnyLink& link) const
      {
        using cast_destination_t = size_t;
        return reinterpret_cast<cast_destination_t>(link.getBasePtr())>>3;
      }
    };

    /**
     * Returns \c false if this is the \c null link.
     */
    bool isValid() const;

    /**
     * Makes this this the \c null link.
     */
    void clear();

    /**
     * Returns the pointer contained in this link as a Base pointer.
     * If the link is not valid, \c null is returned.
     */
    Base* getBasePtr() const;

    /**
     * Sets the pointer contained in this link. The argument is type
     * checked against the concrete instance of Link<> this method is
     * called on. If the type check succeeds, \c true is returned and the
     * pointer is set, otherwise \c false is returned and no action is
     * taken.
     */
    bool setBasePtr(Base*);

  protected:

    AnyLink(Base* base, Type typeId);
    virtual ~AnyLink();

    Base* mBasePtr;
    Type mTypeId;

  };

  /**
   * Compare two AnyLink for equality by comparing the contained
   * pointer values.
   *
   * \ingroup av
   */
  AV_DLL bool operator==(const AnyLink&, const AnyLink&);

  /**
   * Compare two AnyLink for inequality by comparing the contained
   * pointer values.
   *
   * \ingroup av
   */
  AV_DLL bool operator!=(const AnyLink&, const AnyLink&);

  /**
   * Compare two AnyLink by cardinality by comparing the contained pointer
   * values. This is most useful in/for sorted STL containers.
   *
   * \ingroup av
   */
  AV_DLL bool operator<(const AnyLink&, const AnyLink&);

  /**
   * A typed smart pointer class. The Link class template makes the use
   * of smart pointers typesafe with respect to the C++ compiler. T
   * must be a class which is derived from Base.
   *
   * \ingroup av
   */
  template <typename T>
  class Link : public AnyLink
  {

  private:

    /**
     * Be sure that T provides reference().
     * \see http://www.gotw.ca/gotw/071.htm
     */
    bool validateRequirements() const
    {
      void(T::*test)() = &T::reference;

      if (test)
        ;

      return true;
    }

  public:

    /**
     * Constructs a new, empty typed link.
     */
    Link() :
      AnyLink(0, T::getClassTypeId())
    {}

    /**
     * Copy construct a new link.
     */
    Link(const Link<T>& link)
      : AnyLink(link.getBasePtr(), T::getClassTypeId())
    {}

    /**
     * Copy construct a new link from a compatible type.
     */
    template <class Y>
    Link(const Link<Y>& link)
      : AnyLink(convertToBase(link.getPtr()), T::getClassTypeId())
    {}


    /**
     * Constructs a new typed link to the object pointed to by \c ptr.
     */
    Link(T* ptr)
      : AnyLink(ptr, T::getClassTypeId())
    {}


    /**
     * Assign a link. This will increment the reference count of the
     * referenced object.
     */
    const Link<T>& operator=(const Link<T>& link)
    {
      if (this != &link) {
        setBasePtr(link.getPtr());
      }

      return *this;
    }


    /**
     * Dereferences a link like the standard '->' operator does on a
     * normal pointer. If the link is empty or not valid an exeption
     * would be thrown, if the dumb SGI C++ compiler would
     * support exeption handling. For now an assertion is raised, i.e.
     * the program is terminated.
     *
     * \todo Throw exception on empty link dereference.
     *
     */
    T* operator->() const
    {
      AV_ASSERT(mBasePtr);
      return getPtr();
    }

    /**
     * Returns a reference to the linked object. If the link is empty or
     * not valid an assertion is raised, ie. the program is terminated.
     *
     * \todo Throw exception on empty link dereference.
     *
     */
    T& operator*() const
    {
      AV_ASSERT(mBasePtr);
      return *getPtr();
    }

    /**
     * Returns a pointer to the linked object. If the link is not valid,
     * \c null is returned.
     */
    T* getPtr() const
    {
      return static_cast<T*>(mBasePtr);
    }

    /**
     * Get a reference to the empty link.
     */
    static const Link<T>& empty()
    {
      return sEmptyLink;
    }

  private:
    Base* convertToBase(T* ptr) const { return ptr; }
    static const Link<T>& sEmptyLink;
  };

  // variables, exported (extern)

  // functions, inlined (inline)

  // functions, exported (extern)

  template <typename T>
  const Link<T>& Link<T>::sEmptyLink = Link<T>();

  /**
   * Returns \c true if the lhs link points to the same object as the
   * rhs link.
   *
   * \ingroup av
   */
  template <typename T> bool
  operator==(const Link<T>& lhs, const Link<T>& rhs)
  {
    return lhs.getPtr() == rhs.getPtr();
  }

  /**
   * Returns \c true if the lhs link does not point to the same object as
   * the rhs link.
   *
   * \ingroup av
   */
  template <typename T> bool
  operator!=(const Link<T>& a, const Link<T>& b)
  {
    return !(a == b);
  }

  /**
   * Returns \c true if the lhs link is 'less' than the rhs link.
   * In fact, the absolute memory adresses of the linked objects are
   * compared.
   *
   * \ingroup av
   */
  template <typename T> bool
  operator<(const Link<T>& lhs, const Link<T>& rhs)
  {
    return lhs.getPtr() < rhs.getPtr();
  }

  /**
   * Specialization for boost::mem_fn support.
   * Together with boost::bind (which implicitly uses boost::mem_fn) you
   * can iterate over a container of Links, e.g:
   *
   * \code
   *  void f(std::list<Link<Object> >& objects) {
   *    for_each(objects.begin(), objects.end(),
   *             boost::bind(&Object::touch, _1));
   *  }
   * \endcode
   *
   * \ingroup av
   */
  template<typename T> T* get_pointer(const Link<T>& link)
  {
    return link.getPtr();
  }

  AV_DLL InputStream& operator>>(InputStream& is, AnyLink& value);
  AV_DLL OutputStream& operator<<(OutputStream& os, const AnyLink& value);

}

#endif // #if !defined(AV_LINK_H)
