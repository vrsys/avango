import math
import avango
import avango.script
import avango.gua
import avango.gua as av
from examples_common.GuaVE import GuaVE

SPAWN_TIME = 0.4


class BallSpawner(avango.script.Script):

    TimeIn = avango.SFFloat()
    SceneGraph = avango.gua.SFSceneGraph()
    Physics = avango.gua.SFPhysics()
    MaxBallCount = avango.SFFloat()

    def __init__(self):

        self.super(BallSpawner).__init__()

        self.MaxBallCount.value = 50

        self.__last_spawn_time = -1
        self.__loader = avango.gua.nodes.TriMeshLoader()

        self.__spawned_balls = []
        self.red = True

    def evaluate(self):
        global SPAWN_TIME
        current_time = self.TimeIn.value

        if self.__last_spawn_time == -1 or current_time - self.__last_spawn_time >= SPAWN_TIME:
            self.__last_spawn_time = current_time

            if self.red:
                body = avango.gua.nodes.RigidBodyNode(
                    Name="body",
                    Mass=1.5,
                    Friction=0.7,
                    RollingFriction=0.04,
                    Restitution=0.8,
                    Transform=avango.gua.make_trans_mat(
                        math.sin(3 * current_time), 7.0, math.cos(3 *
                                                                  current_time)))
            else:
                body = avango.gua.nodes.RigidBodyNode(
                    Name="body",
                    Mass=2.0,
                    Friction=0.6,
                    RollingFriction=0.03,
                    Restitution=0.3,
                    Transform=avango.gua.make_trans_mat(
                        math.sin(3 * current_time), 7.0, math.cos(3 *
                                                                  current_time)))

            sphere_geometry = self.__loader.create_geometry_from_file(
                "sphere_geometry", "data/objects/sphere.obj")

            sphere_geometry.Transform.value = avango.gua.make_scale_mat(
                0.5, 0.5, 0.5)

            if self.red:
                sphere_geometry.Material.value.set_uniform(
                    #"Color", av.Vec4(0.08, 0.08, 0.09, 1.0))
                    "Color",
                    av.Vec4(0.9, 0.266, 0.136, 1.0))
                sphere_geometry.Material.value.set_uniform("Roughness", 0.75)
                sphere_geometry.Material.value.set_uniform("Metalness", 0.0)

            else:
                sphere_geometry.Material.value.set_uniform(
                    "Color", av.Vec4(1.0, 1.0, 1.0, 1.0))
                sphere_geometry.Material.value.set_uniform("Roughness", 0.2)
                sphere_geometry.Material.value.set_uniform("Metalness", 0.0)

            self.red = not self.red

            collision_shape_node = avango.gua.nodes.CollisionShapeNode(
                Name="collision_shape_node",
                ShapeName="sphere")

            collision_shape_node.Children.value.append(sphere_geometry)
            body.Children.value.append(collision_shape_node)
            self.SceneGraph.value.Root.value.Children.value.append(body)
            self.Physics.value.add_rigid_body(body)

            self.__spawned_balls.append(body)

            if len(self.__spawned_balls) > self.MaxBallCount.value:
                to_remove = self.__spawned_balls.pop(0)
                self.Physics.value.remove_rigid_body(to_remove)
                self.SceneGraph.value.Root.value.Children.value.remove(
                    to_remove)


def create_floor():
    floor_geometry = loader.create_geometry_from_file(
        "floor_geometry", "data/objects/plane.obj",
        av.LoaderFlags.NORMALIZE_SCALE | av.LoaderFlags.NORMALIZE_POSITION)

    floor_geometry.Transform.value = av.make_trans_mat(
        0, 1.0, 0) * av.make_scale_mat(10, 10.0, 10)
    floor_geometry.Material.value.set_uniform("Metalness", 0.0)
    floor_geometry.Material.value.set_uniform(
        "RoughnessMap", "data/textures/oakfloor2_roughness.png")
    floor_geometry.Material.value.set_uniform(
        "ColorMap", "data/textures/oakfloor2_basecolor.png")
    floor_geometry.Material.value.set_uniform(
        "NormalMap", "data/textures/oakfloor2_normal.png")

    av.create_box_shape("box", av.Vec3(10, 1, 10))
    floor_collision_shape = av.nodes.CollisionShapeNode(
        Name="floor_shape",
        ShapeName="box",
        Children=[floor_geometry])
    floor_body = av.nodes.RigidBodyNode(Name="floor_body",
                                        Mass=0,
                                        Friction=0.5,
                                        Restitution=0.7,
                                        Children=[floor_collision_shape])
    return floor_body

# setup scenegraph
graph = av.nodes.SceneGraph(Name="scenegraph")
physics = av.nodes.Physics()
#sphere_sh = av.create_sphere_shape("sphere", 0.25)
av.create_sphere_shape("sphere", 0.5)

loader = av.nodes.TriMeshLoader()

light = avango.gua.nodes.LightNode(
    Name="sun_light",
    Type=avango.gua.LightType.SUN,
    Color=avango.gua.Color(1.0, 1.0, 0.7),
    EnableShadows=True,
    ShadowMapSize=1024,
    ShadowOffset=0.0005,
    ShadowCascadedSplits=[0.1, 4, 7, 20],
    ShadowMaxDistance=30,
    ShadowNearClippingInSunDirection=100,
    ShadowFarClippingInSunDirection=100,
    Brightness=3,
    Transform=avango.gua.make_rot_mat(50, 0, 1, 0) * avango.gua.make_rot_mat(
        -40.0, 1.0, 0.0, 0.0))

size = av.Vec2ui(1024, 768)

window = av.nodes.GlfwWindow(Size=size, LeftResolution=size)

av.register_window("window", window)

cam = av.nodes.CameraNode(LeftScreenPath="/screen",
                          SceneGraph="scenegraph",
                          Resolution=size,
                          OutputWindowName="window",
                          Transform=av.make_trans_mat(0.0, 1.0, 3.0))

res_pass = av.nodes.ResolvePassDescription()
res_pass.EnableSSAO.value = True
res_pass.SSAOIntensity.value = 4.0
res_pass.SSAOFalloff.value = 10.0
res_pass.SSAORadius.value = 7.0

res_pass.EnvironmentLightingColor.value = av.Color(0.02, 0.02, 0.02)
res_pass.ToneMappingMode.value = av.ToneMappingMode.UNCHARTED
res_pass.Exposure.value = 1.0
res_pass.BackgroundColor.value = av.Color(0.45, 0.5, 0.6)

anti_aliasing = av.nodes.SSAAPassDescription()

pipeline_description = av.nodes.PipelineDescription(Passes=[
    av.nodes.TriMeshPassDescription(),
    av.nodes.LightVisibilityPassDescription(),
    res_pass,
    anti_aliasing,
])

cam.PipelineDescription.value = pipeline_description

screen = av.nodes.ScreenNode(Name="screen",
                             Width=2,
                             Height=1.5,
                             Transform=av.make_trans_mat(0, 5.0, 10),
                             Children=[cam])

floor = create_floor()
physics.add_rigid_body(floor)

graph.Root.value.Children.value = [light, screen, floor]

timer = avango.nodes.TimeSensor()
spawner = BallSpawner()
spawner.TimeIn.connect_from(timer.Time)
spawner.SceneGraph.value = graph
spawner.Physics.value = physics

#setup viewer
viewer = av.nodes.Viewer()
viewer.Physics.value = physics
viewer.SceneGraphs.value = [graph]
viewer.Windows.value = [window]

guaVE = GuaVE()
guaVE.start(locals(), globals())
viewer.run()
