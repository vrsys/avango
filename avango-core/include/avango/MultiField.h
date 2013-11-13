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

#if !defined(AVANGO_MULTIFIELD_H)
#define AVANGO_MULTIFIELD_H

#include <vector>
#include <stdexcept>

#include <boost/function.hpp>

#include <avango/Config.h>
#include <avango/Create.h>
#include <avango/Field.h>
#include <avango/InputStream.h>
#include <avango/OutputStream.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#  include <avango/Msg.h>
#endif

namespace av
{

  /**
   * Multi value holder to be put into a FieldContainer
   */
  template <typename Value>
  class MultiField : public Field
  {

  public:

    typedef Field              SuperType;
    typedef std::vector<Value> ContainerType;
    typedef Value              ValueType;

    /**
     * Event class for field getValue events.
     */
    class  GetValueEvent : public Event
    {
      friend class MultiField;

    public:

      /**
       * Get the pointer to write the new value to.
       * Don't access the pointer outside the callback.
       */
      ContainerType* getValuePtr() const { return mValuePtr; }

    protected:
      GetValueEvent(const Field *field, ContainerType* value_ptr) :
        Event(field), mValuePtr(value_ptr) {}

    private:
      ContainerType* mValuePtr;
    };

    /**
     * Event class for field setValue events.
     */
    class  SetValueEvent : public Event
    {
      friend class MultiField;

    public:

      /**
       * Get the new value.
       * A reference to the new value is returned to avoid copying.
       * Don't access it outside the callback.
       */
      const ContainerType& getValue() const { return mValue; }

    protected:
      SetValueEvent(Field *field, const ContainerType& value) :
        Event(field), mValue(value) {}

    private:
      const ContainerType& mValue;
    };

    typedef boost::function<void (const GetValueEvent&)> GetValueCallback;
    typedef boost::function<void (const SetValueEvent&)> SetValueCallback;

    MultiField() :
      Field(),
      mHasGetValueCallback(false),
      mHasSetValueCallback(false)
    {}

    MultiField(const MultiField&) : Field()
    {}

    virtual ~MultiField()
    {}

    static void initClass(const std::string& classname, const std::string& parentname)
    {
      if (Type::badType() == sClassTypeId) {
        sClassTypeId = Field::registerType(classname, parentname,
                                           new CreateA<MultiField<Value> >());
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
              const ContainerType& init)
    {
      addGetValueCallback(get_cb);
      addSetValueCallback(set_cb);

      bind(container, name, owned, init);
    }

    virtual void setValue(const ContainerType& v)
    {
      setValue(v, 0);
    }

    virtual const ContainerType& getValue() const
    {
      if (mHasGetValueCallback)
        mGetValueCallback(GetValueEvent(this, &mValue));
      return mValue;
    }

    virtual void add1Value(const Value& v)
    {
      getValue();
      mValue.push_back(v);
      if (mHasSetValueCallback)
        mSetValueCallback(SetValueEvent(this, mValue));
      fieldChanged(false);
    }

    virtual void remove1Value(const Value& v)
    {
      getValue();

      typename ContainerType::iterator f(std::find(mValue.begin(), mValue.end(), v));

      if (f != mValue.end())
      {
        mValue.erase(f);
        if (mHasSetValueCallback)
          mSetValueCallback(SetValueEvent(this, mValue));
        fieldChanged(false);
      }
    }

    virtual bool has1Value(const Value& v)
    {
      getValue();
      return (std::find(mValue.begin(), mValue.end(), v) != mValue.end());
    }

    /* virtual */ void clear()
    {
      mValue.clear();
      if (mHasSetValueCallback)
        mSetValueCallback(SetValueEvent(this, mValue));
      fieldChanged(false);
    }

    virtual int getSize()
    {
      return static_cast<int>(getValue().size());
    }

    virtual bool isEmpty()
    {
      return getValue().empty();
    }

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
      setValue(vec);
    }

    virtual void write(OutputStream& os)
    {
      getValue();

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
    }

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
     virtual void push(Msg& msg);
     virtual void pop(Msg& msg);
#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)

    template <typename T> friend bool operator==(const MultiField<T>&, const MultiField<T>&);

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
      MultiField<Value>* cloned_field = new MultiField<Value>;
      cloned_field->mValue = mValue;
      return cloned_field;
    }

  protected:

    virtual void setValue(const ContainerType& v, Field* triggered_from)
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

    mutable ContainerType mValue;

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

  template<typename Value>
  void av::MultiField<Value>::pullValueImpl(av::Field* fromField)
  {
    if (!fromField->getTypeId().isOfType(MultiField<Value>::getClassTypeId()))
    {
      throw std::invalid_argument("pullValue: type mismatch. " +
                                  getTypeId().getName() +
                                  " <--> " +
                                  fromField->getTypeId().getName());
    }
    setValue(dynamic_cast<MultiField<Value>*>(fromField)->getValue(), fromField);
  }

  template<> AV_DLL void av::MultiField<double>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::MultiField<float>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::MultiField<int32_t>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::MultiField<int64_t>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::MultiField<uint32_t>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::MultiField<uint64_t>::pullValueImpl(av::Field* fromField);
  template<> AV_DLL void av::MultiField<bool>::pullValueImpl(av::Field* fromField);

  template <typename Value> bool
  operator==(const MultiField<Value>& a, const MultiField<Value>& b)
  {
    return a.getValue() == b.getValue();
  }

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

  template<typename Value> inline void
  MultiField<Value>::push(Msg& msg)
  {
    getValue();

    typename ContainerType::const_iterator i(mValue.begin());

    while (i != mValue.end())
    {
      av_pushMsg(msg, *i);
      ++i;
    }

    av_pushMsg(msg, static_cast<uint32_t>(mValue.size()));
  }

  template<typename Value> inline void
  MultiField<Value>::pop(Msg& msg)
  {
    uint32_t count;

    av_popMsg(msg, count);

    ContainerType swapVec(count);

    mValue.swap(swapVec);

    Value val;

    for (uint32_t i = count; i > 0; --i)
    {
      av_popMsg(msg, val);
      mValue[i-1] = val;
    }

    if (mHasSetValueCallback)
      mSetValueCallback(SetValueEvent(this, mValue));
    fieldChanged(true);
  }

  template<> void MultiField<float>::push(Msg&);
  template<> void MultiField<float>::pop(Msg&);
  template<> void MultiField<double>::push(Msg&);
  template<> void MultiField<double>::pop(Msg&);
  template<> void MultiField<int32_t>::push(Msg&);
  template<> void MultiField<int32_t>::pop(Msg&);
  template<> void MultiField<uint32_t>::push(Msg&);
  template<> void MultiField<uint32_t>::pop(Msg&);
  template<> void MultiField<int64_t>::push(Msg&);
  template<> void MultiField<int64_t>::pop(Msg&);
  template<> void MultiField<uint64_t>::push(Msg&);
  template<> void MultiField<uint64_t>::pop(Msg&);

#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)

}

#endif // #if !defined(AVANGO_MULTIFIELD_H)
