import avango
import avango.script
import avango.gua
from examples_common.GuaVE import GuaVE

import math

class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    def evaluate(self):
        #move the abstract geometry in a pseudo-random manner
        self.MatrixOut.value = avango.gua.make_rot_mat(math.sin(0.5*self.TimeIn.value) * 50.0,
                                                       1.0, 1.0, 0.0) * avango.gua.make_rot_mat(math.cos(1.5*self.TimeIn.value) * 150.0, 0.0, 0.0, 1.0) * avango.gua.make_rot_mat(math.sin(10*self.TimeIn.value) * 15.0, 1.0, 0.0, 1.0)

def set_graph_pbr_materials_based_on_name(node):
    node_children = node.Children.value

    if len(node_children) > 0:
        print("" + str(len(node_children)) )
        for child in node_children:
            if(child.has_field("Material")): #check whether we can access the Material field
                                             # (e.g. Transform nodes do not contain any material)

                #print the name of the material of traversed nodes
                print(child.Material.value.ShaderName.value)
                if "Golden_Material" in child.Material.value.ShaderName.value:
                    child.Material.value.set_uniform("Color",
                                       avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
                    child.Material.value.set_uniform("Roughness", 0.3)
                    child.Material.value.set_uniform("Metalness", 1.0)

                if "Red" in child.Material.value.ShaderName.value:
                    child.Material.value.set_uniform("Color",
                                       avango.gua.Vec4(1.0, 0.266, 0.136, 1.0))
                    child.Material.value.set_uniform("Roughness", 0.6)
                    child.Material.value.set_uniform("Metalness", 0.0)
                    #Render this node as wireframe based on triangle outlines
                    child.Material.value.EnableWireframeRendering.value = True
                if "Transparent_Silver" in child.Material.value.ShaderName.value:
                    #note that, at this point, the transparency has to be set manually when
                    #the rgb values are touched, because it is encoded as the fourth color channel
                    child.Material.value.set_uniform("Color",
                                       avango.gua.Vec4(0.972, 0.960, 0.915, 0.7))
                    child.Material.value.set_uniform("Roughness", 0.3)
                    child.Material.value.set_uniform("Metalness", 1.0)
                if "Plastic_Green" in child.Material.value.ShaderName.value:
                    #for this material, the color and transparency values of the material
                    #file are kept, only the roughness is changed in order to give it a
                    #shiny look
                    child.Material.value.set_uniform("Roughness", 0.2)

                #the transparent white color is parsed from the object file as an example.
                #note that this does not properly translate to PBR materials!

            set_graph_pbr_materials_based_on_name(child)

def start():
    # setup scenegraph
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    loader = avango.gua.nodes.TriMeshLoader()

    abstract_model = loader.create_geometry_from_file(
        "abstract_model", "data/objects/sphere_hierarchy.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE | avango.gua.LoaderFlags.LOAD_MATERIALS)

    monkey1 = loader.create_geometry_from_file(
            "monkey1", "data/objects/monkey.obj",
            avango.gua.LoaderFlags.NORMALIZE_SCALE)

    monkey2 = loader.create_geometry_from_file(
            "monkey2", "data/objects/monkey.obj",
            avango.gua.LoaderFlags.NORMALIZE_SCALE)

    #hardcode monkey1 & monkey2 material, because there is no associated material files
    #the material names will both be "gua_default_material"

    #metalish blue
    monkey1.Material.value.set_uniform("Color",
                                       avango.gua.Vec4(0.662, 0.655, 0.784, 1))
    monkey1.Material.value.set_uniform("Roughness", 0.3)
    monkey1.Material.value.set_uniform("Metalness", 1.0)

    #rather matte red
    monkey2.Material.value.set_uniform("Color",
                                       avango.gua.Vec4(1.0, 0.266, 0.136, 1.0))
    monkey2.Material.value.set_uniform("Roughness", 0.7)
    monkey2.Material.value.set_uniform("Metalness", 0.0)


    transform1_additional_scale = avango.gua.nodes.TransformNode(Transform=avango.gua.make_scale_mat(0.66),
                                                Children=[abstract_model])

    transform1 = avango.gua.nodes.TransformNode(Children=[transform1_additional_scale])

    transform2 = avango.gua.nodes.TransformNode(
        Transform=avango.gua.make_trans_mat(-0.66, 0.0, 0.0) * 
                  avango.gua.make_scale_mat(0.66),
        Children=[monkey1])

    wireframe_transform = avango.gua.nodes.TransformNode(
        Transform=avango.gua.make_trans_mat( 0.66, 0.0, 0.0) * 
                  avango.gua.make_scale_mat(0.66),
        Children=[monkey2])

    light = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="light",
        Color=avango.gua.Color(1.0, 1.0, 1.0),
        Brightness=100.0,
        Transform=(avango.gua.make_trans_mat(1, 1, 5) *
                   avango.gua.make_scale_mat(30, 30, 30)))

    size = avango.gua.Vec2ui(1024, 768)

    window = avango.gua.nodes.GlfwWindow(Size=size, LeftResolution=size)

    avango.gua.register_window("window", window)

    cam = avango.gua.nodes.CameraNode(
        LeftScreenPath="/screen",
        SceneGraph="scenegraph",
        Resolution=size,
        OutputWindowName="window",
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

    cam.PipelineDescription.value = pipeline_description

    cam.PipelineDescription.value.EnableABuffer.value = True

    screen = avango.gua.nodes.ScreenNode(Name="screen",
                                         Width=2,
                                         Height=1.5,
                                         Children=[cam])

    graph.Root.value.Children.value = [transform1, transform2, wireframe_transform, light, screen]

    #setup viewer
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    monkey_updater = TimedRotate()

    timer = avango.nodes.TimeSensor()
    monkey_updater.TimeIn.connect_from(timer.Time)

    #traverse the graph and set some known pbr coefficients for selected materials
    set_graph_pbr_materials_based_on_name(graph.get_node("/"))

    transform1.Transform.connect_from(monkey_updater.MatrixOut)
    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    pipeline_description.touch()
    viewer.run()


if __name__ == '__main__':
    start()
