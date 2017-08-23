import avango
import avango.script
import avango.gua
import avango.sound
import avango.sound.openal
from examples_common.GuaVE import GuaVE

soundtraverser = avango.sound.nodes.SoundTraverser()
openal_renderer = avango.sound.openal.nodes.OpenALSoundRenderer()
openal_renderer.Device.value = ""
soundtraverser.Renderers.value = [openal_renderer]


class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    def evaluate(self):
        self.MatrixOut.value = avango.gua.make_rot_mat(
            self.TimeIn.value * 2.0, 0.0, 1.0, 0.0)


# setup scenegraph
graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
loader = avango.gua.nodes.TriMeshLoader()

monkey1 = loader.create_geometry_from_file(
    "monkey",
    "data/objects/monkey.obj",
    avango.gua.LoaderFlags.NORMALIZE_SCALE)

monkey2 = loader.create_geometry_from_file(
    "monkey",
    "data/objects/monkey.obj",
    avango.gua.LoaderFlags.NORMALIZE_SCALE)

monkey1.Material.value.set_uniform(
    "Color",
    avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
monkey1.Material.value.set_uniform("Roughness", 0.3)
monkey1.Material.value.set_uniform("Metalness", 1.0)

monkey2.Material.value.set_uniform(
    "Color", avango.gua.Vec4(1.0, 0.266, 0.136, 1.0))
monkey2.Material.value.set_uniform("Roughness", 0.6)
monkey2.Material.value.set_uniform("Metalness", 0.0)

transform1 = avango.gua.nodes.TransformNode(
    Children=[monkey1]
    )
transform2 = avango.gua.nodes.TransformNode(
    Transform=avango.gua.make_trans_mat(-0.5, 0.0, 0.0),
    Children=[monkey2]
    )

light = avango.gua.nodes.LightNode(
    Type=avango.gua.LightType.POINT,
    Name="light",
    Color=avango.gua.Color(1.0, 1.0, 1.0),
    Brightness=100.0,
    Transform=(avango.gua.make_trans_mat(1, 1, 5) *
               avango.gua.make_scale_mat(30, 30, 30))
    )

size = avango.gua.Vec2ui(1024, 768)

window = avango.gua.nodes.GlfwWindow(
    Size=size,
    LeftResolution=size
    )

avango.gua.register_window("window", window)

cam = avango.gua.nodes.CameraNode(
    LeftScreenPath="/sound/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="window",
    Transform=avango.gua.make_trans_mat(0.0, 0.0, 3.5)
    )

res_pass = avango.gua.nodes.ResolvePassDescription()
res_pass.EnableSSAO.value = True
res_pass.SSAOIntensity.value = 4.0
res_pass.SSAOFalloff.value = 10.0
res_pass.SSAORadius.value = 7.0

#res_pass.EnableScreenSpaceShadow.value = True

res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
res_pass.Exposure.value = 1.0
res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)

anti_aliasing = avango.gua.nodes.SSAAPassDescription()

pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes=[
        avango.gua.nodes.TriMeshPassDescription(),
        avango.gua.nodes.LightVisibilityPassDescription(),
        res_pass,
        anti_aliasing,
        ])

cam.PipelineDescription.value = pipeline_description

screen = avango.gua.nodes.ScreenNode(
    Name="screen",
    Width=2,
    Height=1.5,
    Children=[cam]
    )

#set up sound
stereosound = avango.sound.nodes.SoundSource()
transform1.Children.value.append(stereosound)
stereosound.URL.value = "applause.ogg"
stereosound.Loop.value = False

monosound = avango.sound.nodes.SoundSource()
transform1.Children.value.append(monosound)
monosound.URL.value = "applause_mono.ogg"
monosound.Loop.value = False

root_group = avango.gua.nodes.TransformNode(Name="sound")
root_group.Children.value = [transform1, transform2, light, screen]
graph.Root.value.Children.value = [root_group]

#setup viewer
viewer = avango.gua.nodes.Viewer()
viewer.SceneGraphs.value = [graph]
viewer.Windows.value = [window]

monkey_updater = TimedRotate()

timer = avango.nodes.TimeSensor()
monkey_updater.TimeIn.connect_from(timer.Time)

transform1.Transform.connect_from(monkey_updater.MatrixOut)

guaVE = GuaVE()
guaVE.start(locals(), globals())

soundtraverser.RootNode.value = root_group
soundtraverser.Traverse.value = True

openal_renderer.ListenerPosition.connect_from(cam.Transform)

viewer.frame()

stereosound.Play.value = True
#monosound.Play.value = True

viewer.run()
