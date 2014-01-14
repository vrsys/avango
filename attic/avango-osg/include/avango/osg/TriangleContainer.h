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

#if !defined(AVANGO_OSG_TRIANGLECONTAINER_H)
#define AVANGO_OSG_TRIANGLECONTAINER_H

#include <avango/osg/Fields.h>
#include <avango/osg/Geometry.h>
#include <osg/Point>

namespace av
{
  namespace osg
  {
    class AV_OSG_DLL TriangleContainer : public Geometry
    {
      AV_FC_DECLARE();

    public:
      TriangleContainer();
      virtual ~TriangleContainer();

      void fieldHasChangedLocalSideEffect(const av::Field& field);
      void evaluateLocalSideEffect();

      MFVec3 Vertices;
      MFVec4 Colors;
      MFVec3 Normals;
      SFInt Mode;
      SFBool ModeToggle;
      SFFloat PointSize;

      unsigned getTriangleCount() const;
      unsigned getNormalCount() const;

    private:

      void update(); //rebuilds the osg geometry buffer upon call
      void updateVertexBuffer();
      void updateColor();
      void updateMode();
      void updateNormal();

      bool mVecFieldHasChanged;
      bool mColorFieldHasChanged;
      bool mModeFieldHasChanged;
      bool mNormalFieldHasChanged;

      unsigned mNormalCount;
      unsigned mTriangleCount;
    };

    typedef SingleField<Link<TriangleContainer> > SFTriangleContainer;
    typedef MultiField<Link<TriangleContainer> > MFTriangleContainer;
  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::TriangleContainer> >;
  template class AV_OSG_DLL MultiField<Link<osg::TriangleContainer> >;
#endif

} // namespace av

#endif
