#!/usr/bin/python3

## import avango-guacamole libraries
import avango
import avango.gua
import avango.script
import avango.vive
import logging

def print_graph(root_node):
    stack = [(root_node, 0)]
    while stack:
        node, level = stack.pop()
        print("│   " * level + "├── {0} <{1}>".format(
                     node.Name.value, node.__class__.__name__))
        stack.extend([(child, level + 1) for child in 
                       reversed(node.Children.value)])

def start():
    
    
    # setup scene
    graph = avango.gua.nodes.SceneGraph(Name = "main_scenegraph")

    trimesh_loader = avango.gua.nodes.TriMeshLoader()
    
    monkey = trimesh_loader.create_geometry_from_file("monkey", "data/objects/monkey.obj")
    
    monkey.Transform.value = avango.gua.make_trans_mat(0.0, 1.0, 0.0) * avango.gua.make_scale_mat(0.5)
    monkey.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.0, 0.0, 1))
    monkey.Material.value.set_uniform("Roughness", 0.2)
    monkey.Material.value.set_uniform("Metalness", 1.0)
    
    graph.Root.value.Children.value.append(monkey)


    light = avango.gua.nodes.LightNode(Type = avango.gua.LightType.POINT, Brightness = 150.0)
    light.Transform.value = avango.gua.make_trans_mat(-3.0, 5.0, 5.0) * avango.gua.make_scale_mat(12)
    graph.Root.value.Children.value.append(light)




    # vive window
    window = avango.vive.nodes.ViveWindow(Title = "main_window")
    window.Size.value = window.Resolution.value
    window.EnableVsync.value = False
    window.EnableFullscreen.value = False
    avango.gua.register_window(window.Title.value, window)

    # viewing setup
    navigation_node = avango.gua.nodes.TransformNode(Name = "nav")
    graph.Root.value.Children.value.append(navigation_node)

    left_screen_node = avango.gua.nodes.ScreenNode(Name = "left_screen",
                                                   Width = window.LeftScreenSize.value.x,
                                                   Height = window.LeftScreenSize.value.y,
                                                   Transform = avango.gua.make_trans_mat(window.LeftScreenTranslation.value))

    right_screen_node = avango.gua.nodes.ScreenNode(Name = "right_screen",
                                                    Width = window.RightScreenSize.value.x,
                                                    Height = window.RightScreenSize.value.y,
                                                    Transform = avango.gua.make_trans_mat(window.RightScreenTranslation.value))

    camera_node = avango.gua.nodes.CameraNode(Name = "Vive-HMD-User_0",
                                              LeftScreenPath = '/nav/Vive-HMD-User_0/left_screen',
                                              RightScreenPath = '/nav/Vive-HMD-User_0/right_screen',
                                              SceneGraph = graph.Name.value,
                                              Resolution = window.Resolution.value,
                                              OutputWindowName = window.Title.value,
                                              EyeDistance = window.EyeDistance.value,
                                              EnableStereo = True)

    camera_node.Children.value.append(left_screen_node)
    camera_node.Children.value.append(right_screen_node)
    navigation_node.Children.value.append(camera_node)

    vive_controller_0 = trimesh_loader.create_geometry_from_file("Vive-HMD-Controller_0", "data/objects/vive_controller/vive_controller.obj")
    window.register_node(vive_controller_0, 0)
    navigation_node.Children.value.append(vive_controller_0)

    vive_controller_1 = trimesh_loader.create_geometry_from_file("Vive-HMD-Controller_1", "data/objects/vive_controller/vive_controller.obj")
    window.register_node(vive_controller_1, 1)
    navigation_node.Children.value.append(vive_controller_1)

    lighthose_station_0 = trimesh_loader.create_geometry_from_file("Vive-HMD-Lighthouse_0", "data/objects/vive_lighthouse/vive_lighthouse.obj")
    window.register_node(lighthose_station_0, 2)
    navigation_node.Children.value.append(lighthose_station_0)

    lighthose_station_1 = trimesh_loader.create_geometry_from_file("Vive-HMD-Lighthouse_1", "data/objects/vive_lighthouse/vive_lighthouse.obj")
    window.register_node(lighthose_station_1, 3)
    navigation_node.Children.value.append(lighthose_station_1)


    monkey2 = trimesh_loader.create_geometry_from_file("monke2", "data/objects/monkey.obj")
    monkey2.Transform.value = avango.gua.make_scale_mat(0.1)
    monkey2.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.0, 0.0, 1))
    monkey2.Material.value.set_uniform("Roughness", 0.2)
    monkey2.Material.value.set_uniform("Metalness", 1.0)
    vive_controller_0.Children.value.append(monkey2)

    monkey3 = trimesh_loader.create_geometry_from_file("monke2", "data/objects/monkey.obj")
    monkey3.Transform.value = avango.gua.make_scale_mat(0.1)
    monkey3.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.0, 0.0, 1))
    monkey3.Material.value.set_uniform("Roughness", 0.2)
    monkey3.Material.value.set_uniform("Metalness", 1.0)
    test_trans = avango.gua.nodes.TransformNode(Name = "test_trans")
    test_trans.Transform.value = avango.gua.make_trans_mat(0,0.2,0)
    vive_controller_1.Children.value.append(test_trans)
    test_trans.Children.value.append(monkey3)

    # pipeline
    resolve_pass = avango.gua.nodes.ResolvePassDescription()
    resolve_pass.BackgroundMode.value = avango.gua.BackgroundMode.SKYMAP_TEXTURE
    resolve_pass.BackgroundTexture.value = "data/textures/skymap.jpg"
    resolve_pass.Exposure.value = 1.0
    pipeline_description = avango.gua.nodes.PipelineDescription(Passes = [])
    pipeline_description.Passes.value.append(avango.gua.nodes.TriMeshPassDescription())
    pipeline_description.Passes.value.append(avango.gua.nodes.LightVisibilityPassDescription())
    pipeline_description.Passes.value.append(resolve_pass)
    pipeline_description.Passes.value.append(avango.gua.nodes.SSAAPassDescription())

    camera_node.PipelineDescription.value = pipeline_description

    print_graph(graph.Root.value)

    # start application/render loop
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]
    viewer.DesiredFPS.value = 200.0
    viewer.run()


if __name__ == '__main__':
    start()
