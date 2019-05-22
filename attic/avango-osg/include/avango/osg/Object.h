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

#if !defined(AVANGO_OSG_OBJECT_H)
#define AVANGO_OSG_OBJECT_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/osg/Types.h>
#include <osg/Object>
#include <osg/observer_ptr>
#include <avango/FieldContainer.h>
#include <avango/osg/ObjectLink.h>

namespace av
{
namespace osg
{
template <class T>
T* get_from_osg_object(::osg::Object* osg_object)
{
    if(osg_object == 0)
        return 0;

    av::osg::ObjectLink* av_object_link = dynamic_cast<av::osg::ObjectLink*>(osg_object->getUserData());

    return (av_object_link != 0 ? dynamic_cast<T*>(av_object_link->getObject()) : 0);
}

/**
 * Abstract Wrapper for ::osg::Object
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL Object : public av::FieldContainer, public ::osg::Observer
{
    AV_FC_DECLARE_ABSTRACT();

  public:
    /**
     * Create av::osg::Object from existing osg::Object.
     */
    Object(::osg::Object* osgobject);

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Object();

  public:
    /**
     * Get the wrapped ::osg::Object.
     */
    ::osg::Object* getOsgObject() const;

  protected:
    /**
     * Use OSGs reference count for avango-osg objects.
     */
    /* virtual */ void refImpl();
    /* virtual */ void unrefImpl();
    /* virtual */ void unrefWithoutDeletionImpl();
    /* virtual */ int refCountImpl();

    /**
     * Virtual method from osg::Observer, is called on delete from osg
     * and is used to delete the Avango wrapper object.
     */
    /* virtual */ void objectDeleted(void*);

  private:
    ::osg::Object* mOsgObject;
};

typedef SingleField<Link<Object>> SFObject;
typedef MultiField<Link<Object>> MFObject;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::Object>>;
template class AV_OSG_DLL MultiField<Link<osg::Object>>;
#endif

} // namespace av

#endif
