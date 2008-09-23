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

#if !defined(AVANGO_FIELD_H)
#define AVANGO_FIELD_H

#include <list>
#include <set>
#include <vector>
#include <utility>

#include <boost/signal.hpp>

#include <avango/Config.h>
#include <avango/Typed.h>

namespace av
{

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
  class Msg;
#endif

  class Create;
  class FieldContainer;
  class InputStream;
  class OutputStream;

  /**
   * Base class for all field types.
   * Fields provide a generic scripting and streaming interface for
   * accessing object state attributes.
   *
   * \ingroup av
   */
  class AV_DLL Field : public Typed
  {

    friend class FieldContainer;

    AV_TYPED_DECLARE_ABSTRACT();

  public:

    /**
     * Field event base class passed to functions attached to field signals.
     */
    class Event
    {
      friend class Field;

    public:

      /**
       * Destructor
       */
      virtual ~Event() {}

      /**
       * The originating field of the event.
       *
       * The field gets invalid when its container is destroyed.
       * So, you should reference the container if you are planning to hold
       * the field or a copy of the event after the callback has returned.
       * You can do this by storing the container in a Link (preferred) or
       * by calling reference/dereference explicitly.
       */
      const Field* getField() const { return mField; }

    protected:
      Event(const Field *field) : mField(field) {}

    private:
      const Field *mField;
    };

    /**
     * Event class for field change events.
     */
    class ChangedEvent : public Event
    {
      friend class Field;

    public:

      /**
       * Was the field change triggered from network or locally?
       */
      bool getChangedFromNet() const { return mChangedFromNet; }

      /**
       * Returns the connected field which triggered this field change.
       * Returns 0 if the field was not changed by a connection.
       * \see FieldEvent::getField for notes about holding the field after
       * the callback has returned.
       */
      Field* getTriggeredFromField() const { return mTriggeredFromField; }

    protected:
      ChangedEvent(Field *field, bool changedFromNet, Field* triggeredFromField) :
        Event(field), mChangedFromNet(changedFromNet), mTriggeredFromField(triggeredFromField) {}

    private:
      bool mChangedFromNet;
      Field* mTriggeredFromField;
    };

    /**
     * Event class for field connect events.
     */
    class ConnectedEvent : public Event
    {
      friend class Field;

    public:

      /**
       * Get the field just connected to the originating field of the event.
       * \see FieldEvent::getField for notes about holding the field after
       * the callback has returned.
       */
      Field* getConnectedField() const { return mConnectedField; }

    protected:
      ConnectedEvent(Field *field, Field *connectedField) :
        Event(field), mConnectedField(connectedField) {}

    private:
      Field* mConnectedField;
    };

    /**
     * Event class for field disconnect events.
     */
    class DisconnectedEvent : public Event
    {
      friend class Field;

    public:

      /**
       * Get the fields just disconnected to the originating field of the event.
       * \see FieldEvent::getField for notes about holding a field after
       * the callback has returned.
       */
      std::vector<Field*> getDisconnectedFields() const { return mDisconnectedFields; }

    protected:
      DisconnectedEvent(Field *field, std::vector<Field*> disconnectedFields) :
        Event(field), mDisconnectedFields(disconnectedFields) {}

    private:
      std::vector<Field*> mDisconnectedFields;
    };


    typedef boost::signal<void (const ChangedEvent&)> ChangedSignal;
    typedef ChangedSignal::slot_type ChangedCallback;
    typedef boost::signals::connection ChangedCallbackHandle;

    typedef boost::signal<void (const ConnectedEvent&)> ConnectedSignal;
    typedef ConnectedSignal::slot_type ConnectedCallback;
    typedef boost::signals::connection ConnectedCallbackHandle;

    typedef boost::signal<void (const DisconnectedEvent&)> DisconnectedSignal;
    typedef DisconnectedSignal::slot_type DisconnectedCallback;
    typedef boost::signals::connection DisconnectedCallbackHandle;

    enum FieldChangeSource
    {
      script,
      connection,
      tune,
      tune_n,
      untune,
      activate,
      deactivate,
      apply,
      unapply,
      net,
      internal
    };

    typedef std::set<Field*>    FieldPtrSet;
    typedef std::vector<Field*> FieldPtrVec;

    Field();
    virtual ~Field();

    /**
     * Returns a pointer to the container this field is contained in.
     */
    FieldContainer* getContainer() const;

    /**
     * Returns \c true if the field is bound to a field container.
     */
    bool isBound() const;

    /**
     * Returns the index of this field. The index can be used on FieldContainer methods
     * (e.g. FieldContainer::getField())
     */
    unsigned int getIndex() const;

    /**
     * Returns the name of this field.
     */
    const std::string& getName() const;

    /**
     * Connects \c field to this field. A field can have multiple connections
     * \em from other fields if in multi-input mode (see setMultiInput()) and can be connected
     * \em to any number of fields.
     *
     * A field connected from another field has the same value as that field . Exceptions:
     * - notify (see enableNotify()) for the source field had been disabled when its value has
     *   changed the last time
     * - the source field's value is only propagated once per frame if single-pushed
     *   (see setMultiPush())
     *
     * The dependent flag is used to signal that the connection is used to express a dependency
     * of the FieldContainer this field belongs to on the FieldContainer field that field belongs
     * to.
     */
    void connectFrom(Field* field, bool dependent = true);

    /**
     * Remove the connection from all connected fields.
     */
    void disconnect();

    /**
     * Remove the connection from a specific field.
     * Useful if the field is multi-input enabled (see setMultiInput()).
     */
    void disconnectFrom(Field* from);

    /**
     * Returns a const reference to the set of field auditors.
     */
    const FieldPtrSet& getAuditors() const;

    /**
     * Remove all connections to other fields.
     */
    void disconnectAuditors();

    /**
     * Returns the field with index \c index (default: 0) this field is connected from.
     * If there is no connection with this index the null pointer is returned.
     */
    Field* getConnectedField(unsigned int index = 0);

    /**
     * Returns the number of fields which are connected to this field.
     */
    unsigned int getNumberOfConnectedFields() const;

    /**
     * Enable or disable dependency on this field. If enabled, the field container is dependent
     * on other field container, which have field connected to this field.
     */
    void enableDependency(bool on);

    /**
     * Returns \c true  if dependency is enabled for this field.
     */
    bool dependencyEnabled();

    /**
     * Enable or disable notification on this field. If enabled, changes to fields or calls to
     * touch() are propagated to all connected fields.
     */
    void enableNotify(bool on);

    /**
     * Returns \c true  if notification is enabled for this field.
     */
    bool notifyEnabled();

    /**
     * Simulates a change to the fields value with respect to the notification mechanism.
     */
    void touch();

    /**
     * Bind field with name \c name to \c container.
     * This function shouldn't be invoked directly. It is called by the AV_FC_INIT macros.
     */
    void bind(FieldContainer* container, const std::string& name, bool owned);

    /**
     * Sets the field to a type dependent default value.
     */
    virtual void clear();

    /**
     * Reads the field from an InputStream
     */
    virtual void read(InputStream& is) = 0;

    /**
     * Writes the field to an OutputStream
     */
    virtual void write(OutputStream& os) = 0;

    /**
     * Reads field connection info from an InputStream
     */
    void readConnection(InputStream& is);

    /**
     * Writes field connection info to an OutputStream
     */
    void writeConnection(OutputStream& os);

    /**
     * Dont't ever write this field out.
     */
    void dontWrite(bool on);

    /**
     * Is this a writeable field?
     */
    bool isWritable() const;

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
    virtual void push(Msg&) = 0;
    virtual void pop(Msg&) = 0;
#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)

    void dontDistribute(bool on);
    bool isDistributable() const;

    void needsDistribution(bool on);
    bool needsDistribution() const;

    bool isMultiPush() const;
    void setMultiPush(bool);

    bool isMultiInput() const;
    void setMultiInput(bool);

    bool isOwned() const;

    FieldChangeSource getFieldChangeSource() const;
    void setFieldChangeSource(FieldChangeSource src);

    /**
     * Register callback invoked after a field change or field touch.
     * The callback must take exactly one parameter of ChangedEvent.
     */
    ChangedCallbackHandle addChangedCallback(const ChangedCallback& callback);

    /**
     * Remove previously registered field changed callback via its handle.
     */
    void removeChangedCallback(const ChangedCallbackHandle& handle);

    /**
     * Register callback invoked after a field has been connected to this field.
     * The callback must take exactly one parameter of ConnectedEvent.
     */
    ConnectedCallbackHandle addConnectedCallback(const ConnectedCallback& callback);

    /**
     * Remove previously registered field ConnectedCallback via its handle.
     */
    void removeConnectedCallback(const ConnectedCallbackHandle& handle);

    /**
     * Register callback invoked after a field has been disconnected from this field.
     * The callback must take exactly one parameter of DisconnectedEvent.
     */
    DisconnectedCallbackHandle addDisconnectedCallback(const DisconnectedCallback& callback);

    /**
     * Remove previously registered field DisconnectedCallback via its handle.
     */
    void removeDisconnectedCallback(const DisconnectedCallbackHandle& handle);

    /**
     * Create a new instance of same field type and same value.
     * This type is not added to any container or event handler.
     */
    virtual Field* clone(void) const = 0;

    /**
     * Call evaluate on dependent field containers.
     */
    virtual void evaluateDependencies(void);

  protected:

    /**
     *  Fields are always wrapped up in a container.
     */
    void setContainer(FieldContainer* c, unsigned int i, bool owned);

    static Type registerType(const std::string& classname,
                             const std::string& parentname,
                             Create* creator);

    void fieldChanged(bool from_net = false, Field* triggered_from = 0);
    void reset();

    virtual void notify(Field* triggered_from = 0);

    void addAuditor(Field* field);
    void removeAuditor(Field* field);

    virtual void pullValue(Field* fromField) = 0;

    FieldContainer* mContainer;

  private:

    // disable copy construction
    Field(const Field&);

    void containerFieldChanged(const ChangedEvent& event);

    ChangedSignal mChangedSignal;
    ConnectedSignal mConnectedSignal;
    DisconnectedSignal mDisconnectedSignal;

    FieldPtrSet mAuditors;

    typedef std::pair<Field*, bool> FieldConnection;
    typedef std::list<FieldConnection> InputFieldsList;
    InputFieldsList mConnectedFrom;

    struct {
      bool touched            :1; // has the field been touched?
      bool dependencyEnabled  :1; // dependency on field?
      bool notifyEnabled      :1; // notify field changes?
      bool hasNotified        :1; // has the field been notified?
      bool dontWrite          :1; // never write this field to a stream
      bool dontDistribute     :1; // never distribute this field over the net
      bool needsDistribution  :1; // does the field need distribution
      bool multiPush          :1; // allow/disallow multiple value push to field auditors per frame
      bool multiInput         :1; // allow/disallow multiple inputs to field
      bool owned              :1; // is field owned by container?
    } mFlags;

    unsigned int mIndex;
    bool mIsNotifying;
    FieldChangeSource mSrc;

  };

  // streaming support
  extern AV_DLL InputStream& operator>>(InputStream& is, Field* field);
  extern AV_DLL OutputStream& operator<<(OutputStream& os, const Field* field);

} // namespace av

#define AV_FIELD_DEFINE(thisClass)                                      \
  template<> av::Type thisClass::sClassTypeId = av::Type::badType();

#endif // #if !defined(AVANGO_FIELD_H)
