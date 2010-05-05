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

#if !defined(AVANGO_OBJECTVALUE_H)
#define AVANGO_OBJECTVALUE_H

#include <string>

#include <avango/Assert.h>
#include <avango/FieldContainer.h>

namespace av
{
  /**
   * The ObjectValue template encapsulates a single value in an AVANGO field
   * container.
   */
  template <typename T> class ObjectValue : public FieldContainer
  {

  public:

    typedef T ValueType;

    SingleField<ValueType> Value;

    ObjectValue()
    {
      AV_FC_ADD_FIELD(Value, ValueType());
    }

  protected:

    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~ObjectValue()
    {}

  public:

    static void initClass()
    {
      if (!isTypeInitialized())
      {
        FieldContainer::initClass();

        sClassTypeId = Type::createType(FieldContainer::getClassTypeId(),
                                        sClassTypeName,
                                        new CreateA<ObjectValue<T> >,
                                        true);

        sClassTypeId.setDistributable(true);
      }
    }

    static Type getClassTypeId()
    {
      AV_ASSERT(Type::badType() != sClassTypeId);
      return sClassTypeId;
    }

    static bool isTypeInitialized()
    {
      return Type::badType() != sClassTypeId;
    }

    Type getTypeId() const
    {
      AV_ASSERT(Type::badType() != sClassTypeId);
      return sClassTypeId;
    }

  private:

    static Type sClassTypeId;
    static const std::string sClassTypeName; // initialized in specialization

  };

  template <typename T> Type ObjectValue<T>::sClassTypeId = Type::badType();

  template class AV_DLL ObjectValue<bool>;
  template class AV_DLL ObjectValue<int>;
  template class AV_DLL ObjectValue<unsigned int>;
  template class AV_DLL ObjectValue<float>;
  template class AV_DLL ObjectValue<double>;
  template class AV_DLL ObjectValue<Link<FieldContainer> >;
  template class AV_DLL ObjectValue<std::string>;

  typedef ObjectValue<bool>                   BoolObject;
  typedef ObjectValue<int>                    IntObject;
  typedef ObjectValue<unsigned int>           UIntObject;
  typedef ObjectValue<float>                  FloatObject;
  typedef ObjectValue<double>                 DoubleObject;
  typedef ObjectValue<Link<FieldContainer> >  ObjectObject;
  typedef ObjectValue<std::string>            StringObject;
}

#endif // #if !defined(AVANGO_OBJECTVALUE_H)
