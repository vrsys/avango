// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#if !defined(AVANGO_OSG_OBJECTLINK_H)
#define AVANGO_OSG_OBJECTLINK_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Referenced>

#include <avango/osg/windows_specific_osg.h>

namespace av
{
  namespace osg
  {
    class Object;

    /**
     * This class is used as user data for ::osg::Object. For an ::osg::Object N,
     * it stores a link to the Avango object to which N belongs. It is needed for
     * retrieving the Avango object when traversing the osg scene graph directly.
     * It is also needed for implementing the Children field of the Avango group
     * node as an adaptor field.
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL ObjectLink : public ::osg::Referenced
    {

    public:

      /**
       * Constructor
       */
      ObjectLink(av::osg::Object* object);

      /**
       * Get the stored av::osg::Object
       */
      av::osg::Object* getObject() const;

    protected:

      /**
       * Destructor, protected according to ::osg::Referenced
       */
      ~ObjectLink();

      av::osg::Object* mObject;

    };

  } // namespace osg

} // namespave av

#endif
