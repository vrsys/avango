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

#include <avango/ObjectValue.h>

//  specialization of the members

template <> const std::string
av::ObjectValue<bool>::sClassTypeName = "av::BoolObject";

template <> const std::string
av::ObjectValue<int>::sClassTypeName = "av::IntObject";

template <> const std::string
av::ObjectValue<unsigned int>::sClassTypeName = "av::UIntObject";

template <> const std::string
av::ObjectValue<float>::sClassTypeName = "av::FloatObject";

template <> const std::string
av::ObjectValue<double>::sClassTypeName = "av::DoubleObject";

template <> const std::string
av::ObjectValue<av::Link<av::FieldContainer> >::sClassTypeName = "av::FieldContainerObject";

template <> const std::string
av::ObjectValue<std::string>::sClassTypeName = "av::StringObject";
