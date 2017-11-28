#include <avango/gua/Init.hpp>

#include <avango/Logger.h>

#include <avango/gua/scenegraph/Node.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/gua/scenegraph/ClippingPlaneNode.hpp>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/scenegraph/LODNode.hpp>
#include <avango/gua/scenegraph/TriMeshNode.hpp>
#include <avango/gua/scenegraph/DepthMapNode.hpp>

#if defined(AVANGO_VIDEO3D_SUPPORT)
#include <avango/gua/scenegraph/Video3DNode.hpp>
#include <avango/gua/renderer/Video3DLoader.hpp>
#include <avango/gua/renderer/Video3DPassDescription.hpp>
#endif
#if defined(AVANGO_SPOINTS_SUPPORT)
#include <avango/gua/scenegraph/SPointsNode.hpp>
#include <avango/gua/renderer/SPointsLoader.hpp>
#include <avango/gua/renderer/SPointsPassDescription.hpp>
#endif
#if defined(AVANGO_PBR_SUPPORT)
#include <avango/gua/scenegraph/PBRNode.hpp>
#include <avango/gua/scenegraph/PLODNode.hpp>
#endif
#include <avango/gua/scenegraph/NURBSNode.hpp>
#include <avango/gua/scenegraph/ScreenNode.hpp>
#include <avango/gua/scenegraph/LightNode.hpp>
#include <avango/gua/scenegraph/RayNode.hpp>
#include <avango/gua/scenegraph/TexturedQuadNode.hpp>
#include <avango/gua/scenegraph/TexturedScreenSpaceQuadNode.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#include <avango/gua/scenegraph/PickResult.hpp>
#include <avango/gua/scenegraph/CameraNode.hpp>

#include <avango/gua/math/BoundingBox.hpp>
#include <avango/gua/math/Frustum.hpp>
#include <avango/gua/math/BoundingSphere.hpp>

#if defined(AVANGO_PHYSICS_SUPPORT)
#include <avango/gua/physics/RigidBodyNode.hpp>
#include <avango/gua/physics/CollisionShapeNode.hpp>
#include <avango/gua/physics/CollisionShape.hpp>
#include <avango/gua/physics/BoxShape.hpp>
#include <avango/gua/physics/ConvexHullShape.hpp>
#include <avango/gua/physics/CylinderShape.hpp>
#include <avango/gua/physics/PlaneShape.hpp>
#include <avango/gua/physics/SphereShape.hpp>
#include <avango/gua/physics/TriangleMeshShape.hpp>
#include <avango/gua/physics/Constraint.hpp>
#include <avango/gua/physics/FixedConstraint.hpp>
#include <avango/gua/physics/HingeConstraint.hpp>
#include <avango/gua/physics/Point2PointConstraint.hpp>
#include <avango/gua/physics/SliderConstraint.hpp>
#include <avango/gua/physics/Physics.hpp>
#endif

#include <avango/gua/renderer/Renderer.hpp>
#include <avango/gua/renderer/TriMeshLoader.hpp>
#if defined(AVANGO_PBR_SUPPORT)
#include <avango/gua/renderer/PBRLoader.hpp>
#include <avango/gua/renderer/PLODLoader.hpp>
#include <avango/gua/renderer/PLODPassDescription.hpp>
#endif
#if defined(AVANGO_TV_3_SUPPORT)
#include <avango/gua/scenegraph/TV_3Node.hpp>
#include <avango/gua/renderer/TV_3Renderer.hpp>
#include <avango/gua/renderer/TV_3SurfacePassDescription.hpp>
#include <avango/gua/renderer/TV_3VolumePassDescription.hpp>
#endif
#include <avango/gua/renderer/NURBSLoader.hpp>
#include <avango/gua/renderer/WindowBase.hpp>
#include <avango/gua/renderer/Window.hpp>
#include <avango/gua/renderer/HeadlessSurface.hpp>
#include <avango/gua/renderer/GlfwWindow.hpp>
#include <avango/gua/renderer/MaterialShaderMethod.hpp>
#include <avango/gua/renderer/MaterialShaderDescription.hpp>
#include <avango/gua/renderer/Material.hpp>
#include <avango/gua/renderer/PipelinePassDescription.hpp>
#include <avango/gua/renderer/StencilPassDescription.hpp>
#include <avango/gua/renderer/TriMeshPassDescription.hpp>
#include <avango/gua/renderer/DepthMapPassDescription.hpp>
#include <avango/gua/renderer/TexturedQuadPassDescription.hpp>
#include <avango/gua/renderer/DebugViewPassDescription.hpp>
#include <avango/gua/renderer/BackgroundPassDescription.hpp>
#include <avango/gua/renderer/SkyMapPassDescription.hpp>
#include <avango/gua/renderer/BBoxPassDescription.hpp>
#include <avango/gua/renderer/EmissivePassDescription.hpp>
#include <avango/gua/renderer/PhysicallyBasedShadingPassDescription.hpp>
#include <avango/gua/renderer/TexturedScreenSpaceQuadPassDescription.hpp>
#include <avango/gua/renderer/FullscreenPassDescription.hpp>
#include <avango/gua/renderer/SSAOPassDescription.hpp>
#include <avango/gua/renderer/SSAAPassDescription.hpp>
#include <avango/gua/renderer/ResolvePassDescription.hpp>
#include <avango/gua/renderer/LightVisibilityPassDescription.hpp>
#include <avango/gua/renderer/PipelineDescription.hpp>

#include <avango/gua/viewer/Viewer.hpp>
#include <avango/gua/viewer/BlenderViewer.hpp>

#include <avango/gua/utils/Logger.hpp>
#include <avango/gua/utils/Ray.hpp>

#include <avango/gua/Fields.hpp>
#include <gua/guacamole.hpp>

namespace
{
    av::Logger& logger(av::getLogger("av::gua::Init"));
}

AV_TYPED_DEFINE_ABSTRACT(av::gua::Init);

/* static */ void
av::gua::Init::initClass()
{
    if (!isTypeInitialized())
    {
        char** argv;
        ::gua::init(0, argv);
        av::gua::initFields();

        av::gua::Node::initClass();
        av::gua::GeometryNode::initClass();
        av::gua::TransformNode::initClass();
        av::gua::ClippingPlaneNode::initClass();
        av::gua::LODNode::initClass();
        av::gua::TriMeshNode::initClass();        
        av::gua::DepthMapNode::initClass();

#if defined(AVANGO_VIDEO3D_SUPPORT)
        av::gua::Video3DNode::initClass();
#endif
#if defined(AVANGO_SPOINTS_SUPPORT)
        av::gua::SPointsNode::initClass();
#endif

        av::gua::CameraNode::initClass();
#if defined(AVANGO_PBR_SUPPORT)
        // av::gua::PBRNode::initClass();
        av::gua::PLODNode::initClass();
#endif

#if defined(AVANGO_TV_3_SUPPORT)
        av::gua::TV_3Node::initClass();
#endif
        // av::gua::NURBSNode::initClass();
        av::gua::ScreenNode::initClass();
        av::gua::LightNode::initClass();
        av::gua::RayNode::initClass();
        av::gua::TexturedQuadNode::initClass();
        av::gua::TexturedScreenSpaceQuadNode::initClass();
        av::gua::SceneGraph::initClass();
        av::gua::PickResult::initClass();

        av::gua::BoundingBox::initClass();
        av::gua::Frustum::initClass();
        av::gua::BoundingSphere::initClass();

#if defined(AVANGO_PHYSICS_SUPPORT)
        av::gua::RigidBodyNode::initClass();
        av::gua::CollisionShapeNode::initClass();
        av::gua::CollisionShape::initClass();
        av::gua::BoxShape::initClass();
        av::gua::ConvexHullShape::initClass();
        av::gua::CylinderShape::initClass();
        av::gua::PlaneShape::initClass();
        av::gua::SphereShape::initClass();
        av::gua::TriangleMeshShape::initClass();
        av::gua::Constraint::initClass();
        av::gua::FixedConstraint::initClass();
        av::gua::HingeConstraint::initClass();
        av::gua::Point2PointConstraint::initClass();
        av::gua::SliderConstraint::initClass();
        av::gua::Physics::initClass();
#endif

        av::gua::Renderer::initClass();
        av::gua::WindowBase::initClass();
        av::gua::Window::initClass();
        av::gua::HeadlessSurface::initClass();
        av::gua::GlfwWindow::initClass();
        av::gua::MaterialShaderMethod::initClass();
        av::gua::MaterialShaderDescription::initClass();
        av::gua::Material::initClass();
        av::gua::PipelinePassDescription::initClass();
        av::gua::StencilPassDescription::initClass();
        av::gua::TriMeshPassDescription::initClass();
        av::gua::DepthMapPassDescription::initClass();

#if defined(AVANGO_VIDEO3D_SUPPORT)
        av::gua::Video3DLoader::initClass();
        av::gua::Video3DPassDescription::initClass();
#endif
#if defined(AVANGO_SPOINTS_SUPPORT)
        av::gua::SPointsLoader::initClass();
        av::gua::SPointsPassDescription::initClass();
#endif
        av::gua::TexturedQuadPassDescription::initClass();
        av::gua::DebugViewPassDescription::initClass();
        av::gua::BackgroundPassDescription::initClass();
        av::gua::SkyMapPassDescription::initClass();
        av::gua::BBoxPassDescription::initClass();
        av::gua::EmissivePassDescription::initClass();
        av::gua::PhysicallyBasedShadingPassDescription::initClass();
        av::gua::TexturedScreenSpaceQuadPassDescription::initClass();
        av::gua::FullscreenPassDescription::initClass();
        av::gua::SSAOPassDescription::initClass();
        av::gua::SSAAPassDescription::initClass();
        av::gua::ResolvePassDescription::initClass();
        av::gua::LightVisibilityPassDescription::initClass();
        av::gua::PipelineDescription::initClass();
        av::gua::TriMeshLoader::initClass();

#if defined(AVANGO_PBR_SUPPORT)
        // av::gua::PBRLoader::initClass();
        av::gua::PLODLoader::initClass();
        av::gua::PLODPassDescription::initClass();
#endif
        // av::gua::NURBSLoader::initClass();

#if defined(AVANGO_TV_3_SUPPORT)
        av::gua::TV_3Loader::initClass();
        av::gua::TV_3SurfacePassDescription::initClass();
        av::gua::TV_3VolumePassDescription::initClass();
#endif

        av::gua::Viewer::initClass();

        av::gua::BlenderViewer::initClass();

        av::gua::Logger::initClass();
        av::gua::Ray::initClass();

        AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::gua::Init", true);
    }
}
