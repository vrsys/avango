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

#if !defined(AVANGO_OSG_STATEATTRIBUTE_H)
#define AVANGO_OSG_STATEATTRIBUTE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/StateAttribute>

#include <avango/osg/Object.h>

namespace av
{
namespace osg
{
/**
 * Abstract Wrapper for ::osg::StateAttribute
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL StateAttribute : public Object
{
    AV_FC_DECLARE_ABSTRACT();

  public:
    /**
     * Constructor.
     */
    StateAttribute(::osg::StateAttribute* osgstateattribute);

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~StateAttribute();

  public:
    /**
     * Get the wrapped ::osg::Object.
     */
    ::osg::StateAttribute* getOsgStateAttribute() const;

  private:
    ::osg::StateAttribute* mOsgStateAttribute;
};

typedef SingleField<Link<StateAttribute>> SFStateAttribute;
typedef MultiField<Link<StateAttribute>> MFStateAttribute;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::StateAttribute>>;
template class AV_OSG_DLL MultiField<Link<osg::StateAttribute>>;
#endif

} // namespace av

#endif
