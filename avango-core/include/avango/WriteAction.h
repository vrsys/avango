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

#if !defined(AV_WRITEACTION_H)
#define AV_WRITEACTION_H

/**
 * \file
 * \ingroup av
 */

// includes, system

#include <map>
#include <string>

// includes, project

#include <avango/Action.h>
#include <avango/OutputStream.h>

namespace av
{

  class Base;

  // types, exported (class, enum, struct, union, typedef)

  /**
   * Write node hierarchies to a file.
   * \ingroup av
   */
  class AV_DLL WriteAction : public Action
  {

    AV_BASE_DECLARE();

  public:

    /**
     * Constructor
     */
    WriteAction();

    /**
     * Construct WriteAction and set file name.
     */
    WriteAction(const std::string& name);

  protected:

    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~WriteAction();

  public:

    /**
     * Sets the file name.
     */
    void setFileName(const std::string& name);


    /**
     * Returns the output stream associated to the WriteAction.
     * Objects written to the stream actually go into the file associated
     * associated with the WriteAction.
     */
    OutputStream& getStream();

    /**
     * Returns the object id of the object referenced by obj if valid,
     * "null" otherwise. A new object id is created on demand.
     */
    std::string lookupObjectId(Link<Base> obj);

    /**
     * Returns \c true if \c obj has been written out to the current
     * file.
     */
    bool isObjectWritten(Link<Base> obj);

    /**
     * Switch for writing in binary mode.
     */
    void enableBinary(bool bin);

    /**
     * Return \true if we are writing in binary mode.
     */
    bool isBinaryEnabled();

    // inherited from Action

    /* virtual */ void apply(Link<Base> node);
    /* virtual */ void traverse(Link<Base> node);

  private:

    std::string getNewId();
    std::string depthIndent();

    typedef std::map<Link<Base>, std::string, std::less<Link<Base> > >
    BaseStringMap;

    BaseStringMap mIdMap;

    int mIdCounter;
    int mBinary;
    int mTravDepth;
    std::string mName;
    OutputStream mOutStream;

  };

  // variables, exported (extern)

  // functions, inlined (inline)

  // functions, exported (extern)

}

#endif // #if !defined(AV_WRITEACTION_H)
