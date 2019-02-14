import sys
import avango
import avango.gua
import avango.gua.nrp
import avango.gua.lod
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


def start(filename):

    # setup scenegraph
    graph = nodes.SceneGraph(Name="scenegraph")

    nrp_root = nrp_nodes.NRPNode()
    nrp_root.Transform.value = make_trans_mat(-3.6, -1.05, -6.0)
    nrp_root.Name.value = "nrp_root"

    nrp_interactive = nrp_nodes.NRPInteractiveNode()
    nrp_interactive.Name.value = "interactive_transform"

    loader = nodes.TriMeshLoader()

    wrapper = nodes.TransformNode(Name="wrapper")
    baseball = loader.create_geometry_from_file(
        "interactive_baseball_2",
        "data/objects/sphere.obj",
        avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE
    )
    baseball.Material.value.set_uniform("Color", avango.gua.Vec4(1., 0., 0., 1.0))
    baseball.Material.value.set_uniform("Roughness", 0.6)
    baseball.Material.value.set_uniform("Metalness", 0.1)
    baseball.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.0) * avango.gua.make_scale_mat(0.1, 0.1, 0.1)
    wrapper.Children.value.append(baseball)
    nrp_interactive.Children.value.append(wrapper)

    size = Vec2ui(2560, 1440)

    window = nodes.GlfwWindow(Title="HBP NRP @ BUW: Distribution Client", Size=size, LeftResolution=size)

    register_window("client_window", window)

    logger = avango.gua.nodes.Logger(EnableWarning=False)

    nettrans = avango.gua.nodes.NetTransform(Name="net", Groupname="AVCLIENT|127.0.0.1|7432")

    graph.Root.value.Children.value = [nettrans, nrp_root, nrp_interactive]

    navigator = examples_common.navigator.Navigator()
    navigator.StartLocation.value = wrapper.Transform.value.get_translate()
    navigator.OutTransform.connect_from(wrapper.Transform)

    navigator.RotationSpeed.value = 0.2
    navigator.MotionSpeed.value = 0.04

    wrapper.Transform.connect_from(navigator.OutTransform)

    viewer = nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()


if __name__ == '__main__':
    start(sys.argv[1])
