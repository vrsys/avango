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

#if !defined(AVANGO_OSG_DRAWABLE_H)
#define AVANGO_OSG_DRAWABLE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Drawable>
#include <avango/osg/Object.h>
#include <avango/osg/StateSet.h>

namespace av
{
namespace osg
{
/**
 * Abstract Wrapper for ::osg::Drawable
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL Drawable : public Object
{
    AV_FC_DECLARE_ABSTRACT();

  public:
    /**
     * Constructor.
     */
    Drawable(::osg::Drawable* osgdrawable);

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Drawable();

  public:
    SFStateSet StateSet;

    /**
     * Get the wrapped ::osg::Drawable.
     */
    ::osg::Drawable* getOsgDrawable() const;
    const ::osg::BoundingBox getBoundingBox() const { return mOsgDrawable->getBound(); }

  protected:
    virtual void getStateSetCB(const av::osg::SFStateSet::GetValueEvent& event);
    virtual void setStateSetCB(const av::osg::SFStateSet::SetValueEvent& event);

  private:
    ::osg::Drawable* mOsgDrawable;
};

typedef SingleField<Link<Drawable>> SFDrawable;
typedef MultiField<Link<Drawable>> MFDrawable;

} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::Drawable>>;
template class AV_OSG_DLL MultiField<Link<osg::Drawable>>;
#endif

} // namespace av

#endif
