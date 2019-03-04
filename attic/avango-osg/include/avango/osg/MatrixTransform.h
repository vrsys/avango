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

#if !defined(AVANGO_OSG_MATRIXTRANSFORM_H)
#define AVANGO_OSG_MATRIXTRANSFORM_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/MatrixTransform>

#include <avango/osg/Fields.h>

#include <avango/osg/Transform.h>

namespace av
{
namespace osg
{
typedef ::osg::MatrixTransform OsgMatrixTransform;

/**
 * Wrapper for ::osg::MatrixTransform
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL MatrixTransform : public Transform
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::osg::MatrixTransform is created.
     * Otherwise, the given ::osg::MatrixTransform is used.
     */
    MatrixTransform(OsgMatrixTransform* osgmatrixtransform = new OsgMatrixTransform());
    // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~MatrixTransform();

  public:
    /**
     * Matrix directly sets and gets the Matrix of the wrapped ::osg::MatrixTransform.
     */
    SFMatrix Matrix;

    /**
     * Get the wrapped ::osg::MatrixTransform.
     */
    ::osg::MatrixTransform* getOsgMatrixTransform() const;

  protected:
    virtual void getMatrixCB(const av::osg::SFMatrix::GetValueEvent& event);
    virtual void setMatrixCB(const av::osg::SFMatrix::SetValueEvent& event);

  private:
    ::osg::MatrixTransform* mOsgMatrixTransform;
};

typedef SingleField<Link<MatrixTransform>> SFMatrixTransform;
typedef MultiField<Link<MatrixTransform>> MFMatrixTransform;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::MatrixTransform>>;
template class AV_OSG_DLL MultiField<Link<osg::MatrixTransform>>;
#endif

} // namespace av

#endif
