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

#include <avango/Reader.h>

#include <fstream>

#include <avango/Base.h>
#include <avango/Field.h>
#include <avango/FieldContainer.h>
#include <avango/InputStream.h>
#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::Reader"));
}

av::Reader::Reader() : mBinary(true) {}

av::Reader::~Reader() {}

av::Base* av::Reader::readObject(av::InputStream& stream)
{
    // read object type
    std::string type_str;
    stream >> type_str;

    AVANGO_LOG(logger, logging::INFO, boost::str(boost::format("readObject: type: %1%") % type_str));

    if(stream.eof())
    {
        AVANGO_LOG(logger, logging::WARN, "readObject: eof encountered");
        return 0;
    }

    Link<Base> object = (Base*)Type::createInstanceOfType(type_str);

    if(!object.isValid())
    {
        AVANGO_LOG(logger, logging::WARN, boost::str(boost::format("readObject: illegal type name '%1%'") % type_str));
        return 0;
    }

    // read object id
    std::string obj_id;
    stream >> obj_id;

    AVANGO_LOG(logger, logging::INFO, boost::str(boost::format("readObject: id: ") % obj_id));

    if(stream.fail() || stream.bad())
    {
        AVANGO_LOG(logger, logging::WARN, boost::str(boost::format("readObject: error reading object id for '%1%'") % type_str))
        return 0;
    }

    // read object
    stream >> object.getPtr();

    if(stream.fail() || stream.bad())
    {
        AVANGO_LOG(logger, logging::WARN, boost::str(boost::format("readObject: error reading object of type '%1%'") % type_str));
        return 0;
    }

    mObjectMap.insert(StringBaseMap::value_type(obj_id, object));

    return object.getPtr();
}

av::Base* av::Reader::readFromFile(const std::string& filename)
{
    std::ifstream* file = new std::ifstream(filename.c_str());

    if(!file || file->bad())
    {
        AVANGO_LOG(logger, logging::WARN, boost::str(boost::format("readFromFile: can't open file '%1%'") % filename));
        return 0;
    }

    AVANGO_LOG(logger, logging::INFO, boost::str(boost::format("readFromFile: '%1%'") % filename));

    InputStream* stream = new InputStream(*file);

    stream->enableBinary(isBinaryEnabled());
    stream->setReader(this);

    mObjectMap.clear();
    mConnectionMap.clear();

    // recursively read object hierachy
    Base* root = readObject(*stream);

    delete stream;
    delete file;

    // resolve pending connections
    for(ConnectionMap::iterator i = mConnectionMap.begin(); i != mConnectionMap.end(); ++i)
    {
        Field* to_field = (*i).first;
        std::string from_id = (*i).second.first;
        int from_index = (*i).second.second;

        Base* from_base = lookupObject(from_id);
        if(from_base && from_base->getTypeId().isOfType(FieldContainer::getClassTypeId()))
        {
            FieldContainer* fc = (FieldContainer*)from_base;
            Field* from_field = fc->getField(from_index);

            to_field->connectFrom(from_field);
        }
        else
        {
            AVANGO_LOG(logger, logging::WARN, boost::str(boost::format("readFromFile: error resolving connection from object '%1%'!") % from_id));
        }
    }

    AVANGO_LOG(logger, logging::INFO, boost::str(boost::format("readFromFile: %1% connections resolved!") % mConnectionMap.size()));

    mObjectMap.clear();
    mConnectionMap.clear();
    return root;
}

av::Base* av::Reader::lookupObject(const std::string& id)
{
    StringBaseMap::iterator i = mObjectMap.find(id);

    if(i != mObjectMap.end())
    {
        return (*i).second.getPtr();
    }

    return 0;
}

void av::Reader::addConnection(av::Field* toField, const std::string& fromId, int fromIndex)
{
    mConnectionMap.insert(ConnectionMap::value_type(toField, std::pair<std::string, int>(fromId, fromIndex)));
}

void av::Reader::enableBinary(bool bin) { mBinary = bin; }

bool av::Reader::isBinaryEnabled() { return mBinary; }
