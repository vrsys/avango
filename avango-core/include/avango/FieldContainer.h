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

#if !defined(AVANGO_FIELDCONTAINER_H)
#define AVANGO_FIELDCONTAINER_H

/**
 * \file
 * \ingroup av
 */

#include <avango/Config.h>
#include <avango/Distributed.h>
#include <avango/StandardFields.h>

#ifdef __GNUC__ // GNU C++ stores TR1 headers differently
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

namespace av
{
  class InputStream;
  class OutputStream;
  class Field;
  class FieldRef;

  /**
   * The FieldContainer class is the base class for all classes which
   * contain fields (see Field) to encapsulate object attributes.
   * It provides methods to query number, name, type and value of any field
   * it contains.
   *
   * It provides a complex macro package which should be used by
   * derived classes in order for runtime typing, reference counting,
   * field declaration and inheritance to work.
   *
   * \ingroup av
   */
  class AV_DLL FieldContainer : public Distributed
  {
    friend class Field;

    AV_BASE_DECLARE_ABSTRACT();

  public:

    typedef long int IDType;

    /**
     * Constructor
     */
    FieldContainer();

  protected:

    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~FieldContainer();

  public:

    /**
     * Name of the field container.
     */
    SFString Name;

    /**
     * Returns the number of fields in this field container.
     */
    unsigned int getNumFields();

    /**
     * Returns the number of fields in this field container which will be written to streams.
     */
    unsigned int getNumWriteableFields();

    /**
     * Disconnects all Fields contained in this FieldContainer.
     */
    void disconnectAllFields();

    /**
     * Dynamically adds a field by type name to this field container.
     * Adding a new field with a name of an already existing field is prohibited, and
     * is checked with an assertion.
     */
    virtual void addDynamicField(const std::string& typeName, const std::string& fieldName);

    /**
     * Dynamically adds a copy of a field to this field container.
     * Adding a new field with a name of an already existing field is prohibited, and
     * is checked with an assertion.
     */
    virtual void addDynamicField(Field* field, const std::string& fieldName);

    /**
     * Remove a field from the field container.
     * CAUTION: If you use this functionality
     * 1) The avango distribution will not work correctly.
     * 2) The id of the fields will change. Therefore code which stores and resembles
     *    the id of fields may and most propably will not work correctly.
     */
    virtual Field* removeDynamicField(const std::string& fieldName);

    /**
     * Returns true if there is a field named \e name.
     */
    virtual bool hasField(const std::string& name);

    /**
     * Returns a pointer to the field with index \e index.
     * If there is no such field, a null pointer is returned.
     */
    virtual Field* getField(unsigned int index);

    /**
     * Returns a pointer to the field named \e name.
     * If there is no such field, a null pointer is returned.
     */
    virtual Field* getField(const std::string& name);

    /**
     * Returns the name of the field with index \e index.
     * If there is no such field, an empty string is returned.
     */
    virtual const std::string& getFieldName(unsigned int index);

    /**
     * Returns a vector of pointers to all fields in this container.
     */
    virtual std::vector<Field*>& getFields();

    /**
     * Returns the identifier. The return value is unique for this
     * container for the currently running instance of Avango.
     */
    IDType getId() const;

    /**
     * Returns a fp_field_ref object that identifies a fields as field container
     * pointer and field index.
     */
    virtual bool getFieldRef(const std::string& name, FieldRef& fr);

    /**
     * Mark this field container as touched. This has the same effect
     * as if one of the fields had been touched.
     */
    virtual void touch(bool from_net = false);

    /**
     * Enable or disable notification on this node. If disabled, changes to
     * fields or calls to touch() are not propagated to connected fields.
     * This calls enableNotify on all fields.
     */
    void enableNotify(bool on);

    /** Enable or disable scheduling on this node. If enabled, the node can be
     * scheduled to be evaluated automatically.
     */
    void allowScheduling(bool on);

    /**
     * Set/unset an field-container to be evaluated every frame.
     */
    void alwaysEvaluate(bool on);

    /**
     * Evaluates another FieldContainer, if own evaluation depends on it,
     * but there is no field connection to it.
     * May only be called from evaluate or evaluteLocalSideEffect.
     */
    void evaluateDependency(FieldContainer& dependency);

    /**
     * Starts evaluation of all field containers.
     */
    static void evaluateAllContainers();

    /**
     * Remove all containers from evaluation schedule.
     * This function is primarily useful for cleanup at application exit
     * and should not be called during normal operation.
     */
    static void unscheduleEvaluationForAllContainers();

    /**
     * Get time of last change.
     */
    double lastChange() const;

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
    virtual void push(Msg&);
    virtual void pop(Msg&);
#endif

    /**
     * Read container representation from InputStream.
     */
    virtual void read(InputStream& is);

    /**
     * Write container representation to OutputStream.
     */
    virtual void write(OutputStream& os);

  protected:

    /**
     * Name adaptor field getter callback
     */
    virtual void getNameCB(const av::SFString::GetValueEvent& event);

    /**
     * Name adaptor field setter callback
     */
    virtual void setNameCB(const av::SFString::SetValueEvent& event);

    /**
     * Schedule this field container for evaluation (if needed).
     */
    void scheduleForEvaluation();

    /**
     * Time of last change (used for synchronisation with graph display).
     */
    double mLastChange;

    // you may overload these, if you use this as a base class

    /**
     * This function is called whenever one of the fields has been changed. You want to
     * overload this function, if you want your node to respond to field changes in a
     * non standard way. evaluate() is called only once per simulation frame, even
     * if multiple fields have been changed.
     */
    virtual void evaluate();
    virtual void evaluateLocalSideEffect();

    /**
     * This function is called immediately after \e field has been changed. In a distributed
     * setup you must not set or touch any other fields on this from within this function.
     * This can savely be done in the evaluate() callback.
     */
    virtual void fieldHasChanged(const Field& field);
    virtual void fieldHasChangedLocalSideEffect(const Field& field);

    /**
     * Returns \c true if a field of this container has changed in this frame.
     */
    bool needsEvaluation() const;

    void callEvaluate();
    void fieldChanged(const Field& field, bool from_net = false);

    struct
    {
      bool mNeedsEvaluation:1;
      bool mFieldsCalculated:1;
      bool mAlwaysEvaluate:1;
      bool mIsEvaluating:1;
      bool mAllowScheduling:1;
    } mFlags;

  private:

    typedef std::vector<Field*> FieldPtrVec;
    FieldPtrVec mFieldPtrs;     // for fast lookup in getFields, do not use directly !

    struct FieldInfo
    {
      FieldInfo() : mField(0) {}
      Field*      mField;
      std::string mName;
    };

    typedef std::vector<FieldInfo> FieldInfos;
    FieldInfos mFields;

    typedef std::tr1::unordered_map<std::string, FieldInfos::size_type> FieldsIndex;
    FieldsIndex mFieldsIndex;

    FieldInfo* getFieldInfo(const std::string& name);
    FieldInfo* getFieldInfo(unsigned int index);

    unsigned int addField(Field* field, const std::string& fieldName);
    Field* removeField(unsigned int index);
    IDType mId;

    unsigned int mEvaluateId;
    static unsigned int sEvaluateId;

  };

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_DLL SingleField<Link<FieldContainer> >;
  template class AV_DLL MultiField<Link<FieldContainer> >;
#endif

  typedef SingleField<Link<FieldContainer> > SFContainer;
  typedef MultiField<Link<FieldContainer> >  MFContainer;

}

#define AV_FC_DECLARE() \
  AV_BASE_DECLARE()

#define AV_FC_DECLARE_ABSTRACT()                \
  AV_BASE_DECLARE_ABSTRACT()

#define AV_FC_DEFINE_ABSTRACT(thisClass)        \
  AV_BASE_DEFINE_ABSTRACT(thisClass)

#define AV_FC_DEFINE(thisClass)                 \
  AV_BASE_DEFINE(thisClass)

#define AV_FC_INIT_ABSTRACT(parentClass, thisClass, isPublic)   \
  AV_BASE_INIT_ABSTRACT(parentClass, thisClass, isPublic)

#define AV_FC_INIT(parentClass, thisClass, isPublic)    \
  AV_BASE_INIT(parentClass, thisClass, isPublic)

#define AV_FC_ADD_FIELD(fieldName, initialValue)        \
  fieldName.bind(this, #fieldName, false, initialValue)

#define AV_FC_ADD_ADAPTOR_FIELD(fieldName, getcb, setcb)        \
  fieldName.bind(this, #fieldName, false, getcb, setcb)

#endif // #if !defined(AVANGO_FIELDCONTAINER_H)
