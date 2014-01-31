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

#include "../include/avango/utils/Init.h"
#include "../include/avango/utils/ProximitySensor.h"
#include "../include/avango/utils/Bool2Or.h"
#include "../include/avango/utils/Bool2And.h"
#include "../include/avango/utils/Bool3Or.h"
#include "../include/avango/utils/Bool3And.h"
#include "../include/avango/utils/Trackball.h"
#include "../include/avango/utils/MultiValueField.h"

#ifdef PCL_SUPPORT
  #include "../include/avango/utils/PCLPointCloud.h"
#endif

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::utils::Init"));
}

AV_TYPED_DEFINE_ABSTRACT(av::utils::Init);

/* static */ void
av::utils::Init::initClass()
{
  if (!isTypeInitialized())
  {
    av::utils::ProximitySensor::initClass();
    av::utils::Bool2Or::initClass();
    av::utils::Bool2And::initClass();
    av::utils::Bool3Or::initClass();
    av::utils::Bool3And::initClass();
    av::utils::Trackball::initClass();
    av::utils::initMultiValueFields();
    av::utils::initMultiValueOSGFields();

    #ifdef PCL_SUPPORT
      av::utils::PCLPointCloud::initClass();
    #endif

    AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::utils::Init", true);
  }
}
