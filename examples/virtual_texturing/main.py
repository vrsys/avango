import avango
import avango.script
import avango.gua
from examples_common.GuaVE import GuaVE

import math

class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    Window = 0

    def set_window(self, window):
        self.Window = window


    def evaluate(self):
        #move the abstract geometry in a pseudo-random manner
        self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value * 25.0, 1.0, 1.0, 1.0)
        if 0 != self.Window:
            print("RenderingFPS :" + str(self.Window.RenderingFPS.value) )

def start():
    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    #fallback_mat = avango.gua.create_material(avango.gua.MaterialCapabilities.COLOR_VALUE)
    #fallback_mat.set_uniform("vt_test", "/opt/3d_models/virtual_texturing/earth_colour_86400x43200_256x256_1_rgb.atlas")
    #fallback_mat.EnableVirtualTexturing.value = True

    earth_geode_1 = loader.create_geometry_from_file(
                    "earth_1", "/opt/3d_models/virtual_texturing/earth_86400x43200_smooth_normals.obj",
                    avango.gua.LoaderFlags.NORMALIZE_SCALE)

    #hardcode monkey1 & monkey2 material, because there is no associated material files
    #the material names will both be "gua_default_material"

    #apply vt color 1:1
    earth_geode_1.Material.value.set_uniform("Roughness", 1.0)
    earth_geode_1.Material.value.set_uniform("vt_test", "/opt/3d_models/virtual_texturing/earth_colour_86400x43200_256x256_1_rgb.atlas")
    earth_geode_1.Material.value.EnableVirtualTexturing.value = True


    transform2 = avango.gua.nodes.TransformNode(
        Transform=avango.gua.make_trans_mat(0, 0.0, 0.0) * 
                  avango.gua.make_scale_mat(0.66),
        Children=[earth_geode_1])


    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="light",
        Color=avango.gua.Color(1.0, 1.0, 1.0),
        Brightness=100.0,
        Transform=(avango.gua.make_trans_mat(1, 1, 5) *
                   avango.gua.make_scale_mat(30, 30, 30)))

    size = avango.gua.Vec2ui(1600, 1200)

    window1 = avango.gua.nodes.GlfwWindow(Size=size, LeftResolution=size)
    window1.EnableVsync.value = False

    avango.gua.register_window("window1", window1)

    cam1 = avango.gua.nodes.CameraNode(
        LeftScreenPath="/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window1",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5))

    window2 = avango.gua.nodes.GlfwWindow(Size=size, LeftResolution=size)
    window2.EnableVsync.value = False

    avango.gua.register_window("window2", window2)

    cam2 = avango.gua.nodes.CameraNode(
        LeftScreenPath="/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window2",
        Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5))

    res_pass = avango.gua.nodes.ResolvePassDescription()
    res_pass.EnableSSAO.value = True
    res_pass.SSAOIntensity.value = 4.0
    res_pass.SSAOFalloff.value = 10.0
    res_pass.SSAORadius.value = 7.0

    res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)

    anti_aliasing = avango.gua.nodes.SSAAPassDescription()

    pipeline_description = avango.gua.nodes.PipelineDescription(Passes=[
        avango.gua.nodes.TriMeshPassDescription(),
        avango.gua.nodes.LightVisibilityPassDescription(),
        res_pass,
        anti_aliasing,
    ])

    cam1.PipelineDescription.value = pipeline_description
    cam2.PipelineDescription.value = pipeline_description

    screen = avango.gua.nodes.ScreenNode(Name="screen",
                                         Width=2,
                                         Height=1.5,
                                         Children=[cam1, cam2])

    graph.Root.value.Children.value = [transform2, light, screen]

    vt_backend = avango.gua.VTBackend()
    vt_backend.add_camera(cam1)
    vt_backend.add_camera(cam2)
    vt_backend.start_backend()

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window1, window2]
    viewer.DesiredFPS.value = 500.0

    monkey_updater = TimedRotate()
    monkey_updater.set_window(window1)

    timer = avango.nodes.TimeSensor()
    monkey_updater.TimeIn.connect_from(timer.Time)

    earth_geode_1.Transform.connect_from(monkey_updater.MatrixOut)

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    #pipeline_description.touch()
    viewer.run()


if __name__ == '__main__':
    start()
