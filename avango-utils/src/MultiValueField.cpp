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

#include <avango/StandardFields.h>
#include <avango/osg/Fields.h>
#include "../include/avango/utils/MultiValueField.h"

/**
 * standard type fields
 */
AV_FIELD_DEFINE(av::utils::MVFBool)
AV_FIELD_DEFINE(av::utils::MVFDouble)
AV_FIELD_DEFINE(av::utils::MVFFloat)
AV_FIELD_DEFINE(av::utils::MVFInt)
AV_FIELD_DEFINE(av::utils::MVFLong)
AV_FIELD_DEFINE(av::utils::MVFUInt)
AV_FIELD_DEFINE(av::utils::MVFULong)
AV_FIELD_DEFINE(av::utils::MVFString)

/*
 * OSG type fields. Should be moved to av::osg::Field.h
 * as soon as this class is factored into avango-core (if ever)
 */
AV_FIELD_DEFINE(av::utils::MVFMatrixf);
AV_FIELD_DEFINE(av::utils::MVFMatrixd);
AV_FIELD_DEFINE(av::utils::MVFVec2f);
AV_FIELD_DEFINE(av::utils::MVFVec2d);
AV_FIELD_DEFINE(av::utils::MVFVec3f);
AV_FIELD_DEFINE(av::utils::MVFVec3d);
AV_FIELD_DEFINE(av::utils::MVFVec4f);
AV_FIELD_DEFINE(av::utils::MVFVec4d);
AV_FIELD_DEFINE(av::utils::MVFQuat);


void
av::utils::initMultiValueFields()
{
  av::initStandardFields();

  av::utils::MVFBool::initClass("av::utils::MVFBool", "av::MFBool");
  av::utils::MVFDouble::initClass("av::utils::MVFDouble", "av::MFDouble");
  av::utils::MVFFloat::initClass("av::utils::MVFFloat", "av::MFFloat");
  av::utils::MVFInt::initClass("av::utils::MVFInt", "av::MFInt");
  av::utils::MVFLong::initClass("av::utils::MVFLong", "av::MFLong");
  av::utils::MVFUInt::initClass("av::utils::MVFUInt", "av::MFInt");
  av::utils::MVFULong::initClass("av::utils::MVFULong", "av::MFULong");
  av::utils::MVFString::initClass("av::utils::MVFString", "av::MFString");
}

void
av::utils::initMultiValueOSGFields()
{
  av::osg::initFields();

  MVFMatrixf::initClass("av::utils::MVFMatrixf", "av::osg::MFMatrixf");
  MVFMatrixd::initClass("av::utils::MVFMatrixd", "av::osg::MFMatrixd");
  MVFVec2f::initClass("av::utils::MVFVec2f", "av::osg::MFVec2f");
  MVFVec2d::initClass("av::utils::MVFVec2d", "av::osg::MFVec2d");
  MVFVec3f::initClass("av::utils::MVFVec3f", "av::osg::MFVec3f");
  MVFVec3d::initClass("av::utils::MVFVec3d", "av::osg::MFVec3d");
  MVFVec4f::initClass("av::utils::MVFVec4f", "av::osg::MFVec4f");
  MVFVec4d::initClass("av::utils::MVFVec4d", "av::osg::MFVec4d");
  MVFQuat::initClass("av::utils::MVFQuat", "av::osg::MFQuat");
}


namespace
{

  template <typename FieldType>
  bool fieldHasType(av::Field* field)
  {
    AV_ASSERT(field);
    return field->getTypeId().isOfType(av::utils::MultiValueField<FieldType>::getClassTypeId());
  }

  template <typename FromType, typename ToType>
  void setFieldValues(av::Field* fromField, av::utils::MultiValueField<ToType>* toMVField)
  {
    AV_ASSERT(fromField);
    AV_ASSERT(toMVField);

    av::utils::MultiValueField<FromType> *fromMVField = dynamic_cast<av::utils::MultiValueField<FromType>*>(fromField);
    AV_ASSERT(fromMVField);
    uint sz(fromMVField->getSize());
    toMVField->resize(sz);
    uint last_changed_ind = sz;

    for (uint i = 0; i < sz; i++)
    {
      if (fromMVField->valueHasChanged(i))
      {
        toMVField->set1Value(fromMVField->get1Value(i), i, fromField, false);
        last_changed_ind = i;
      }
    }

    if (last_changed_ind < sz)
    {
      toMVField->triggerFieldChange(fromField);
    }
  }
}

template<>
void av::utils::MultiValueField<double>::pullValueImpl(av::Field* fromField)
{
  if (fieldHasType<double>(fromField))
  {
    setFieldValues<double, double>(fromField, this);
  }
  else if (fieldHasType<float>(fromField))
  {
    setFieldValues<float, double>(fromField, this);
  }
  else if (fieldHasType<int32_t>(fromField))
  {
    setFieldValues<int32_t, double>(fromField, this);
  }
  else if (fieldHasType<int64_t>(fromField))
  {
    setFieldValues<int64_t, double>(fromField, this);
  }
  else if (fieldHasType<uint32_t>(fromField))
  {
    setFieldValues<uint32_t, double>(fromField, this);
  }
  else if (fieldHasType<uint64_t>(fromField))
  {
    setFieldValues<uint64_t, double>(fromField, this);
  }
  else if (fieldHasType<bool>(fromField))
  {
    setFieldValues<bool, double>(fromField, this);
  }
  else
  {
    throw std::invalid_argument("pullValue: type mismatch. " +
                                getTypeId().getName() +
                                " <--> " +
                                fromField->getTypeId().getName());
  }
}

template<>
void av::utils::MultiValueField<float>::pullValueImpl(av::Field* fromField)
{
  if (fieldHasType<double>(fromField))
  {
    setFieldValues<double, float>(fromField, this);
  }
  else if (fieldHasType<float>(fromField))
  {
    setFieldValues<float, float>(fromField, this);
  }
  else if (fieldHasType<int32_t>(fromField))
  {
    setFieldValues<int32_t, float>(fromField, this);
  }
  else if (fieldHasType<int64_t>(fromField))
  {
    setFieldValues<int64_t, float>(fromField, this);
  }
  else if (fieldHasType<uint32_t>(fromField))
  {
    setFieldValues<uint32_t, float>(fromField, this);
  }
  else if (fieldHasType<uint64_t>(fromField))
  {
    setFieldValues<uint64_t, float>(fromField, this);
  }
  else if (fieldHasType<bool>(fromField))
  {
    setFieldValues<bool, float>(fromField, this);
  }
  else
  {
    throw std::invalid_argument("pullValue: type mismatch. " +
                                getTypeId().getName() +
                                " <--> " +
                                fromField->getTypeId().getName());
  }
}

template<>
void av::utils::MultiValueField<int32_t>::pullValueImpl(av::Field* fromField)
{
  if (fieldHasType<double>(fromField))
  {
    setFieldValues<double, int32_t>(fromField, this);
  }
  else if (fieldHasType<float>(fromField))
  {
    setFieldValues<float, int32_t>(fromField, this);
  }
  else if (fieldHasType<int32_t>(fromField))
  {
    setFieldValues<int32_t, int32_t>(fromField, this);
  }
  else if (fieldHasType<int64_t>(fromField))
  {
    setFieldValues<int64_t, int32_t>(fromField, this);
  }
  else if (fieldHasType<uint32_t>(fromField))
  {
    setFieldValues<uint32_t, int32_t>(fromField, this);
  }
  else if (fieldHasType<uint64_t>(fromField))
  {
    setFieldValues<uint64_t, int32_t>(fromField, this);
  }
  else if (fieldHasType<bool>(fromField))
  {
    setFieldValues<bool, int32_t>(fromField, this);
  }
  else
  {
    throw std::invalid_argument("pullValue: type mismatch. " +
                                getTypeId().getName() +
                                " <--> " +
                                fromField->getTypeId().getName());
  }
}

template<>
void av::utils::MultiValueField<int64_t>::pullValueImpl(av::Field* fromField)
{
  if (fieldHasType<double>(fromField))
  {
    setFieldValues<double, int64_t>(fromField, this);
  }
  else if (fieldHasType<float>(fromField))
  {
    setFieldValues<float, int64_t>(fromField, this);
  }
  else if (fieldHasType<int32_t>(fromField))
  {
    setFieldValues<int32_t, int64_t>(fromField, this);
  }
  else if (fieldHasType<int64_t>(fromField))
  {
    setFieldValues<int64_t, int64_t>(fromField, this);
  }
  else if (fieldHasType<uint32_t>(fromField))
  {
    setFieldValues<uint32_t, int64_t>(fromField, this);
  }
  else if (fieldHasType<uint64_t>(fromField))
  {
    setFieldValues<uint64_t, int64_t>(fromField, this);
  }
  else if (fieldHasType<bool>(fromField))
  {
    setFieldValues<bool, int64_t>(fromField, this);
  }
  else
  {
    throw std::invalid_argument("pullValue: type mismatch. " +
                                getTypeId().getName() +
                                " <--> " +
                                fromField->getTypeId().getName());
  }
}

template<>
void av::utils::MultiValueField<uint32_t>::pullValueImpl(av::Field* fromField)
{
  if (fieldHasType<double>(fromField))
  {
    setFieldValues<double, uint32_t>(fromField, this);
  }
  else if (fieldHasType<float>(fromField))
  {
    setFieldValues<float, uint32_t>(fromField, this);
  }
  else if (fieldHasType<int32_t>(fromField))
  {
    setFieldValues<int32_t, uint32_t>(fromField, this);
  }
  else if (fieldHasType<int64_t>(fromField))
  {
    setFieldValues<int64_t, uint32_t>(fromField, this);
  }
  else if (fieldHasType<uint32_t>(fromField))
  {
    setFieldValues<uint32_t, uint32_t>(fromField, this);
  }
  else if (fieldHasType<uint64_t>(fromField))
  {
    setFieldValues<uint64_t, uint32_t>(fromField, this);
  }
  else if (fieldHasType<bool>(fromField))
  {
    setFieldValues<bool, uint32_t>(fromField, this);
  }
  else
  {
    throw std::invalid_argument("pullValue: type mismatch. " +
                                getTypeId().getName() +
                                " <--> " +
                                fromField->getTypeId().getName());
  }
}

template<>
void av::utils::MultiValueField<uint64_t>::pullValueImpl(av::Field* fromField)
{
  if (fieldHasType<double>(fromField))
  {
    setFieldValues<double, uint64_t>(fromField, this);
  }
  else if (fieldHasType<float>(fromField))
  {
    setFieldValues<float, uint64_t>(fromField, this);
  }
  else if (fieldHasType<int32_t>(fromField))
  {
    setFieldValues<int32_t, uint64_t>(fromField, this);
  }
  else if (fieldHasType<int64_t>(fromField))
  {
    setFieldValues<int64_t, uint64_t>(fromField, this);
  }
  else if (fieldHasType<uint32_t>(fromField))
  {
    setFieldValues<uint32_t, uint64_t>(fromField, this);
  }
  else if (fieldHasType<uint64_t>(fromField))
  {
    setFieldValues<uint64_t, uint64_t>(fromField, this);
  }
  else if (fieldHasType<bool>(fromField))
  {
    setFieldValues<bool, uint64_t>(fromField, this);
  }
  else
  {
    throw std::invalid_argument("pullValue: type mismatch. " +
                                getTypeId().getName() +
                                " <--> " +
                                fromField->getTypeId().getName());
  }
}

template<>
void av::utils::MultiValueField<bool>::pullValueImpl(av::Field* fromField)
{
  if (fieldHasType<double>(fromField))
  {
    setFieldValues<double, bool>(fromField, this);
  }
  else if (fieldHasType<float>(fromField))
  {
    setFieldValues<float, bool>(fromField, this);
  }
  else if (fieldHasType<int32_t>(fromField))
  {
    setFieldValues<int32_t, bool>(fromField, this);
  }
  else if (fieldHasType<int64_t>(fromField))
  {
    setFieldValues<int64_t, bool>(fromField, this);
  }
  else if (fieldHasType<uint32_t>(fromField))
  {
    setFieldValues<uint32_t, bool>(fromField, this);
  }
  else if (fieldHasType<uint64_t>(fromField))
  {
    setFieldValues<uint64_t, bool>(fromField, this);
  }
  else if (fieldHasType<bool>(fromField))
  {
    setFieldValues<bool, bool>(fromField, this);
  }
  else
  {
    throw std::invalid_argument("pullValue: type mismatch. " +
                                getTypeId().getName() +
                                " <--> " +
                                fromField->getTypeId().getName());
  }
}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

template<> void
av::utils::MultiValueField<float>::push(av::Msg& msg)
{
  av_pushMsg(msg, mValue);
}

template<> void
av::utils::MultiValueField<float>::pop(av::Msg& msg)
{
  av_popMsg(msg, mValue);
  fieldChanged(true);
}

template<> void
av::utils::MultiValueField<double>::push(av::Msg& msg)
{
  av_pushMsg(msg, mValue);
}

template<> void
av::utils::MultiValueField<double>::pop(av::Msg& msg)
{
  av_popMsg(msg, mValue);
  fieldChanged(true);
}

template<> void
av::utils::MultiValueField<int32_t>::push(av::Msg& msg)
{
  av_pushMsg(msg, mValue);
}

template<> void
av::utils::MultiValueField<int32_t>::pop(av::Msg& msg)
{
  av_popMsg(msg, mValue);
  fieldChanged(true);
}

template<> void
av::utils::MultiValueField<uint32_t>::push(av::Msg& msg)
{
  av_pushMsg(msg, mValue);
}

template<> void
av::utils::MultiValueField<uint32_t>::pop(av::Msg& msg)
{
  av_popMsg(msg, mValue);
  fieldChanged(true);
}

template<> void
av::utils::MultiValueField<int64_t>::push(av::Msg& msg)
{
  av_pushMsg(msg, mValue);
}

template<> void
av::utils::MultiValueField<int64_t>::pop(av::Msg& msg)
{
  av_popMsg(msg, mValue);
  fieldChanged(true);
}

template<> void
av::utils::MultiValueField<uint64_t>::push(av::Msg& msg)
{
  av_pushMsg(msg, mValue);
}

template<> void
av::utils::MultiValueField<uint64_t>::pop(av::Msg& msg)
{
  av_popMsg(msg, mValue);
  fieldChanged(true);
}

#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)

