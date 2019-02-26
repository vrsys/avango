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

#if !defined(AVANGO_OSG_DEPTH_H)
#define AVANGO_OSG_DEPTH_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Depth>
#include <avango/osg/Fields.h>
#include <avango/osg/StateAttribute.h>

namespace av
{
namespace osg
{
typedef ::osg::Depth OsgDepth;

/**
 * Wrapper for ::osg::Depth
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL Depth : public StateAttribute
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::osg::Depth is created.
     * Otherwise, the given ::osg::Depth is used.
     */
    Depth(OsgDepth* osgdepth = new OsgDepth());
    // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Depth();

  public:
    SFInt Function;
    SFDouble ZNear;
    SFDouble ZFar;
    SFBool WriteMask;

    /**
     * Get the wrapped ::osg::Depth.
     */
    ::osg::Depth* getOsgDepth() const;

  protected:
    virtual void getFunctionCB(const av::SFInt::GetValueEvent& event);
    virtual void setFunctionCB(const av::SFInt::SetValueEvent& event);
    virtual void getZNearCB(const av::SFDouble::GetValueEvent& event);
    virtual void setZNearCB(const av::SFDouble::SetValueEvent& event);
    virtual void getZFarCB(const av::SFDouble::GetValueEvent& event);
    virtual void setZFarCB(const av::SFDouble::SetValueEvent& event);
    virtual void getWriteMaskCB(const av::SFBool::GetValueEvent& event);
    virtual void setWriteMaskCB(const av::SFBool::SetValueEvent& event);

  private:
    ::osg::Depth* mOsgDepth;
};

typedef SingleField<Link<Depth>> SFDepth;
typedef MultiField<Link<Depth>> MFDepth;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::Depth>>;
template class AV_OSG_DLL MultiField<Link<osg::Depth>>;
#endif

} // namespace av

#endif
