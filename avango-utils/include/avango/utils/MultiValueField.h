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

#if !defined(AVANGO_UTILS_MULTIVALUEFIELD_H)
#define AVANGO_UTILS_MULTIVALUEFIELD_H

#include <avango/Field.h>
#include <avango/osg/Types.h>
#include "windows_specific_utils.h"

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#  include <avango/Msg.h>
#endif

namespace av
{
  namespace utils
  {
    /**
     * Multi value holder, extended by the ability to set, get, and insert individual values.
     * This can be used as an alternative to MultiField, which provides only limited capabilities
     * for setting and getting values: values can be set and read only on the whole. Individual
     * components can only be appended to the end. Opposed to this, MultiValueField allows to
     * set and get individual components of the vector, offering an interface similar to
     * std::vector (partly). In addition, some of the setter functions allow the client to decide
     * whether the fieldChanged() function is called or not.
     *
     */
    template <typename Value>
    class MultiValueField : public Field
    {

    public:

      typedef Field              SuperType;
      typedef std::vector<Value> ContainerType;
      typedef Value              ValueType;

      MultiValueField()
      {}

      MultiValueField(const MultiValueField&)
      {}

      virtual ~MultiValueField()
      {}

      static void initClass(const std::string& classname, const std::string& parentname)
      {
        if (Type::badType() == sClassTypeId) {
          sClassTypeId = Field::registerType(classname, parentname,
                                             new CreateA<MultiValueField<Value> >());
        }
      }

      static Type getClassTypeId()
      {
        return sClassTypeId;
      }

      virtual Type getTypeId() const
      {
        return sClassTypeId;
      }

      void bind(FieldContainer* container, const std::string& name,
                 bool owned, const ContainerType& init)
      {
        Field::bind(container, name, owned);
        mValue = init;
        mChangedValues = std::vector<bool>(init.size(), true);
      }

      /**
       * Indicates if the value at position i was changed. Call it in fieldHasChanged()
       * of the field container using this field.
       */
      virtual bool valueHasChanged(uint i) const
      {
        return mChangedValues.at(i);
      }

      /**
       * Returns a vector of indices that point to changed values.
       * Call it in fieldHasChanged() of the field container using this field.
       */
      virtual std::vector<uint> changedValueInds() const
      {
        std::vector<uint> inds;

        for (uint i = 0; i < mChangedValues.size(); i++)
          if (mChangedValues[i])
            inds.push_back(i);

        return inds;
      }

      /**
       * Resets the flags for changed individual values. Should be called at the end of evaluate()
       * of each field container that uses a MultiValueField. A better solution would be to re-implement
       * Field::reset().
       */
      virtual void resetChangedFlags()
      {
        mChangedValues = std::vector<bool>(mValue.size(), false);
      }

      /**
       * Sets some values of the multi field. The indices into the multi field should be
       * passed in v_inds. The new values should be passed in v.
       */
      virtual void setSomeValues(const ContainerType& v, const std::vector<uint>& v_inds)
      {
        for (uint u = 0; u < v_inds.size(); u++)
          set1Value(v.at(u), v_inds[u], false);

        if (v_inds.size() > 0)
          triggerFieldChange(0);
      }

      /**
       * This is effectively the same as MultiField::setValue().
       */
      virtual void setAllValues(const ContainerType& v)
      {
        setAllValues(v, 0);
      }

      /**
       * Same as MultiField::getValue().
       */
      virtual const ContainerType& getAllValues() const
      {
        return mValue;
      }

      virtual void clearAllValues()
      {
        mValue.clear();
        mChangedValues.clear();
        fieldChanged(false);
      }

      /**
       * Returns size of field.
       */
      virtual int getSize()
      {
        return mValue.size();
      }

      /**
       * Returns if field is empty.
       */
      virtual bool isEmpty()
      {
        return mValue.empty();
      }

      /*
       * set1Value sets one value of the field vector. In order to avoid a trigger of fieldChanged()
       * each time one value is set during on frame, the caller can pass false as the last parameter.
       * The last set1Value within on frame should get true in order to issue the call of fieldChanged().
       * Or use the method triggerFieldChange().
       */
      virtual void set1Value(const Value& v, uint i, bool trigger_field_change = true)
      {
        set1Value(v, i, 0, trigger_field_change);
      }

      /**
       * Returns value at position i.
       */
      virtual const Value get1Value(uint i) const
      {
        return mValue.at(i);
      }

      /**
       * Inserts one value before given position. Caller can decide if this should trigger a fieldChanged().
       */
      virtual void insert1Value(const Value& v, uint before, bool trigger_field_change = true)
      {
        if (before > mValue.size())
          throw std::out_of_range("MultiValueField::insert1Value()");

        mValue.insert(mValue.begin() + before, v);
        mChangedValues.insert(mChangedValues.begin() + before, true);

        for (uint p = before + 1; p < mChangedValues.size(); p++)
          mChangedValues[p] = true;

        if (trigger_field_change)
          fieldChanged(false);
      }

      virtual void erase1Value(uint pos, bool trigger_field_change = true)
      {
        if (pos >= mValue.size())
          throw std::out_of_range("MultiValueField::erase1Value()");

        mValue.erase(mValue.begin() + pos);
        mChangedValues.erase(mChangedValues.begin() + pos);

        for (uint p = pos; p < mChangedValues.size(); p++)
          mChangedValues[p] = true;

        if (trigger_field_change)
          fieldChanged(false);
      }

      virtual void add1Value(const Value& v)
      {
        mValue.push_back(v);
        mChangedValues.push_back(true);
        fieldChanged(false);
      }

      virtual void remove1Value(const Value& v)
      {
        typename ContainerType::iterator f(std::find(mValue.begin(), mValue.end(), v));

        if (f != mValue.end())
        {
          uint pos = f - mValue.begin();
          mValue.erase(f);
          mChangedValues.erase(mChangedValues.begin() + pos);

          for (uint p = pos; p < mChangedValues.size(); p++)
            mChangedValues[p] = true;

          fieldChanged(false);
        }
      }

      /**
       * Used by setAllValues(const ContainerType&). Should probably be moved to the protected part of interface.
       */
      virtual void setAllValues(const ContainerType& v, Field* triggered_from)
      {
        mValue = v;
        mChangedValues = std::vector<bool>(v.size(), true);
        fieldChanged(false, triggered_from);
      }

      /**
       * Help function for set1Value(const Value&, uint, bool). Needed in the public part since the
       * template specializations of pullValueImpl() use a non-member help function that needs to call it.
       * Should appear in the protected part.
       */
      virtual void set1Value(const Value& v, uint i, Field* triggered_from, bool trigger_field_change = true)
      {
        mValue.at(i) = v;
        mChangedValues.at(i) = true;

        if (trigger_field_change)
          fieldChanged(false, triggered_from);
      }

      /**
       * Issues a fieldChanged() call. Call this method after you have set values with trigger_field_change
       * parameter set to false.
       */
      void triggerFieldChange(Field* triggered_from)
      {
        fieldChanged(false, triggered_from);
      }

      /**
       * Would prefer to have this in the protected part. Needed by non-member help functions.
       */
      virtual void resize(uint sz)
      {
        mValue.resize(sz);
        mChangedValues.resize(sz);
      }

      /*virtual*/ Field* clone(void) const
      {
        MultiValueField<Value>* cloned_field = new MultiValueField<Value>;
        cloned_field->mValue = mValue;
        cloned_field->mChangedValues = mChangedValues;
        return cloned_field;
      }

      /**
       * Read from stream, including change flags.
       */
      virtual void read(InputStream& is)
      {
        int count;

        is >> count;
        if (!is)
        {
          return;
        }

        ContainerType vec;
        for (int i=0; i<count; ++i)
        {
          Value val;
          is >> val;
          vec.push_back(val);
        }

        mValue = vec;

        std::vector<bool> bvec;
        for (int i=0; i<count; ++i)
        {
          bool bval;
          is >> bval;
          bvec.push_back(bval);
        }

        mChangedValues = bvec;

        fieldChanged(false, 0);
      }

      /**
       * Write to stream, including change flags.
       */
      virtual void write(OutputStream& os)
      {
        assert(mValue.size == mChangedValues.size());

  #if defined(__APPLE__)
        os.operator<<(static_cast< typename std::vector<Value>::size_type>(mValue.size()));
  #endif
  #if defined(_WIN32)
        // cl of VS 8 apparently not able to resolve os << mValue.size()
        os.operator<<(static_cast<std::vector<Value>::size_type>(mValue.size()));
  #endif
  #if defined(__linux__)
        os << mValue.size();
  #endif

        typename ContainerType::const_iterator i(mValue.begin());

        if (os.isBinaryEnabled())
        {
          while (i != mValue.end())
          {
            os << (*i);
            ++i;
          }
        }
        else
        {
          (std::ostream&) os << ' ';

          while (i != mValue.end())
          {
            os << (*i);
            (std::ostream&) os << ' ';
            ++i;
          }
        }

        std::vector<bool>::const_iterator b(mChangedValues.begin());

        if (os.isBinaryEnabled())
        {
          while (b != mChangedValues.end())
          {
            os << (*b);
            ++b;
          }
        }
        else
        {
          (std::ostream&) os << ' ';

          while (b != mChangedValues.end())
          {
            os << (*b);
            (std::ostream&) os << ' ';
            ++b;
          }
        }
      }

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
      virtual void push(Msg& msg);
      virtual void pop(Msg& msg);
#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)

      template <typename T> friend bool operator==(const MultiValueField<T>&, const MultiValueField<T>&);


    protected:

      /* virtual */ void pullValue(Field* fromField)
      {
        pullValueImpl(fromField);
      }

      std::vector<bool> mChangedValues;
      ContainerType mValue;

    private:

      void pullValueImpl(Field* fromField);

      static Type sClassTypeId;

    };

    template<typename Value>
    void av::utils::MultiValueField<Value>::pullValueImpl(av::Field* fromField)
    {
      if (!fromField->getTypeId().isOfType(MultiValueField<Value>::getClassTypeId()))
      {
        throw std::invalid_argument("pullValue: type mismatch. " +
                                    getTypeId().getName() +
                                    " <--> " +
                                    fromField->getTypeId().getName());
      }

      MultiValueField<Value> *fromMVField = dynamic_cast<MultiValueField<Value>*>(fromField);

      uint sz = fromMVField->getSize();
      resize(sz);
      uint last_changed_ind = sz;

      for (uint i = 0; i < sz; i++)
      {
        if (fromMVField->valueHasChanged(i))
        {
          set1Value(fromMVField->get1Value(i), i, fromField, false);
          last_changed_ind = i;
        }
      }

      if (last_changed_ind < sz)
        triggerFieldChange(fromField);
    }

    template<> AV_UTILS_DLL void av::utils::MultiValueField<double>::pullValueImpl(av::Field* fromField);
    template<> AV_UTILS_DLL void av::utils::MultiValueField<float>::pullValueImpl(av::Field* fromField);
    template<> AV_UTILS_DLL void av::utils::MultiValueField<int32_t>::pullValueImpl(av::Field* fromField);
    template<> AV_UTILS_DLL void av::utils::MultiValueField<int64_t>::pullValueImpl(av::Field* fromField);
    template<> AV_UTILS_DLL void av::utils::MultiValueField<uint32_t>::pullValueImpl(av::Field* fromField);
    template<> AV_UTILS_DLL void av::utils::MultiValueField<uint64_t>::pullValueImpl(av::Field* fromField);
    template<> AV_UTILS_DLL void av::utils::MultiValueField<bool>::pullValueImpl(av::Field* fromField);

    template <typename Value> bool
    operator==(const MultiValueField<Value>& a, const MultiValueField<Value>& b)
    {
      return (a.getAllValues() == b.getAllValues()) && (a.mChangedValues == b.mChangedValues);
    }

    /**
     * standard type fields
     */
  #ifdef AV_INSTANTIATE_FIELD_TEMPLATES
    template class AV_UTILS_DLL MultiValueField<bool>;
    template class AV_UTILS_DLL MultiValueField<double>;
    template class AV_UTILS_DLL MultiValueField<float>;
    template class AV_UTILS_DLL MultiValueField<int32_t>;
    template class AV_UTILS_DLL MultiValueField<int64_t>;
    template class AV_UTILS_DLL MultiValueField<uint32_t>;
    template class AV_UTILS_DLL MultiValueField<uint64_t>;
    template class AV_UTILS_DLL MultiValueField<std::string>;
  #endif

    typedef MultiValueField<bool>               MVFBool;
    typedef MultiValueField<double>             MVFDouble;
    typedef MultiValueField<float>              MVFFloat;
    typedef MultiValueField<int32_t>             MVFInt;
    typedef MultiValueField<int64_t>             MVFLong;
    typedef MultiValueField<uint32_t>            MVFUInt;
    typedef MultiValueField<uint64_t>            MVFULong;
    typedef MultiValueField<std::string>         MVFString;

    /*
     * OSG type fields. Should be moved to av::osg::Field.h
     * as soon as this class is factored into avango-core (if ever)
     */
#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_UTILS_DLL MultiValueField< ::osg::Matrixf>;
  template class AV_UTILS_DLL MultiValueField< ::osg::Matrixd>;
  template class AV_UTILS_DLL MultiValueField< ::osg::Vec2f>;
  template class AV_UTILS_DLL MultiValueField< ::osg::Vec2d>;
  template class AV_UTILS_DLL MultiValueField< ::osg::Vec3f>;
  template class AV_UTILS_DLL MultiValueField< ::osg::Vec3d>;
  template class AV_UTILS_DLL MultiValueField< ::osg::Vec4f>;
  template class AV_UTILS_DLL MultiValueField< ::osg::Vec4d>;
  template class AV_UTILS_DLL MultiValueField< ::osg::Quat>;
#endif

    typedef MultiValueField< ::osg::Matrixf> MVFMatrixf;
    typedef MultiValueField< ::osg::Matrixd> MVFMatrixd;
    typedef MultiValueField< ::osg::Matrix> MVFMatrix;
    typedef MultiValueField< ::osg::Vec2f> MVFVec2f;
    typedef MultiValueField< ::osg::Vec2d> MVFVec2d;
    typedef MultiValueField< ::osg::Vec2> MVFVec2;
    typedef MultiValueField< ::osg::Vec3f> MVFVec3f;
    typedef MultiValueField< ::osg::Vec3d> MVFVec3d;
    typedef MultiValueField< ::osg::Vec3> MVFVec3;
    typedef MultiValueField< ::osg::Vec4f> MVFVec4f;
    typedef MultiValueField< ::osg::Vec4d> MVFVec4d;
    typedef MultiValueField< ::osg::Vec4> MVFVec4;
    typedef MultiValueField< ::osg::Quat> MVFQuat;

    void initMultiValueFields();
    void initMultiValueOSGFields();


#if defined(AVANGO_DISTRIBUTION_SUPPORT)

    template<typename Value> inline void
    MultiValueField<Value>::push(Msg& msg)
    {
      AV_ASSERT(mValue.size() == mChangedValues.size());
      typename ContainerType::const_iterator i(mValue.begin());

      while (i != mValue.end())
      {
        av_pushMsg(msg, *i);
        ++i;
      }

      std::vector<bool>::iterator b(mChangedValues.begin());

      while (b != mChangedValues.end())
      {
        av_pushMsg(msg, *b);
        ++b;
      }

      av_pushMsg(msg, mValue.size());
    }

    template<typename Value> inline void
    MultiValueField<Value>::pop(Msg& msg)
    {
      int count;

      av_popMsg(msg, count);
      mChangedValues.resize(count);

      bool bval;

      for (int b = count; b > 0; --b)
      {
        av_popMsg(msg, bval);
        mChangedValues[b-1] = bval;
      }

      mValue.resize(count);
      Value val;

      for (int i = count; i > 0; --i)
      {
        av_popMsg(msg, val);
        mValue[i-1] = val;
      }

      fieldChanged(true);
    }

    template<> void MultiValueField<float>::push(Msg&);
    template<> void MultiValueField<float>::pop(Msg&);
    template<> void MultiValueField<double>::push(Msg&);
    template<> void MultiValueField<double>::pop(Msg&);
    template<> void MultiValueField<int32_t>::push(Msg&);
    template<> void MultiValueField<int32_t>::pop(Msg&);
    template<> void MultiValueField<uint32_t>::push(Msg&);
    template<> void MultiValueField<uint32_t>::pop(Msg&);
    template<> void MultiValueField<int64_t>::push(Msg&);
    template<> void MultiValueField<int64_t>::pop(Msg&);
    template<> void MultiValueField<uint64_t>::push(Msg&);
    template<> void MultiValueField<uint64_t>::pop(Msg&);

#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)


  }
}


#endif // #if !defined(AVANGO_UTILS_MULTIVALUEFIELD_H)
