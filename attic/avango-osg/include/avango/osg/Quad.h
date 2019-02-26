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

#if !defined(AVANGO_OSG_QUAD_H)
#define AVANGO_OSG_QUAD_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/osg/Geometry.h>
#include <avango/osg/Fields.h>
#include <avango/StandardFields.h>

namespace av
{
namespace osg
{
/**
 * Node that creates a Quad as Drawable
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL Quad : public Geometry
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor.
     */
    Quad();

    SFFloat Width;
    SFFloat Height;
    SFVec4 Color;
    SFVec3 Position;

    /* virtual */ void fieldHasChangedLocalSideEffect(const av::Field& field);
    /* virtual */ void evaluateLocalSideEffect();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Quad();

  private:
    bool mGeometryChanged;
    bool mColorChanged;
    bool mFirstFrame;

    ::osg::ref_ptr<::osg::Vec3Array> mVertexArray;
    ::osg::ref_ptr<::osg::Vec3Array> mNormals;
    ::osg::ref_ptr<::osg::Vec4Array> mColors;

    void updateGeometry();
    void updateColor();
};

typedef SingleField<Link<Quad>> SFQuad;
typedef MultiField<Link<Quad>> MFQuad;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::Quad>>;
template class AV_OSG_DLL MultiField<Link<osg::Quad>>;
#endif

} // namespace av

#endif
