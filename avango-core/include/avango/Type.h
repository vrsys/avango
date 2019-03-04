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

#if !defined(AV_TYPE_H)
#define AV_TYPE_H

/**
 * \file
 * \ingroup av
 */

#include "windows_specific.h"

#if defined(_MSC_VER)
#include "stdint_replacement.h"
#else
#include <inttypes.h>
#endif // _MSC_VER

#include <cstddef>
#include <string>
#include <vector>

#include <avango/InputStream.h>
#include <avango/OutputStream.h>

namespace av
{
class Typed;
class Create;

/**
 * Encapsulates the type information for runtime typing.
 * The Type class keeps track of runtime type information in Avango.
 * Each type is associated with a given name, so lookup is possible in
 * either direction.
 *
 * Many Avango classes request a unique Type when they are initialized.
 * This type can then be used to find out the actual class of an
 * instance when only its base class is known, or to obtain an instance
 * of a particular class given its type or name.
 *
 * \todo Check if mTypeSpec can be changed to be a normal c++ class
 *
 * \ingroup av
 */
class AV_DLL Type
{
  public:
    /**
     * Hash Functor for using Type in hash_map.
     */
    class Hasher
    {
      public:
        inline size_t operator()(const Type& t) const { return t.getId(); }
    };

    /**
     * List of Types type definition. This is used as return value of
     * some functions.
     */
    using TypeList = std::vector<Type>;

    /**
     * Constructor
     */
    Type();

    /**
     * Copy Constructor
     */
    Type(const Type& other);

    /**
     * Construct a Type from its 32 bit integer representation. This
     * is useful for file or network i/o involving instancies of Type}.
     */
    Type(uint32_t id);

    /**
     * Destructor
     */
    ~Type();

    /**
     * Assignment Operator
     */
    const Type& operator=(const Type& rhs);

    /**
     * Init run-time type system.
     * This effectively clears the type registry removing all registered
     * types.
     */
    static void init();

    /**
     * Returns an always-illegal type. Useful for returning errors.
     */
    static Type badType();

    /**
     * Returns the type associated with the given name.
     */
    static Type getByName(const std::string& typeName);

    /**
     * %Create and register new type.
     */
    static Type createType(Type parentType, const std::string& typeName, Create* creator, bool typeIsPublic);

    /**
     * %Create and register new abstract type.
     */
    static Type createAbstractType(Type parentType, const std::string& typeName, bool typeIsPublic);

    /**
     * Returns \c true if the type is a bad type.
     */
    bool isBad() const;

    /**
     * Returns \c true if the type is derived from type \c typeToCheck.
     */
    bool isDerivedFrom(Type typeToCheck) const;

    /**
     * Returns \c true if the type is derived from or is equal to type
     * \c typeToCheck.
     */
    bool isOfType(Type typeToCheck) const;

    /**
     * Returns \c true if the type is not an internal type.
     */
    bool isPublic() const;

    /**
     * Some types are able to create instances; for example, most nodes
     * (those which are not abstract classes) can be created this
     * way. This method returns \c true if the type supports such
     * creation.
     */
    bool canCreate() const;

    /**
     * Returns the name associated with a type.
     */
    const std::string& getName() const;

    /**
     * Returns a list of all types derived from the given type.
     */
    TypeList getAllDerivedFrom() const;

    /**
     * Returns the parent type of the given type.
     */
    Type getParent() const;

    /**
     * Returns \c true if the type is the same as \c rhs.
     */
    bool operator==(const Type rhs) const;

    /**
     * Returns \c true if the type is the same as \c rhs.
     */
    bool operator!=(const Type rhs) const;

    /**
     * Returns \c true if the id of the type is less than the id of
     * \c rhs.
     */
    bool operator<(const Type rhs) const;

    /**
     * Coerce a Type to <tt>unsigned int</tt>. This is useful for file or
     * network i/o involving instancies of Type
     *
     * \todo Check if uint32_t() can be removed
     */
    operator uint32_t() const;

    /**
     * Coerce a Type to <tt>unsigned int</tt>. This is useful for file or
     * network i/o involving instancies of Type
     */
    uint32_t getId() const;

    /**
     * Creates and returns a pointer to an instance of the type. Returns
     * null if an instance could not be created for some reason. The
     * pointer is returned as a generic pointer, but can be cast to the
     * appropriate type. For example
     * \code
     *   Group *g = static_cast<Group*> Group::getClassTypeId().createInstance();
     * \endcode
     * is a convoluted way of creating a new instance of a Group node.
     */
    Typed* createInstance();

    /**
     * Creates and returns a pointer to an instance of the type
     * associated with name. Returns null if an instance could not be
     * created for some reason. If \c typeName does not correspond with a
     * built-in type, it is looked for a shared object file which
     * implements the requested type.
     *
     * \see ModuleManager
     * \todo Library namespaces vs. shared object file names
     */
    static Typed* createInstanceOfType(const std::string& typeName, bool unloadable = true);

    /**
     * Allow instances of this type to be network distributed if \true.
     */
    void setDistributable(bool isDistributable);

    /**
     * Returns \c true if instances of this type are allowed to be
     * network distributed.
     */
    bool isDistributable() const;

    /**
     * Print out current type map via Avango Logging mechanism.
     */
    static void dumpTypeMap();

  private:
    /**
     * Return index of the type in type map
     */
    unsigned int getIndex() const;

    static bool isTypeRegistered(const std::string& typeName);

    static Type allocateNewType(bool isPublic, bool canCreate, bool isAdaptor);

    static void registerNewType(const std::string& typeName, Type parentType, Type newType, Create* creator);

    static void initRegistry();

    // make this class small, it will be a member of almost everything
    union {
        struct
        {
            uint32_t mIndex : 29; // index into the type table
            bool mIsPublic : 1;   // for quick checks without table lookup
            bool mCanCreate : 1;  // for quick checks without table lookup
            bool mIsAdaptor : 1;  // connection to Performer class hierarchy
        } mStorage;
        uint32_t mId;
    } mTypeSpec;
};

/**
 * An Alias for Type::TypeList
 */
using TypeList = Type::TypeList;

InputStream& operator>>(InputStream& is, Type& value);
OutputStream& operator<<(OutputStream& os, const Type& value);

} // namespace av

/**
 * Shortcut for object creation.
 * \ingroup av
 */
#define AV_NEW(typeName) dynamic_cast<typeName*>(av::Type::createInstanceOfType(#typeName))

#endif // #if !defined(AV_TYPE_H)
