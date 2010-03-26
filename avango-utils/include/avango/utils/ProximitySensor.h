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

#if !defined(AV_UTILS_PROXIMITYSENSOR_H)
#define AV_UTILS_PROXIMITYSENSOR_H

/**
 * \file
 * \ingroup av_utils
 */

#include <avango/FieldContainer.h>
#include <avango/StandardFields.h>
#include <avango/osg/Fields.h>
#include "windows_specific_utils.h"

namespace av
{
  namespace utils
  {

    class AV_UTILS_DLL ProximitySensor : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      ProximitySensor();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~ProximitySensor();

    public:

      /** Distance where Proximity reaches 1. */
      av::SFDouble MinDistance;

      /** Distance where Proximity starts to increase from 0. */
      av::SFDouble MaxDistance;

      /** Center of Sensor */
      av::osg::SFMatrix ReferencePosition;

      /** Current postion of target */
      av::osg::SFMatrix CurrentPosition;

      /** Proximity of target expressed in intervall [0, 1] */
      av::SFDouble Proximity;

      /** If ProximitySensor is triggered outputs 0, -1 otherwise */
      av::SFInt Switch;

      /* virtual */ void evaluate();

    private:
      void set_proximity_value(double proximity);
      void set_switch_value(int switch_);
      double smooth(double x);
    };

  }
}

#endif
