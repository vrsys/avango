import avango
import avango.gua
import avango.gua.nrp
import avango.script
from avango.script import field_has_changed
from avango.gua import *
from avango.gua.nrp import *

import examples_common.navigator
from examples_common.GuaVE import GuaVE


## print the subgraph under a given node to the console
def print_graph(root_node):
    stack = [(root_node, 0)]
    while stack:
        node, level = stack.pop()
        print("│   " * level + "├── {0} <{1}>".format(
            node.Name.value, node.__class__.__name__))
        stack.extend(
            [(child, level + 1) for child in reversed(node.Children.value)])


## print all fields of a field container to the console
def print_fields(node, print_values=False):
    for i in range(node.get_num_fields()):
        field = node.get_field(i)
        print("→ {0} <{1}>".format(field._get_name(), field.__class__.__name__))
        if print_values:
            print("  with value '{0}'".format(field.value))


class Picker(avango.script.Script):
    SceneGraph = avango.gua.SFSceneGraph()
    Ray = avango.gua.SFRayNode()
    Options = avango.SFInt()
    BlackList = []
    WhiteList = []
    Results = avango.gua.MFPickResult()

    def __init__(self):
        self.super(Picker).__init__()
        self.always_evaluate(True)

        self.SceneGraph.value = avango.gua.nodes.SceneGraph()
        self.Ray.value = avango.gua.nodes.RayNode()
        self.Options.value = avango.gua.PickingOptions.GET_TEXTURE_COORDS \
                             | avango.gua.PickingOptions.GET_WORLD_NORMALS \
                             | avango.gua.PickingOptions.INTERPOLATE_NORMALS \
                             | avango.gua.PickingOptions.PICK_ONLY_FIRST_FACE

    def evaluate(self):
        results = self.SceneGraph.value.ray_test(self.Ray.value,
                                                 self.Options.value,
                                                 self.BlackList,
                                                 self.WhiteList)
        self.Results.value = results.value


class MaterialUpdater(avango.script.Script):
    PickedNodes = avango.gua.MFPickResult()
    OldNodes = avango.gua.MFPickResult()

    @field_has_changed(PickedNodes)
    def update_materials(self):

        # print("Picked: ", len(self.PickedNodes.value))

        # for i in range(0, len(self.PickedNodes.value)):
        #    print(type(self.PickedNodes.value[i].Object.value))

        for i in range(0, len(self.OldNodes.value)):
            if isinstance(self.OldNodes.value[i].Object.value, avango.gua.TriMeshNode):
                self.OldNodes.value[i].Object.value.Material.value.set_uniform("Color", avango.gua.Vec4(1, 1, 1, 1))

        for i in range(0, len(self.PickedNodes.value)):
            if isinstance(self.PickedNodes.value[i].Object.value, avango.gua.TriMeshNode):
                self.PickedNodes.value[i].Object.value.Material.value.set_uniform("Color", avango.gua.Vec4(1, 0, 0, 0.5))

        self.OldNodes.value = self.PickedNodes.value


def start():
    # setup scenegraph
    graph = nodes.SceneGraph(Name="scenegraph")

    nrp_root = nrp_nodes.NRPNode()
    nrp_root.Transform.value = make_trans_mat(-3.6, -1.05, -6.0)
    nrp_root.Name.value = "nrp_root"

    # TODO: showcase interactivity

    pick_ray = nodes.RayNode(Name="pick_ray")
    pick_ray.Transform.value = avango.gua.make_trans_mat(0.0, -1.0, 0.0) * \
                                avango.gua.make_scale_mat(1.0, 1.0, 50.0)

    loader = nodes.TriMeshLoader()

    ray_geom = loader.create_geometry_from_file(
        "ray_geom",
        "data/objects/cylinder.obj",
        avango.gua.LoaderFlags.DEFAULTS
    )

    ray_geom.Transform.value = avango.gua.make_scale_mat(0.1, 0.1, 100)
    pick_ray.Children.value.append(ray_geom)

    picker = Picker()
    picker.SceneGraph.value = graph
    picker.Ray.value = pick_ray

    material_updater = MaterialUpdater()
    material_updater.PickedNodes.connect_from(picker.Results)

    size = Vec2ui(2560, 1440)

    window = nodes.GlfwWindow(Size=size, LeftResolution=size)

    register_window("window", window)

    res_pass = nodes.ResolvePassDescription()

    res_pass.EnvironmentLightingColor.value = Color(0.1, 0.1, 0.1)
    res_pass.ToneMappingMode.value = ToneMappingMode.UNCHARTED
    res_pass.Exposure.value = 1.0
    res_pass.BackgroundColor.value = Color(0.45, 0.5, 0.6)

    screen = nodes.ScreenNode(Name="screen", Width=4.8, Height=2.7)
    screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

    cam = nrp_nodes.NRPCameraNode()
    cam.Name.value = "cam"

    cam.PipelineDescription.value = nodes.PipelineDescription(Passes=[nrp_nodes.NRPPassDescription(),
                                                                      nodes.TriMeshPassDescription(),
                                                                      nodes.LightVisibilityPassDescription(),
                                                                      res_pass,
                                                                      nodes.SSAAPassDescription()])

    cam.LeftScreenPath.value = "/cam/screen"
    cam.SceneGraph.value = "scenegraph"
    cam.Resolution.value = size
    cam.OutputWindowName.value = "window"
    cam.Transform.value = make_trans_mat(0., 0., 2.0)

    cam.Children.value.append(screen)
    cam.Children.value.append(pick_ray)

    cam.PipelineDescription.value.EnableABuffer.value = True

    graph.Root.value.Children.value = [nrp_root, cam]

    navigator = examples_common.navigator.Navigator()
    navigator.StartLocation.value = cam.Transform.value.get_translate()
    navigator.OutTransform.connect_from(cam.Transform)

    navigator.RotationSpeed.value = 0.2
    navigator.MotionSpeed.value = 0.04

    cam.Transform.connect_from(navigator.OutTransform)

    viewer = nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    cam.PipelineDescription.value.touch()
    viewer.run()


if __name__ == '__main__':
    start()
