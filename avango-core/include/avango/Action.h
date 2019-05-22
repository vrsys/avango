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

#if !defined(AV_ACTION_H)
#define AV_ACTION_H

/**
 * \file
 * \ingroup av
 */

// includes, system

// includes, project

#include <avango/Base.h>
#include <avango/Link.h>

namespace av
{
// types, exported (class, enum, struct, union, typedef)

/**
 * %Base class for all action classes.
 *
 * Action allows operations to be applied to nodes in  hierarchies.
 * Concrete actions usually re-implement apply() to add initialization
 * code to the traversal and add per node functionality to traverse().
 *
 * traverse() itself calls Base::doAction(*this) for node
 * specific recursion. You should invoke the parent traverse()
 * function in classes inherited from Action.
 *
 * \ingroup av
 *
 */
class AV_DLL Action : public Base
{
    AV_BASE_DECLARE();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Action();

  public:
    /**
     * Apply action to nodes.
     * \param node Node to which the action applies
     */
    virtual void apply(Link<Base> node);

    /**
     * Node traversal function.
     * \param node Node to which the action applies
     */
    virtual void traverse(Link<Base> node);

  protected:
    /**
     * Constructor.
     * Made protected to prevent instantiation of Action objects.
     */
    Action();
};

// variables, exported (extern)

// functions, inlined (inline)

// functions, exported (extern)

} // namespace av

#endif // #if !defined(AV_ACTION_H)
