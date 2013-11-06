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

#if !defined(AVANGO_SINGLEFIELD_H)
#define AVANGO_SINGLEFIELD_H

#include <stdexcept>

#include <boost/function.hpp>

#include <avango/Assert.h>
#include <avango/Config.h>
#include <avango/Create.h>
#include <avango/Field.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#  include <avango/Msg.h>
#endif

namespace av
{

  /**
   * Single value holder to be put into a FieldContainer
   */
  template <typename Value>
  class SingleField : public Field
  {

    /**
     * The only purpose of this method is to validate the requirements
     * SingleField puts on the underlying value type.
     * Having a volatile guard set to false hopefully forces the compiler
     * to generate but never execute the included code.
     */
    bool validateRequirements() const
    {
      volatile bool guard(false);

      if (guard)
      {
        Value v;
        OutputStream os; os << v;
        InputStream is; is >> v;

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
        Msg msg;
        av_pushMsg(msg, v);
        av_popMsg(msg, v);
#endif
      }
      return true;
    }

  public:

    typedef Field SuperType;
    typedef Value ValueType;

    /**
     * Event class for field getValue events.
     */
    class GetValueEvent : public Event
    {
      friend class SingleField;

    public:

      /**
       * Get the pointer to write the new value to.
       * Don't access the pointer outside the callback.
       */
      ValueType* getValuePtr() const { return mValuePtr; }

    protected:
      GetValueEvent(const Field *field, ValueType* value_ptr) :
        Event(field), mValuePtr(value_ptr) {}

    private:
      ValueType* mValuePtr;
    };

    /**
     * Event class for field setValue events.
     */
    class SetValueEvent : public Event
    {
      friend class SingleField;

    public:

      /**
       * Get the new value.
       * A reference to the new value is returned to avoid copying.
       * Don't access it outside the callback.
       */
      const ValueType& getValue() const { return mValue; }

    protected:
      SetValueEvent(Field *field, const ValueType& value) :
        Event(field), mValue(value) {}

    private:
      const ValueType& mValue;
    };

    typedef boost::function<void (const GetValueEvent&)> GetValueCallback;
    typedef boost::function<void (const SetValueEvent&)> SetValueCallback;

    SingleField() :
      Field(),
      mValue(Value()),
      mHasGetValueCallback(false),
      mHasSetValueCallback(false)
    {
      AV_ASSERT(validateRequirements());
    }

    SingleField(const SingleField&): Field()
    {
      AV_ASSERT(validateRequirements());
    }

    virtual ~SingleField()
    {}

    static void initClass(const std::string& classname, const std::string& parentname)
    {
      if (Type::badType() == sClassTypeId)
      {
        sClassTypeId = Field::registerType(classname, parentname,
                                           new CreateA<SingleField<Value> >);
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

    void bind(FieldContainer* container, const std::string& name, bool owned, const Value& init)
    {
      Field::bind(container, name, owned);
      mValue = init;
      if (mHasSetValueCallback)
        mSetValueCallback(SetValueEvent(this, mValue));
    }

    void bind(FieldContainer* container, const std::string& name, bool owned,
              const GetValueCallback& get_cb, const SetValueCallback& set_cb)
    {
      if (!isBound())
      {
        SuperType::bind(container, name, owned);
      }

      addGetValueCallback(get_cb);
      addSetValueCallback(set_cb);

      getValue();
    }

    void bind(FieldContainer* container, const std::string& name, bool owned,
              const GetValueCallback& get_cb, const SetValueCallback& set_cb,
              const Value& init)
    {
      addGetValueCallback(get_cb);
      addSetValueCallback(set_cb);

      bind(container, name, owned, init);
    }

    virtual void setValue(const Value& v)
    {
      setValue(v, 0);
    }

    virtual const Value& getValue() const
    {
      if (mHasGetValueCallback)
        mGetValueCallback(GetValueEvent(this, &mValue));
      return mValue;
    }

    /* virtual */ void clear()
    {
      setValue(Value());
    }

    virtual void read(InputStream& is)
    {
      Value val;
      is >> val;
      setValue(val);
    }

    virtual void write(OutputStream& os)
    {
#if !defined(_WIN32)
      os << getValue();
#else
      // cl of VS 8 apparently not able to resolve os << getValue().
      Value val;
      val = getValue();
      av::operator<<( static_cast<av::OutputStream&>(os), static_cast< Value >( val ));
#endif
    }

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
    virtual void push(Msg& msg)
    {
      getValue();
      av_pushMsg(msg, mValue);
    }

    virtual void pop(Msg& msg)
    {
      av_popMsg(msg, mValue);
      if (mHasSetValueCallback)
        mSetValueCallback(SetValueEvent(this, mValue));
      fieldChanged(true);
    }
#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)

    template <typename T> friend bool operator==(const SingleField<T>&, const SingleField<T>&);

    /**
     * Register callback invoked for a getValue called on this field.
     * The callback must take exactly one parameter of GetValueEvent.
     */
    void addGetValueCallback(const GetValueCallback& callback)
    {
      mHasGetValueCallback = true;
      mGetValueCallback = callback;
    }

    /**
     * Remove previously registered field GetValueCallback via its handle.
     */
    void removeGetValueCallback(void)
    {
      mHasGetValueCallback = false;
      mGetValueCallback = 0;
    }

    /**
     * Register callback invoked for a setValue called on this field.
     * The callback must take exactly one parameter of SetValueEvent.
     */
    void addSetValueCallback(const SetValueCallback& callback)
    {
      mHasSetValueCallback = true;
      mSetValueCallback = callback;
    }

    /**
     * Remove previously registered field SetValueCallback via its handle.
     */
    void removeSetValueCallback(void)
    {
      mHasSetValueCallback = false;
      mSetValueCallback = 0;
    }

    /*virtual*/ Field* clone(void) const
    {
      SingleField<Value>* cloned_field = new SingleField<Value>;
      cloned_field->mValue = mValue;
      return cloned_field;
    }

  protected:

    virtual void setValue(const Value& v, Field* triggered_from)
    {
      mValue = v;
      if (mHasSetValueCallback)
        mSetValueCallback(SetValueEvent(this, mValue));
      fieldChanged(false, triggered_from);
    }

    virtual void pullValue(Field* fromField)
    {
      pullValueImpl(fromField);
    }

    mutable Value mValue;

  private:

    void pullValueImpl(Field* fromField);

    static Type sClassTypeId;

    GetValueCallback mGetValueCallback;
    SetValueCallback mSetValueCallback;

    // Even though boost::function provides an empty() method, we track this
    // status ourselves as it is quite a bit faster and speed is quite
    // important here.
    bool mHasGetValueCallback;
    bool mHasSetValueCallback;
  };

  template <typename Value> inline bool
  operator==(const SingleField<Value>& a, const SingleField<Value>& b)
  {
    return a.getValue() == b.getValue();
  }

  template<typename Value>
  void av::SingleField<Value>::pullValueImpl(av::Field* fromField)
  {
    if (!fromField->getTypeId().isOfType(SingleField<Value>::getClassTypeId()))
    {
      throw std::invalid_argument("pullValue: type mismatch. " +
                                  getTypeId().getName() +
                                  " <--> " +
                                  fromField->getTypeId().getName());
    }
    setValue(dynamic_cast<SingleField<Value>*>(fromField)->getValue(), fromField);
  }

  template<> AV_DLL void av::SingleField<double>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::SingleField<float>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::SingleField<int32_t>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::SingleField<int64_t>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::SingleField<uint32_t>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::SingleField<uint64_t>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::SingleField<bool>::pullValueImpl(av::Field* fromField);
}

#endif // #if !defined(AVANGO_SINGLEFIELD_H)
