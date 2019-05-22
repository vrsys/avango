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

#if !defined(AVANGO_STANDARDFIELDS_H)
#define AVANGO_STANDARDFIELDS_H

#include <string>

#include <avango/MultiField.h>
#include <avango/SingleField.h>
#include <avango/windows_specific.h>

namespace av
{
#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_DLL SingleField<bool>;
template class AV_DLL MultiField<bool>;

template class AV_DLL SingleField<double>;
template class AV_DLL MultiField<double>;

template class AV_DLL SingleField<float>;
template class AV_DLL MultiField<float>;

template class AV_DLL SingleField<int32_t>;
template class AV_DLL MultiField<int32_t>;

template class AV_DLL SingleField<int64_t>;
template class AV_DLL MultiField<int64_t>;

template class AV_DLL SingleField<uint32_t>;
template class AV_DLL MultiField<uint32_t>;

template class AV_DLL SingleField<uint64_t>;
template class AV_DLL MultiField<uint64_t>;

template class AV_DLL SingleField<std::string>;
template class AV_DLL MultiField<std::string>;
#endif

using SFBool = SingleField<bool>;
using MFBool = MultiField<bool>;

using SFDouble = SingleField<double>;
using MFDouble = MultiField<double>;

using SFFloat = SingleField<float>;
using MFFloat = MultiField<float>;

using SFInt = SingleField<int32_t>;
using MFInt = MultiField<int32_t>;

using SFLong = SingleField<int64_t>;
using MFLong = MultiField<int64_t>;

using SFUInt = SingleField<uint32_t>;
using MFUInt = MultiField<uint32_t>;

using SFULong = SingleField<uint64_t>;
using MFULong = MultiField<uint64_t>;

using SFString = SingleField<std::string>;
using MFString = MultiField<std::string>;

void AV_DLL initStandardFields();
} // namespace av

#endif // #if !defined(AVANGO_STANDARDFIELDS_H)
