// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include <avango/SingleField.h>

namespace
{
template <typename FieldType>
bool fieldHasType(av::Field* field)
{
    AV_ASSERT(field);
    return field->getTypeId().isOfType(av::SingleField<FieldType>::getClassTypeId());
}

template <typename FromType, typename ToType>
ToType convertFieldValue(av::Field* field)
{
    av::SingleField<FromType>* from_field = dynamic_cast<av::SingleField<FromType>*>(field);
    AV_ASSERT(from_field);
    return static_cast<ToType>(from_field->getValue());
}

} // namespace

template <>
void av::SingleField<double>::pullValueImpl(av::Field* fromField)
{
    AV_ASSERT(fromField);
    if(fieldHasType<double>(fromField))
    {
        setValue(convertFieldValue<double, double>(fromField), fromField);
    }
    else if(fieldHasType<float>(fromField))
    {
        setValue(convertFieldValue<float, double>(fromField), fromField);
    }
    else if(fieldHasType<int32_t>(fromField))
    {
        setValue(convertFieldValue<int32_t, double>(fromField), fromField);
    }
    else if(fieldHasType<int64_t>(fromField))
    {
        setValue(convertFieldValue<int64_t, double>(fromField), fromField);
    }
    else if(fieldHasType<uint32_t>(fromField))
    {
        setValue(convertFieldValue<uint32_t, double>(fromField), fromField);
    }
    else if(fieldHasType<uint64_t>(fromField))
    {
        setValue(convertFieldValue<uint64_t, double>(fromField), fromField);
    }
    else if(fieldHasType<bool>(fromField))
    {
        setValue(convertFieldValue<bool, double>(fromField), fromField);
    }
    else
    {
        throw std::invalid_argument("pullValue: type mismatch. " + getTypeId().getName() + " <--> " + fromField->getTypeId().getName());
    }
}

template <>
void av::SingleField<float>::pullValueImpl(av::Field* fromField)
{
    AV_ASSERT(fromField);
    if(fieldHasType<float>(fromField))
    {
        setValue(convertFieldValue<float, float>(fromField), fromField);
    }
    else if(fieldHasType<double>(fromField))
    {
        setValue(convertFieldValue<double, float>(fromField), fromField);
    }
    else if(fieldHasType<int32_t>(fromField))
    {
        setValue(convertFieldValue<int32_t, float>(fromField), fromField);
    }
    else if(fieldHasType<int64_t>(fromField))
    {
        setValue(convertFieldValue<int64_t, float>(fromField), fromField);
    }
    else if(fieldHasType<uint32_t>(fromField))
    {
        setValue(convertFieldValue<uint32_t, float>(fromField), fromField);
    }
    else if(fieldHasType<uint64_t>(fromField))
    {
        setValue(convertFieldValue<uint64_t, float>(fromField), fromField);
    }
    else if(fieldHasType<bool>(fromField))
    {
        setValue(convertFieldValue<bool, float>(fromField), fromField);
    }
    else
    {
        throw std::invalid_argument("pullValue: type mismatch. " + getTypeId().getName() + " <--> " + fromField->getTypeId().getName());
    }
}

template <>
void av::SingleField<int32_t>::pullValueImpl(av::Field* fromField)
{
    AV_ASSERT(fromField);
    if(fieldHasType<int32_t>(fromField))
    {
        setValue(convertFieldValue<int32_t, int32_t>(fromField), fromField);
    }
    else if(fieldHasType<int64_t>(fromField))
    {
        setValue(convertFieldValue<int64_t, int32_t>(fromField), fromField);
    }
    else if(fieldHasType<uint32_t>(fromField))
    {
        setValue(convertFieldValue<uint32_t, int32_t>(fromField), fromField);
    }
    else if(fieldHasType<uint64_t>(fromField))
    {
        setValue(convertFieldValue<uint64_t, int32_t>(fromField), fromField);
    }
    else if(fieldHasType<bool>(fromField))
    {
        setValue(convertFieldValue<bool, int32_t>(fromField), fromField);
    }
    else
    {
        throw std::invalid_argument("pullValue: type mismatch. " + getTypeId().getName() + " <--> " + fromField->getTypeId().getName());
    }
}

template <>
void av::SingleField<int64_t>::pullValueImpl(av::Field* fromField)
{
    AV_ASSERT(fromField);
    if(fieldHasType<int64_t>(fromField))
    {
        setValue(convertFieldValue<int64_t, int64_t>(fromField), fromField);
    }
    else if(fieldHasType<uint32_t>(fromField))
    {
        setValue(convertFieldValue<uint32_t, int64_t>(fromField), fromField);
    }
    else if(fieldHasType<uint64_t>(fromField))
    {
        setValue(convertFieldValue<uint64_t, int64_t>(fromField), fromField);
    }
    else if(fieldHasType<bool>(fromField))
    {
        setValue(convertFieldValue<bool, int64_t>(fromField), fromField);
    }
    else if(fieldHasType<int32_t>(fromField))
    {
        setValue(convertFieldValue<int32_t, int64_t>(fromField), fromField);
    }
    else
    {
        throw std::invalid_argument("pullValue: type mismatch. " + getTypeId().getName() + " <--> " + fromField->getTypeId().getName());
    }
}

template <>
void av::SingleField<uint32_t>::pullValueImpl(av::Field* fromField)
{
    AV_ASSERT(fromField);
    if(fieldHasType<uint32_t>(fromField))
    {
        setValue(convertFieldValue<uint32_t, uint32_t>(fromField), fromField);
    }
    else if(fieldHasType<int64_t>(fromField))
    {
        setValue(convertFieldValue<int64_t, uint32_t>(fromField), fromField);
    }
    else if(fieldHasType<int32_t>(fromField))
    {
        setValue(convertFieldValue<int32_t, uint32_t>(fromField), fromField);
    }
    else if(fieldHasType<uint64_t>(fromField))
    {
        setValue(convertFieldValue<uint64_t, uint32_t>(fromField), fromField);
    }
    else if(fieldHasType<bool>(fromField))
    {
        setValue(convertFieldValue<bool, uint32_t>(fromField), fromField);
    }
    else
    {
        throw std::invalid_argument("pullValue: type mismatch. " + getTypeId().getName() + " <--> " + fromField->getTypeId().getName());
    }
}

template <>
void av::SingleField<uint64_t>::pullValueImpl(av::Field* fromField)
{
    AV_ASSERT(fromField);
    if(fieldHasType<uint64_t>(fromField))
    {
        setValue(convertFieldValue<uint64_t, uint64_t>(fromField), fromField);
    }
    else if(fieldHasType<int64_t>(fromField))
    {
        setValue(convertFieldValue<int64_t, uint64_t>(fromField), fromField);
    }
    else if(fieldHasType<int32_t>(fromField))
    {
        setValue(convertFieldValue<int32_t, uint64_t>(fromField), fromField);
    }
    else if(fieldHasType<uint32_t>(fromField))
    {
        setValue(convertFieldValue<uint32_t, uint64_t>(fromField), fromField);
    }
    else if(fieldHasType<bool>(fromField))
    {
        setValue(convertFieldValue<bool, uint64_t>(fromField), fromField);
    }
    else
    {
        throw std::invalid_argument("pullValue: type mismatch. " + getTypeId().getName() + " <--> " + fromField->getTypeId().getName());
    }
}

template <>
void av::SingleField<bool>::pullValueImpl(av::Field* fromField)
{
    AV_ASSERT(fromField);
    if(fieldHasType<bool>(fromField))
    {
        setValue(convertFieldValue<bool, bool>(fromField), fromField);
    }
    else if(fieldHasType<int64_t>(fromField))
    {
        setValue(convertFieldValue<int64_t, bool>(fromField), fromField);
    }
    else if(fieldHasType<int32_t>(fromField))
    {
        setValue(convertFieldValue<int32_t, bool>(fromField), fromField);
    }
    else if(fieldHasType<uint32_t>(fromField))
    {
        setValue(convertFieldValue<uint32_t, bool>(fromField), fromField);
    }
    else if(fieldHasType<uint64_t>(fromField))
    {
        setValue(convertFieldValue<uint64_t, bool>(fromField), fromField);
    }
    else if(fieldHasType<double>(fromField))
    {
        setValue(convertFieldValue<double, bool>(fromField), fromField);
    }
    else if(fieldHasType<float>(fromField))
    {
        setValue(convertFieldValue<float, bool>(fromField), fromField);
    }
    else
    {
        throw std::invalid_argument("pullValue: type mismatch. " + getTypeId().getName() + " <--> " + fromField->getTypeId().getName());
    }
}
