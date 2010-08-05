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

#if !defined(AVANGO_OSG_FIELDS_H)
#define AVANGO_OSG_FIELDS_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/osg/Types.h>

#include <avango/SingleField.h>
#include <avango/MultiField.h>

namespace av
{
  namespace osg
  {
    void AV_OSG_DLL initFields();

    typedef SingleField< ::osg::Matrixf> SFMatrixf;
    typedef MultiField< ::osg::Matrixf> MFMatrixf;

    typedef SingleField< ::osg::Matrixd> SFMatrixd;
    typedef MultiField< ::osg::Matrixd> MFMatrixd;

    // ::osg::Matrix is a typedef for ::osg::Matrix{f,d}
    typedef SingleField< ::osg::Matrix> SFMatrix;
    typedef MultiField< ::osg::Matrix> MFMatrix;

    typedef SingleField< ::osg::Vec2f> SFVec2f;
    typedef MultiField< ::osg::Vec2f> MFVec2f;

    typedef SingleField< ::osg::Vec2d> SFVec2d;
    typedef MultiField< ::osg::Vec2d> MFVec2d;

    // ::osg::Vec2 is a typedef for ::osg::Vec2{f,d}
    typedef SingleField< ::osg::Vec2> SFVec2;
    typedef MultiField< ::osg::Vec2> MFVec2;

    typedef SingleField< ::osg::Vec3f> SFVec3f;
    typedef MultiField< ::osg::Vec3f> MFVec3f;

    typedef SingleField< ::osg::Vec3d> SFVec3d;
    typedef MultiField< ::osg::Vec3d> MFVec3d;

    // ::osg::Vec3 is a typedef for ::osg::Vec3{f,d}
    typedef SingleField< ::osg::Vec3> SFVec3;
    typedef MultiField< ::osg::Vec3> MFVec3;

    typedef SingleField< ::osg::Vec4f> SFVec4f;
    typedef MultiField< ::osg::Vec4f> MFVec4f;

    typedef SingleField< ::osg::Vec4d> SFVec4d;
    typedef MultiField< ::osg::Vec4d> MFVec4d;

    // ::osg::Vec4 is a typedef for ::osg::Vec4{f,d}
    typedef SingleField< ::osg::Vec4> SFVec4;
    typedef MultiField< ::osg::Vec4> MFVec4;

    typedef SingleField< ::osg::Quat> SFQuat;
    typedef MultiField< ::osg::Quat> MFQuat;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField< ::osg::Matrixf>;
  template class AV_OSG_DLL MultiField< ::osg::Matrixf>;

  template class AV_OSG_DLL SingleField< ::osg::Matrixd>;
  template class AV_OSG_DLL MultiField< ::osg::Matrixd>;

  template class AV_OSG_DLL SingleField< ::osg::Vec2f>;
  template class AV_OSG_DLL MultiField< ::osg::Vec2f>;

  template class AV_OSG_DLL SingleField< ::osg::Vec2d>;
  template class AV_OSG_DLL MultiField< ::osg::Vec2d>;

  template class AV_OSG_DLL SingleField< ::osg::Vec3f>;
  template class AV_OSG_DLL MultiField< ::osg::Vec3f>;

  template class AV_OSG_DLL SingleField< ::osg::Vec3d>;
  template class AV_OSG_DLL MultiField< ::osg::Vec3d>;

  template class AV_OSG_DLL SingleField< ::osg::Vec4f>;
  template class AV_OSG_DLL MultiField< ::osg::Vec4f>;

  template class AV_OSG_DLL SingleField< ::osg::Vec4d>;
  template class AV_OSG_DLL MultiField< ::osg::Vec4d>;

  template class AV_OSG_DLL SingleField< ::osg::Quat>;
  template class AV_OSG_DLL MultiField< ::osg::Quat>;
#endif
}

#endif
