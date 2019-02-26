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

#if !defined(AV_READER_H)
#define AV_READER_H

#include <map>
#include <vector>

#include <avango/Link.h>

namespace av
{
class Field;
class InputStream;
class Base;

/**
 * Reads Avango objects from a file or an InputStream.
 */
class AV_DLL Reader
{
  public:
    using BaseLinkVec = std::vector<Link<Base>>;
    using StringBaseMap = std::map<std::string, Link<Base>>;
    using ConnectionMap = std::map<Field*, std::pair<std::string, int>>;

    Reader();
    ~Reader();

    Base* readFromFile(const std::string& filename);
    Base* readObject(InputStream&);

    Base* lookupObject(const std::string& id);
    void addConnection(Field* toField, const std::string& fromId, int fromIndex);

    void enableBinary(bool bin);
    bool isBinaryEnabled();

  private:
    bool mBinary;

    StringBaseMap mObjectMap;
    ConnectionMap mConnectionMap;
};

} // namespace av

#endif // #if !defined(AVANGO_LIBFP_LIBFP_FPREADER_H)
