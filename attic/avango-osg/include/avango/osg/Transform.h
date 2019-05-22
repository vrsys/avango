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

#if !defined(AVANGO_OSG_TRANSFORM_H)
#define AVANGO_OSG_TRANSFORM_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Transform>

#include <avango/osg/Group.h>

namespace av
{
namespace osg
{
typedef ::osg::Transform OsgTransform;

/**
 * Wrapper for ::osg::Transform
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL Transform : public Group
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::osg::Transform is created.
     * Otherwise, the given ::osg::Transform is used.
     */
    Transform(OsgTransform* osgtransform = new OsgTransform());
    // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Transform();

  public:
    // TODO: Wrap this as an int and utilize the boost::python enum wrapper
    // String representing an enum for setting the reference frame of this transform
    // valid options: RELATIVE_RF, ABSOLUTE_RF, ABSOLUTE_RF_INHERIT_VIEWPOINT
    ::av::SFString ReferenceFrame;

    /* virtual */ void fieldHasChangedLocalSideEffect(const av::Field& field);

    /**
     * Get the wrapped ::osg::Transform.
     */
    ::osg::Transform* getOsgTransform() const;

  private:
    ::osg::Transform* mOsgTransform;
};

typedef SingleField<Link<Transform>> SFTransform;
typedef MultiField<Link<Transform>> MFTransform;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::Transform>>;
template class AV_OSG_DLL MultiField<Link<osg::Transform>>;
#endif

} // namespace av

#endif
