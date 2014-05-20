#include <boost/python.hpp>

#include <avango/Init.h>
#include <avango/Config.h>

#include <avango/gua/Init.hpp>
#include <avango/python/register_field.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/Init.h>
#endif

#include "math/math.hpp"

#include "scenegraph/Node.hpp"
#include "scenegraph/TransformNode.hpp"
#include "scenegraph/LODNode.hpp"
#include "scenegraph/TriMeshNode.hpp"
#include "scenegraph/Video3DNode.hpp"
#include "scenegraph/NURBSNode.hpp"
#include "scenegraph/ScreenNode.hpp"
#include "scenegraph/PointLightNode.hpp"
#include "scenegraph/SpotLightNode.hpp"
#include "scenegraph/SunLightNode.hpp"
#include "scenegraph/RayNode.hpp"
#include "scenegraph/TexturedQuadNode.hpp"
#include "scenegraph/SceneGraph.hpp"
#include "scenegraph/PickResult.hpp"

#include "math/BoundingBox.hpp"

#include "physics/RigidBodyNode.hpp"
#include "physics/CollisionShapeNode.hpp"
#include "physics/CollisionShape.hpp"
#include "physics/BoxShape.hpp"
#include "physics/ConvexHullShape.hpp"
#include "physics/CylinderShape.hpp"
#include "physics/PlaneShape.hpp"
#include "physics/SphereShape.hpp"
#include "physics/TriangleMeshShape.hpp"
#include "physics/Constraint.hpp"
#include "physics/FixedConstraint.hpp"
#include "physics/HingeConstraint.hpp"
#include "physics/Point2PointConstraint.hpp"
#include "physics/SliderConstraint.hpp"
#include "physics/PhysicsDatabase.hpp"
#include "physics/Physics.hpp"

#include "renderer/Pipeline.hpp"
#include "renderer/Window.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Databases.hpp"
#include "renderer/Camera.hpp"
#include "renderer/TriMeshLoader.hpp"
#include "renderer/Video3DLoader.hpp"
#include "renderer/NURBSLoader.hpp"
#include "renderer/Texture.hpp"

#include "viewer/Viewer.hpp"

#include "utils/Color.hpp"
#include "utils/Logger.hpp"

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include "network/NetTransform.h"
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

BOOST_PYTHON_MODULE(_gua)
{
    PyEval_InitThreads();


    av::gua::Init::initClass();

    init_Vec2();
    init_Vec2ui();
    init_Vec3();
    init_Vec4();
    init_Quat();

    init_Mat3();
    init_Mat4();

    init_Color();

    init_Node();
    init_TransformNode();
    init_LODNode();
#if defined(AVANGO_DISTRIBUTION_SUPPORT)
    init_NetTransform();
    av::gua::network::Init::initClass();
#endif
    init_TriMeshNode();
    init_Video3DNode();
    init_NURBSNode();
    init_ScreenNode();
    init_PointLightNode();
    init_SpotLightNode();
    init_SunLightNode();
    init_RayNode();
    init_TexturedQuadNode();
    init_SceneGraph();
    init_PickResult();

    init_BoundingBox();

    init_RigidBodyNode();
    init_CollisionShapeNode();
    init_CollisionShape();
    init_BoxShape();
    init_ConvexHullShape();
    init_CylinderShape();
    init_PlaneShape();
    init_SphereShape();
    init_TriangleMeshShape();
    init_Constraint();
    init_FixedConstraint();
    init_HingeConstraint();
    init_Point2PointConstraint();
    init_SliderConstraint();
    init_PhysicsDatabase();
    init_Physics();

    init_Pipeline();
    init_Window();
    init_Renderer();
    init_Databases();
    init_Camera();
    init_TriMeshLoader();
    init_Video3DLoader();
    init_NURBSLoader();
    init_Texture();

    init_Viewer();

    init_Logger();
}
