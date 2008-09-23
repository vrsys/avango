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

#include <avango/Field.h>

#include <algorithm>
#include <iostream>
#include <iterator>

#include <boost/bind.hpp>

#include <avango/Assert.h>
#include <avango/Config.h>
#include <avango/ContainerPool.h>
#include <avango/Distributed.h>
#include <avango/FieldContainer.h>
#include <avango/InputStream.h>
#include <avango/Logger.h>
#include <avango/OutputStream.h>
#include <avango/Reader.h>
#include <avango/WriteAction.h>

#include <avango/Config.h>

namespace
{
  av::Logger& logger(av::getLogger("av::Field"));

  /// Helper class to make setting/unsetting bool values exception safe (RAII Idiom)
  class SetBoolFlag
  {
  public:
    SetBoolFlag(bool& value) :
      mValueRef(value)
    {
      mValueRef = true;
    }
    ~SetBoolFlag()
    {
      mValueRef = false;
    }
  private:
    bool& mValueRef;
  };

  bool defaultPushIsMulti = true;
  bool defaultInputIsMulti = true;

} // namespace

AV_TYPED_DEFINE_ABSTRACT(av::Field);

void
av::Field::setContainer(av::FieldContainer* c, unsigned int i, bool owned)
{
  AV_ASSERT(c);

  mContainer = c;
  mIndex = i;
  mFlags.owned = owned;
}

av::FieldContainer*
av::Field::getContainer() const
{
  AV_ASSERT(mContainer);

  return mContainer;
}

bool
av::Field::isBound() const
{
  return (mContainer != 0);
}

unsigned int
av::Field::getIndex() const
{
  return mIndex;
}

av::Field*
av::Field::getConnectedField(unsigned int index)
{
  if (index >= mConnectedFrom.size())
  {
    return 0;
  }

  InputFieldsList::iterator listIter = mConnectedFrom.begin();
  std::advance(listIter, index);
  return listIter->first;
}

unsigned int
av::Field::getNumberOfConnectedFields() const
{
  return mConnectedFrom.size();
}

void
av::Field::enableDependency(bool on)
{
  mFlags.dependencyEnabled = on;
}

bool
av::Field::dependencyEnabled()
{
  return mFlags.dependencyEnabled;
}

void
av::Field::enableNotify(bool on)
{
  mFlags.notifyEnabled = on;
}

bool
av::Field::notifyEnabled()
{
  return mFlags.notifyEnabled;
}

void
av::Field::notify(Field* triggered_from)
{
#if defined(AVANGO_DEBUG)
  if (mFlags.hasNotified && !mAuditors.empty())
  {
    SingleField<std::string>* container_name =
      dynamic_cast<SingleField<std::string>* >(getContainer()->getField("Name"));

    LOG_TRACE(logger) << "notify: "
                   << (0 != container_name ? container_name->getValue() : "unnamed")
                   << "."
                   <<  getName()
                   << " of type <"
                   << getContainer()->getTypeId().getName()
                   << "."
                   << getTypeId().getName() << "> ("
                   << (mFlags.multiPush ? "multi" : "single")
                   << "-pushed, "
                   << mAuditors.size()
                   << " auditor"
                   << (1 != mAuditors.size() ? "s" : "")
                   << "was already notified.";
  }
#endif

  if (mIsNotifying)
  {
    throw std::runtime_error("Field triggered while in on notify call. Possible loop.");
  }

  if (mFlags.hasNotified && !mFlags.multiPush)
  {
    return;
  }

  SetBoolFlag isNotifying(mIsNotifying);

  // copy and reference auditors while in notify because the connection topology may change

  FieldPtrVec auditors;
  auditors.reserve(mAuditors.size());
  std::copy(mAuditors.begin(), mAuditors.end(), std::back_inserter(auditors));

  FieldPtrVec::const_iterator current;
  FieldPtrVec::const_iterator past_of_end = auditors.end();

  // create references to field containers
  std::vector<Link<FieldContainer> > auditor_containers;
  auditor_containers.reserve(mAuditors.size());
  std::transform(auditors.begin(), auditors.end(), std::back_inserter(auditor_containers),
                 boost::bind(&Field::getContainer, _1));

  // notify

  for (current = auditors.begin(); current != past_of_end; ++current)
  {
    Field* to_field(*current);

    AV_ASSERT(to_field != 0);

    if (to_field != triggered_from)
    {
      to_field->setFieldChangeSource(connection);
      to_field->pullValue(this);
    }
  }

  mFlags.hasNotified = true;
}

void
av::Field::touch()
{
  fieldChanged(false);
}

void
av::Field::reset()
{
  mFlags.touched = false;
  mFlags.hasNotified = false;
}

void
av::Field::dontWrite(bool on)
{
  mFlags.dontWrite = on;
}

bool
av::Field::isWritable() const
{
  return !mFlags.dontWrite;
}

void
av::Field::dontDistribute(bool on)
{
  mFlags.dontDistribute = on;
}

void
av::Field::needsDistribution(bool on)
{
  mFlags.needsDistribution = on;
}

bool
av::Field::needsDistribution() const
{
  return mFlags.needsDistribution;
}

bool
av::Field::isDistributable() const
{
  return !mFlags.dontDistribute;
}

bool
av::Field::isMultiPush () const
{
  return mFlags.multiPush;
}

void
av::Field::setMultiPush (bool onoff)
{
  if (!onoff)
  {
    logger.warn() << "Disabling multi-push on a field is deprecated.";
  }

  mFlags.multiPush = onoff;
}

bool
av::Field::isMultiInput () const
{
  return mFlags.multiInput;
}

void
av::Field::setMultiInput (bool onoff)
{
  if (!onoff)
  {
    logger.warn() << "Disabling multi-input on a field is deprecated.";
  }

  mFlags.multiInput = onoff;
}

bool
av::Field::isOwned() const
{
  return mFlags.owned;
}

av::Field::Field() :
  mContainer(0),
  mIndex(0),
  mIsNotifying(false),
  mSrc(internal)
{
  mFlags.touched = false;
  mFlags.notifyEnabled = true;
  mFlags.dependencyEnabled = true;
  mFlags.hasNotified = false;
  mFlags.dontWrite = false;
  mFlags.dontDistribute = false;
  mFlags.needsDistribution = false;
  mFlags.multiPush = defaultPushIsMulti;
  mFlags.multiInput = defaultInputIsMulti;
  mFlags.owned = false;
}

const std::string&
av::Field::getName() const
{
  AV_ASSERT(mContainer != 0);

  return mContainer->getFieldName(mIndex);
}

void
av::Field::fieldChanged(bool fromNet, Field* triggeredFrom)
{
  if (!mContainer)
  {
    return;
  }

  if (!fromNet)
  {
    mFlags.needsDistribution = true;
  }

  if (notifyEnabled())
  {
    mChangedSignal(ChangedEvent(this, fromNet, triggeredFrom));
    notify(triggeredFrom);
    ContainerPool::notifyFieldHasChanged(this);
    setFieldChangeSource(fromNet ? net : internal);
  }

  mFlags.touched = true;
}


void
av::Field::connectFrom(av::Field* field, bool dependent)
{
  if (!field)
  {
    throw std::invalid_argument("av::Field* field is 0");
  }

  // discard old connection
  if (!mFlags.multiInput)
  {
    disconnect();
  }

  mConnectedFrom.push_back(std::make_pair(field, dependent));
  field->addAuditor(this);
  mConnectedSignal(ConnectedEvent(this, field));

  ContainerPool::notifyConnect(this);
}

void
av::Field::disconnect()
{
  if (mConnectedFrom.empty())
  {
    return;
  }

  // prevent deletion if container is only referenced via field connections
  Link<FieldContainer> container_ref(getContainer());

  std::vector<Field*> disconnected_fields;
  disconnected_fields.reserve(mConnectedFrom.size());

  for (InputFieldsList::iterator list_iter = mConnectedFrom.begin();
       list_iter != mConnectedFrom.end();
       ++list_iter)
  {
    list_iter->first->removeAuditor(this);
    disconnected_fields.push_back(list_iter->first);
  }

  mConnectedFrom.clear();

  // we can notice disconnects this way
  touch();

  mDisconnectedSignal(DisconnectedEvent(this, disconnected_fields));

  ContainerPool::notifyDisconnect(this);
}

namespace
{
  template<class A, class B> class first_equal_to
  {
    typedef std::pair<A, B> Pair;
    A mValue;

  public:

    first_equal_to(const A& value) :
      mValue(value)
    {
    }

    bool operator() (const Pair& x)
    {
      return x.first == mValue;
    }
  };
}

void
av::Field::disconnectFrom(av::Field* field)
{
  if (mConnectedFrom.empty())
  {
    return;
  }

  InputFieldsList::iterator field_iter = std::find_if(mConnectedFrom.begin(),
                                                   mConnectedFrom.end(),
                                                   first_equal_to<Field*, bool>(field));
  if (field_iter == mConnectedFrom.end())
  {
    return;
  }

  // prevent deletion if container is only referenced via field connections
  Link<FieldContainer> container_ref(getContainer());

  field_iter->first->removeAuditor(this);
  mConnectedFrom.erase(field_iter);

  // we can notice disconnects this way
  touch();

  mDisconnectedSignal(DisconnectedEvent(this, std::vector<Field*>(1, field)));

  ContainerPool::notifyDisconnect(this);
}

const av::Field::FieldPtrSet&
av::Field::getAuditors() const
{
  return mAuditors;
}

void
av::Field::disconnectAuditors()
{
  while (!mAuditors.empty())
  {
    (*mAuditors.begin())->disconnectFrom(this);
  }
}

void
av::Field::addAuditor(av::Field* field)
{
  AV_ASSERT(field);

  field->getContainer()->reference();
  mAuditors.insert(field);
  field->pullValue(this);
}

void
av::Field::removeAuditor(av::Field* field)
{
  AV_ASSERT(field);

  mAuditors.erase(field);
  field->getContainer()->unreference();
}

av::Field::FieldChangeSource
av::Field::getFieldChangeSource() const
{
  return mSrc;
}

void
av::Field::setFieldChangeSource(FieldChangeSource src)
{
  mSrc = src;
}

void
av::Field::containerFieldChanged(const ChangedEvent& event)
{
  AV_ASSERT(this == event.getField());
  mContainer->fieldChanged(*(event.getField()), event.getChangedFromNet());
}

void
av::Field::bind(av::FieldContainer* container, const std::string& name, bool owned)
{
  AV_ASSERT(container);

  if (isBound())
  {
    logger.warn() << "bind: "
    << (container ? container->getTypeId().getName() : "null-pointer" )
    << "."
    << name
    << " already bound";
    return;
  }

  unsigned int index = container->addField(this, name);
  setContainer(container, index, owned);
  addChangedCallback(boost::bind(&Field::containerFieldChanged, this, _1));
}

/* vitual */ void
av::Field::clear()
{}

/* virtual */
av::Field::~Field()
{
  disconnectAuditors();

  AV_ASSERT(mConnectedFrom.size() == 0);
  AV_ASSERT(mAuditors.size() == 0);
}

void
av::Field::initClass()
{
  if (!isTypeInitialized())
  {
    AV_TYPED_INIT_ABSTRACT(av::Typed, av::Field, true);
  }
}

av::Type
av::Field::registerType(const std::string&  classname,
                        const std::string& parentname,
                        av::Create* creator)
{
#if defined(AVANGO_DEBUG)
  logger.debug() << "registerType: registration request for type '"
                 << classname
                 << "' with parent type '"
                 << parentname
                 << "'.";
#endif

  Type type (Type::getByName(classname));

  if (Type::badType() != type)
  {
    logger.warn() << "registerType: type '"
                  << classname
                  << "' already registered, returning '"
                  << type.getName()
                  << "'!";
    return type;
  }

  Type parent;
  if (!parentname.empty())
  {
    parent = Type::getByName(parentname);
    if (Type::badType() == parent)
    {
      logger.warn() << "registerType: parent type '"
      << parentname
      << "' of type '"
      << classname
      << "' doesn't exist, returning '"
      << Type::badType().getName()
      << "'!";
      return Type::badType();
    }
  }
  else
  {
    logger.warn() << "registerType: setting parent type '"
    << parentname << "' of type '"
    << classname
    << "' to '"
    << Type::badType().getName()
    << "'!";
    parent = Type::badType();
  }

  type = Type::createType(parent, classname, creator, true);

#if defined(AVANGO_DEBUG)
  logger.debug() << "registerType: created type '"
                 << type.getName()
                 << "' (id: "
                 << type.getId()
                 << ", parent: '"
                 << type.getParent().getName()
                 << ")";
#endif
  return type;
}

av::InputStream&
av::operator>>(av::InputStream& is, av::Field* field)
{
  field->read(is);
  return is;
}

av::OutputStream&
av::operator<<(av::OutputStream& os, const av::Field* field)
{
  const_cast<av::Field*>(field)->write(os);
  return os;
}

void
av::Field::writeConnection(av::OutputStream& os)
{
  Field* connected_field = getConnectedField();

  if (!connected_field)
  {
    (std::ostream&) os << 'N';
    return;
  }

  (std::ostream&) os << 'C';

  uint32_t index = connected_field->getIndex();

  os.write((char*) &index, sizeof(index));

  if (os.getWriteAction()->isObjectWritten(connected_field->getContainer()))
  {
    // object is alraedy in file, only add reference
    (std::ostream&) os << 'R';

    os << os.getWriteAction()->lookupObjectId(connected_field->getContainer());
  }
  else
  {
    // object is not written, write it.
    (std::ostream&) os << 'O';

    // this will recurse write actions on base links
    os.getWriteAction()->traverse(connected_field->getContainer());
  }
}

void
av::Field::readConnection(av::InputStream& is)
{
  char con_flag;

  (std::istream&) is >> con_flag;

  if (con_flag != 'C')
  {
    return;
  }

  uint32_t index = 0;

  is.read((char*) &index, sizeof(index));

  av::Base* from_base = 0;
  char    reference_flag;

  (std::istream&) is >> reference_flag;

  if (reference_flag == 'R') {
    // object is already there, just connect
    std::string obj_id;

    is >> obj_id;

    from_base = is.getReader()->lookupObject(obj_id);
  } else {
    // object is new, load it
    from_base = is.getReader()->readObject(is);
  }

  // make connection
  if (from_base &&
      from_base->getTypeId().isOfType(FieldContainer::getClassTypeId()))
  {
    FieldContainer* fc = (FieldContainer*) from_base;
    Field*          from_field = fc->getField(index);

    this->connectFrom(from_field);
  }
  else
  {
    logger.warn() << "read: can't resolve field connection for type '"
    << (from_base ? from_base->getTypeId().getName() : "unnamed at 0x0")
    << "'!";
  }
}

av::Field::ChangedCallbackHandle
av::Field::addChangedCallback(const ChangedCallback& callback)
{
  return mChangedSignal.connect(callback);
}

void
av::Field::removeChangedCallback(const ChangedCallbackHandle& handle)
{
  handle.disconnect();
}

av::Field::ConnectedCallbackHandle
av::Field::addConnectedCallback(const ConnectedCallback& callback)
{
  return mConnectedSignal.connect(callback);
}

void
av::Field::removeConnectedCallback(const ConnectedCallbackHandle& handle)
{
  handle.disconnect();
}

av::Field::DisconnectedCallbackHandle
av::Field::addDisconnectedCallback(const DisconnectedCallback& callback)
{
  return mDisconnectedSignal.connect(callback);
}

void
av::Field::removeDisconnectedCallback(const DisconnectedCallbackHandle& handle)
{
  handle.disconnect();
}

void
av::Field::evaluateDependencies(void)
{
  if ( (mConnectedFrom.empty()) || (!dependencyEnabled()) )
  {
    return;
  }

  // reference all connected containers as connections may change dynamically
  std::list<Link<FieldContainer> > connected_from_containers;
  for (Field::InputFieldsList::iterator from_field_it = mConnectedFrom.begin();
      from_field_it != mConnectedFrom.end(); ++from_field_it)
  {
    if (from_field_it->second)
    {
      connected_from_containers.push_back(from_field_it->first->getContainer());
    }
  }

  while (!connected_from_containers.empty())
  {
    connected_from_containers.front()->callEvaluate();
    connected_from_containers.pop_front();
  }
}
