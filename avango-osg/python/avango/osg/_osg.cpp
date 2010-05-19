// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include <boost/python.hpp>

#include <avango/Config.h>

#include <avango/osg/Drawable.h>
#include <avango/osg/Init.h>
#include <avango/osg/LineSegmentIntersector.h>
#include <avango/python/register_field.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/osg/network/Init.h>
#endif

#include "OSGAbsoluteTransform.h"
#include "OSGBlendFunc.h"
#include "OSGBoundingBoxCalculator.h"
#include "OSGBoundingBox.h"
#include "OSGBoundingSphere.h"
#include "OSGBox.h"
#include "OSGCameraAttachment.h"
#include "OSGCamera.h"
#include "OSGCapsule.h"
#include "OSGClipPlane.h"
#include "OSGClipNode.h"
#include "OSGCullFace.h"
#include "OSGDepth.h"
#include "OSGDrawable.h"
#include "OSGFog.h"
#include "OSGGeode.h"
#include "OSGGeometry.h"
#include "OSGGroup.h"
#include "OSGImage.h"
#include "OSGImageStream.h"
#include "OSGIntersection.h"
#include "OSGLayerGeode.h"
#include "OSGLayerGroup.h"
#include "OSGLight.h"
#include "OSGLightSource.h"
#include "OSGLineSegmentIntersector.h"
#include "OSGLoadFile.h"
#include "OSGLoadImage.h"
#include "OSGLoadImageStream.h"
#include "OSGMatrix.h"
#include "OSGMatrixTransform.h"
#include "OSGNode.h"
#include "OSGObject.h"
#include "OSGPanel.h"
#include "OSGPlane.h"
#include "OSGPolygonOffset.h"
#include "OSGProgram.h"
#include "OSGProjection.h"
#include "OSGQuad.h"
#include "OSGQuat.h"
#include "OSGShader.h"
#include "OSGSphere.h"
#include "OSGStateAttribute.h"
#include "OSGStateSet.h"
#include "OSGSwitch.h"
#include "OSGText.h"
#include "OSGTexture2DArray.h"
#include "OSGTexture2D.h"
#include "OSGTextureCubeMap.h"
#include "OSGTexturedQuad.h"
#include "OSGTexture.h"
#include "OSGTransform.h"
#include "OSGTriangleContainer.h"
#include "OSGUniform.h"
#include "OSGVectors.h"


#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include "NetMatrixTransform.h"
#endif

using namespace boost::python;
using namespace av::python;


namespace boost
 {
  namespace python
   {
    template <class T> struct pointee<av::Link<T> >
     {
      typedef T type;
     };
   }
 }

BOOST_PYTHON_MODULE(_osg)
{
  init_OSGVec2();
  init_OSGVec3();
  init_OSGVec4();
  init_OSGQuat();
  init_OSGMatrix();
  init_OSGBoundingBox();
  init_OSGBoundingSphere();
  init_OSGPlane();

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
  init_NetMatrixTransform();
  av::osg::network::Init::initClass();
#endif

  av::osg::Init::initClass();

  def("line_intersect", av::osg::lineIntersect);

  register_field<av::osg::SFDrawable>("SFDrawable");
  register_multifield<av::osg::MFDrawable>("MFDrawable");

  init_OSGIntersection();
  init_OSGObject();
  init_OSGNode();

  init_OSGGroup();
  init_OSGTransform();

  init_OSGCamera();
  init_OSGCameraAttachment();
  init_OSGCullFace();
  init_OSGClipPlane();
  init_OSGClipNode();
  init_OSGDepth();
  init_OSGBlendFunc();
  init_OSGBoundingBoxCalculator();
  init_OSGDrawable();
  init_OSGGeometry();
  init_OSGText();
  init_OSGPanel();
  init_OSGTexturedQuad();
  init_OSGFog();
  init_OSGShader();
  init_OSGProgram();
  init_OSGUniform();
  init_OSGLight();
  init_OSGLightSource();
  init_OSGPolygonOffset();
  init_OSGQuad();
  init_OSGStateAttribute();
  init_OSGStateSet();
  init_OSGGeode();
  init_OSGLayerGeode();
  init_OSGLayerGroup();
  init_OSGSwitch();
  init_OSGMatrixTransform();
  init_OSGLoadFile();
  init_OSGSphere();
  init_OSGBox();
  init_OSGCapsule();
  init_OSGAbsoluteTransform();
  init_OSGTriangleContainer();
  init_OSGProjection();
  init_OSGLineSegmentIntersector();
  init_OSGImage();
  init_OSGImageStream();
  init_OSGLoadImage();
  init_OSGLoadImageStream();
  init_OSGTexture();
  init_OSGTexture2D();
  init_OSGTexture2DArray();
  init_OSGTextureCubeMap();

}
