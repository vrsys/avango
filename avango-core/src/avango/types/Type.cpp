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

// include i/f header

#include "avango/Type.h"

// includes, system

#include <cstdlib>
#include <map>
#include <stdexcept>
#include <boost/format.hpp>

// includes, project

#include <avango/Create.h>
#include <avango/logging/Logger.h>

#include <avango/Config.h>

// internal unnamed namespace

namespace
{
  // types, internal

  // variables, internal

  av::Logger& logger(av::getLogger("av::Type"));

  class TypeData {

  public:

    std::string  mName;
    av::Type     mParent;
    av::Type     mType;
    av::Create*  mCreate;
    bool         mDistributable;
  };

  typedef std::vector<TypeData> TypeDataVec;
  typedef std::map<std::string, uint32_t> TypeMap;

  // store internal type information indexed by Type::_type_spec
  TypeDataVec type_data_vec;

  // name to Type::_type_spec mapping
  TypeMap type_map;

  // functions, internal

} // namespace

// variables, exported

// functions, exported

av::Type::Type()
{
  mTypeSpec.mId = 0;
}

av::Type::Type(const Type& rhs)
  : mTypeSpec(rhs.mTypeSpec)
{}

av::Type::Type(uint32_t id)
{
  mTypeSpec.mId = id;
}

av::Type::~Type()
{}

const av::Type&
av::Type::operator=(const Type& rhs)
{
  mTypeSpec = rhs.mTypeSpec;
  return *this;
}

/* static */ av::Type
av::Type::badType()
{
  return Type(0);
}

av::Type::operator uint32_t() const
{
  return mTypeSpec.mId;
}

uint32_t
av::Type::getId() const
{
  return mTypeSpec.mId;
}

unsigned int
av::Type::getIndex() const
{
  return mTypeSpec.mStorage.mIndex;
}

av::Type
av::Type::getParent() const
{
  return type_data_vec[getIndex()].mParent;
}

bool
av::Type::operator==(const Type rhs) const
{
  return mTypeSpec.mId == rhs.mTypeSpec.mId;
}

bool
av::Type::operator!=(const Type rhs) const
{
  return !(*this == rhs);
}

bool
av::Type::operator< (const Type rhs) const
{
  return mTypeSpec.mId < rhs.mTypeSpec.mId;
}

bool
av::Type::isBad() const
{
  return (*this == badType());
}

bool
av::Type::isPublic() const
{
  return mTypeSpec.mStorage.mIsPublic;
}

bool
av::Type::canCreate() const
{
  return mTypeSpec.mStorage.mCanCreate;
}

const std::string&
av::Type::getName() const
{
  return type_data_vec[getIndex()].mName;
}

/* static */ void
av::Type::init()
{
  initRegistry();
  Type bad_type = allocateNewType(false, false, false);
  registerNewType("BAD_TYPE", bad_type, bad_type, 0);
}

/* static */ void
av::Type::initRegistry()
{
  TypeDataVec emptyVec;
  type_data_vec.swap(emptyVec);
  type_data_vec.reserve(1);
  TypeMap emptyMap;
  type_map.swap(emptyMap);
}

/* static */ av::Type
av::Type::getByName(const std::string& typeName)
{
  TypeMap::const_iterator found_type_id = type_map.find(typeName);

  if (found_type_id == type_map.end()) {
    return badType();
  }

  return type_data_vec[found_type_id->second].mType;
}


/* static */ bool
av::Type::isTypeRegistered(const std::string& typeName)
{
  return getByName(typeName) != badType();
}

/* static */ av::Type
av::Type::allocateNewType(bool isPublic, bool canCreate, bool isAdaptor)
{
  Type new_type;

  new_type.mTypeSpec.mStorage.mIndex = type_data_vec.size();
  new_type.mTypeSpec.mStorage.mIsPublic = isPublic;
  new_type.mTypeSpec.mStorage.mCanCreate = canCreate;
  new_type.mTypeSpec.mStorage.mIsAdaptor = isAdaptor;
  return new_type;
}

/* static */ void
av::Type::registerNewType(const std::string& typeName, Type parentType,
                          Type newType,  Create* creator)
{
  TypeData type_data;

  type_data.mName = typeName;
  type_data.mParent = parentType;
  type_data.mType = newType;
  type_data.mCreate = creator;

  // inherit _distributable from parent class
  if (parentType == badType()) {
    type_data.mDistributable = false;
  } else {
    type_data.mDistributable = parentType.isDistributable();
  }

  type_data_vec.push_back(type_data);

  type_map[type_data.mName] = newType.getIndex();
}

av::Type
av::Type::createAbstractType(Type parentType, const std::string& typeName,
                             bool isPublic)
{
  // check for duplicates
  if (isTypeRegistered(typeName)) {
    AVANGO_LOG(logger,logging::FATAL , boost::str(boost::format("createAbstractType(): duplicate type name '%1%' used. exiting!") % typeName))

    ::abort();

  }

  // check if parent type exists
  if (parentType != badType() &&
      parentType.getIndex() >= type_data_vec.size()) {

    AVANGO_LOG(logger,logging::WARN , boost::str(boost::format("createAbstractType(): parent '%1%' of '%2%' doesn't exist yet (parent idx: %3%, max idx: %4%), returning 'badType'") % parentType.getName() % typeName % parentType.getIndex() % type_data_vec.size()))

    return badType();
  }

  // ok, lets do it
  Type new_type = allocateNewType(isPublic, false, false);

  registerNewType(typeName, parentType, new_type, 0);

  AVANGO_LOG(logger,logging::DEBUG , boost::str(boost::format("createAbstractType(): created abstract type '%s' (id: % 09d, idx: % 09d, parent: '%s').") % typeName % new_type.getId() % new_type.getIndex() % parentType.getName()))

  return new_type;
}

av::Type
av::Type::createType(Type parentType, const std::string& typeName,
                     Create* create, bool isPublic)
{
  // check for duplicates
  if (isTypeRegistered(typeName)) {
    AVANGO_LOG(logger,logging::FATAL , boost::str(boost::format("createType(): duplicate type name '%1%' used. exiting!") % typeName))

    ::abort();
  }

  // check if parent type exists
  if (parentType != badType() &&
      parentType.getIndex() >= type_data_vec.size()) {

      AVANGO_LOG(logger,logging::WARN , boost::str(boost::format("createType(): parent '%1%' of '%2%' doesn't exist yet (parent idx: %3%, max idx: %4%), returning 'badType'") % parentType.getName() % typeName % parentType.getIndex() % type_data_vec.size()))

    return badType();
  }

  // ok, lets do it
  Type new_type = allocateNewType(isPublic, true, false);
  registerNewType(typeName, parentType, new_type, create);

  AVANGO_LOG(logger,logging::DEBUG , boost::str(boost::format("createType(): created type '%s' (id: % 09d, idx: % 09d, parent: '%s')") % typeName % new_type.getId() % new_type.getIndex() % parentType.getName()))

  return new_type;
}

void
av::Type::setDistributable(bool isDistributable)
{
  type_data_vec[getIndex()].mDistributable = isDistributable;
}

bool
av::Type::isDistributable() const
{
  return type_data_vec[getIndex()].mDistributable;
}

bool
av::Type::isDerivedFrom(Type typeToCheck) const
{
  Type parent_type = getParent();

  while (parent_type != badType()) {
    if (typeToCheck == parent_type) {
      return true;
    }
    parent_type = parent_type.getParent();
  }
  return false;
}

bool
av::Type::isOfType(Type typeToCheck) const
{
  return (*this == typeToCheck || isDerivedFrom(typeToCheck));
}

av::TypeList
av::Type::getAllDerivedFrom() const
{
  TypeList derived_types;

  for (unsigned int i = getIndex(); i < type_data_vec.size(); ++i) {
    if (type_data_vec[i].mType.isDerivedFrom(*this)) {
      derived_types.push_back(type_data_vec[i].mType);
    }
  }
  return derived_types;
}

av::Typed*
av::Type::createInstance()
{
  if (!canCreate()) {
    return 0;
  }

  av::Typed* obj = type_data_vec[getIndex()].mCreate->makeInstance();

  AVANGO_LOG(logger,logging::DEBUG , boost::str(boost::format("createInstance(): created instance of type '%1%'") % type_data_vec[getIndex()].mName))

  return obj;
}

/* static */ av::Typed*
av::Type::createInstanceOfType(const std::string& typeName,
                               bool /*unloadable*/)
{
  Type type = getByName(typeName);

  if (type != badType())
    return type.createInstance();
  else
    throw std::invalid_argument("invalid type: " + typeName);
}

/* static */ void
av::Type::dumpTypeMap()
{
  AVANGO_LOG(logger,logging::INFO , boost::str(boost::format("dumpTypeMap(): dumping %1% types") % type_map.size()))

  size_t longest_name_size = 0;

  for (TypeMap::const_iterator current_type = type_map.begin();
       current_type != type_map.end(); ++current_type) {
    const size_t name_size ((*current_type).first.size());

    if (name_size > longest_name_size) {
      longest_name_size = name_size;
    }
  }

  for (TypeMap::const_iterator current_type = type_map.begin();
       current_type != type_map.end(); ++current_type) {
    const std::string    name (current_type->first);
    const int            index (current_type->second);
    const TypeData&      current_type_data (type_data_vec[index]);

    std::string derivation;

    {
      Type act_type = current_type_data.mType;

      while (!act_type.isBad()) {
        derivation += act_type.getName();

        act_type = act_type.getParent();

        if (!act_type.isBad())
          derivation += "->";
      }
    }

    std::string buffer(longest_name_size - name.size(), ' ');

    AVANGO_LOG(logger,logging::INFO , boost::str(boost::format("%1%'%2%' idx: %3% [derivation: %4%] [public: %5%] [distributable: %6%]") % buffer % name % index
        % (!derivation.empty() ? derivation : "none")
        % (current_type_data.mCreate ? "yes" : " no")
        % (current_type_data.mDistributable ? "yes" : " no")))

    buffer = std::string(longest_name_size + 17, ' ');

  }
}

av::InputStream& av::operator>>(av::InputStream& is, av::Type& value)
{
  uint32_t id;
  is >> id;
  value = Type(id);
  return is;
}

av::OutputStream& av::operator<<(av::OutputStream& os, const av::Type& value)
{
  uint32_t id = value.getId();
  os << id;
  return os;
}
