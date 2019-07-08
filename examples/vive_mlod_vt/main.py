#!/usr/bin/python3

## import avango-guacamole libraries
import avango
import avango.gua
import avango.gua.lod
import avango.script
import avango.vive

class ButtonReader(avango.script.Script):

    def __init__(self):
        self.super(ButtonReader).__init__()

    def assign_window(self, VIVE_WINDOW):
        self.window = VIVE_WINDOW
        self.latest_trigger_value = 0.0
        self.always_evaluate(True)

    def evaluate(self):
        if self.window.Controller0TriggerButtonPressed.value:
            print("Controller 0 Pressed: TRIGGER_BUTTON")

        if self.window.Controller0TriggerValue.value != self.latest_trigger_value:
            self.latest_trigger_value = self.window.Controller0TriggerValue.value
            print("Controller 0 Trigger Value Changed: " + str(self.latest_trigger_value))


def start():
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    mesh_loader = avango.gua.nodes.TriMeshLoader()

    transform = avango.gua.nodes.TransformNode(Name="transform")
    graph.Root.value.Children.value.append(transform)
    
    # configure lod backend
    lod_loader = avango.gua.lod.nodes.LodLoader()
    lod_loader.OutOfCoreBudget.value = 16384
    lod_loader.RenderBudget.value = 512
    lod_loader.UploadBudget.value = 8
	
    vt_mat = avango.gua.create_material(avango.gua.MaterialCapabilities.ROUGHNESS_VALUE |
                                        avango.gua.MaterialCapabilities.METALNESS_VALUE |
                                        avango.gua.MaterialCapabilities.EMISSIVITY_VALUE)
	
    # vt_mat.set_uniform("Color", avango.gua.Vec4(1.0, 0.0, 0.0, 1.0))
    vt_mat.set_uniform("Emissivity", 0.75)
    vt_mat.set_uniform("Roughness", 0.75)
    vt_mat.set_uniform("Metalness", 0.25)
    vt_mat.set_uniform("vt_test", "C://Data\Repositories/MESHLOD/low_resolution_meshlod/Erfurt_Obelisk_low_fix_texture.atlas")
    vt_mat.EnableBackfaceCulling.value = True
    vt_mat.EnableVirtualTexturing.value = True

    mlod_node = lod_loader.load_lod_trimesh("mlod_node_slot", "C://Data/Repositories/MESHLOD/low_resolution_meshlod/Erfurt_Obelisk_low_fix.bvh", vt_mat, avango.gua.lod.LoaderFlags.NORMALIZE_SCALE | avango.gua.lod.LoaderFlags.NORMALIZE_POSITION)
    
    transform.Transform.value = avango.gua.make_trans_mat(0, 1, 0) * avango.gua.make_rot_mat(-90, 1, 0, 0)
    
	# C:\Data\Repositories\MESHLOD\low_resolution_meshlod\Erfurt_Obelisk_low_fix.bvh

    # if not isinstance(mlod_node, avango.gua.lod.MLodNode):
    #    print(str(type(mlod_node)) + " is not an instance of a M-LOD Node")
    #    return

    mlod_node.ErrorThreshold.Value = 1.0
    mlod_node.Material.Value = vt_mat
	
    transform.Children.value.append(mlod_node)
    mlod_node.ShadowMode.value = 1
	
    monkey = mesh_loader.create_geometry_from_file("monkey", "data/objects/monkey.obj", avango.gua.LoaderFlags.NORMALIZE_SCALE | avango.gua.LoaderFlags.NORMALIZE_POSITION)
    monkey.Transform.value = avango.gua.make_trans_mat(-0.5, 1.0, 0.0) * avango.gua.make_scale_mat(0.2)
    monkey.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.0, 0.0, 1))
    monkey.Material.value.set_uniform("Roughness", 0.2)
    monkey.Material.value.set_uniform("Metalness", 1.0)
	
    graph.Root.value.Children.value.append(monkey)
    monkey.ShadowMode.value = 1

    spot_light_1 = avango.gua.nodes.LightNode(Name="spot_light_1",
                                              Type=avango.gua.LightType.SPOT,
                                              Color=avango.gua.Color(1.0, 1.0, 1.0),
                                              EnableShadows=False,
                                              ShadowMapSize=1024,
                                              ShadowOffset=0.002,
                                              ShadowMaxDistance=10,
                                              Falloff=1.5,
                                              ShadowNearClippingInSunDirection=0.1,
                                              ShadowFarClippingInSunDirection=10.0,
                                              Softness=2,
                                              Brightness=20)
    spot_light_1.Transform.value = avango.gua.make_trans_mat(0.0, 3.0, 0.0) * avango.gua.make_rot_mat(-90, 1, 0, 0) * avango.gua.make_scale_mat(4)
    graph.Root.value.Children.value.append(spot_light_1)

    point_light1 = avango.gua.nodes.LightNode(
        Type=avango.gua.LightType.POINT,
        Name="point_light1",
        Color=avango.gua.Color(0.2, 1.0, 1.7),
        EnableShadows=False,
        ShadowMapSize=1024,
        ShadowMaxDistance=10,
        ShadowOffset=0.03,
        Falloff=0.5,
        ShadowNearClippingInSunDirection=0.1,
        ShadowFarClippingInSunDirection=10.0,
        Brightness=20)

    point_light1.Transform.value = avango.gua.make_trans_mat(1.5, 1.0, 1.5) * avango.gua.make_scale_mat(4)
    graph.Root.value.Children.value.append(point_light1)

    sun_light = avango.gua.nodes.LightNode(Name="sun_light",
                                           Type=avango.gua.LightType.SUN,
                                           Color=avango.gua.Color(1.0, 1.0, 0.7),
                                           EnableShadows=False,
                                           ShadowMapSize=1024,
                                           ShadowOffset=0.0005,
                                           ShadowCascadedSplits=[0.1, 4, 7, 20],
                                           ShadowMaxDistance=30,
                                           ShadowNearClippingInSunDirection=100,
                                           ShadowFarClippingInSunDirection=100,
                                           Brightness=2
                                           )
    sun_light.Transform.value = avango.gua.make_rot_mat(210, 0, 1, 0) * avango.gua.make_rot_mat(-50.0, 1.0, 0.0, 0.0)
    graph.Root.value.Children.value.append(sun_light)

    # vive window
    window = avango.vive.nodes.ViveWindow(Title = "vive_example")
    window.Size.value = window.Resolution.value
    window.EnableVsync.value = False
    window.EnableFullscreen.value = False
    avango.gua.register_window(window.Title.value, window)

    # viewing setup
    navigation_node = avango.gua.nodes.TransformNode(Name = "navigation")
    graph.Root.value.Children.value.append(navigation_node)

    controller0 = mesh_loader.create_geometry_from_file("controller0", "data/objects/vive_controller/vive_controller.obj", avango.gua.LoaderFlags.LOAD_MATERIALS)
    controller1 = mesh_loader.create_geometry_from_file("controller1", "data/objects/vive_controller/vive_controller.obj", avango.gua.LoaderFlags.LOAD_MATERIALS)
    base0 = mesh_loader.create_geometry_from_file("base0", "data/objects/vive_lighthouse/vive_lighthouse.obj", avango.gua.LoaderFlags.LOAD_MATERIALS)
    base1 = mesh_loader.create_geometry_from_file("base1", "data/objects/vive_lighthouse/vive_lighthouse.obj", avango.gua.LoaderFlags.LOAD_MATERIALS)
    controller0.Transform.connect_from(window.Controller0SensorOrientation)
    controller1.Transform.connect_from(window.Controller1SensorOrientation)
    base0.Transform.connect_from(window.TrackingReference0SensorOrientation)
    base1.Transform.connect_from(window.TrackingReference1SensorOrientation)
    navigation_node.Children.value = [controller0, controller1, base0, base1]

    head_node = avango.gua.nodes.TransformNode(Name = "head")
    head_node.Transform.connect_from(window.HMDSensorOrientation)
    navigation_node.Children.value.append(head_node)

    left_screen_node = avango.gua.nodes.ScreenNode(Name = "left_screen_node",
                                                   Width = window.LeftScreenSize.value.x,
                                                   Height = window.LeftScreenSize.value.y,
                                                   Transform = avango.gua.make_trans_mat(window.LeftScreenTranslation.value))
    head_node.Children.value.append(left_screen_node)

    right_screen_node = avango.gua.nodes.ScreenNode(Name = "right_screen_node",
                                                    Width = window.RightScreenSize.value.x,
                                                    Height = window.RightScreenSize.value.y,
                                                    Transform = avango.gua.make_trans_mat(window.RightScreenTranslation.value))
    head_node.Children.value.append(right_screen_node)

    camera_node = avango.gua.nodes.CameraNode(Name = "camera_node",
                                              LeftScreenPath = left_screen_node.Path.value,
                                              RightScreenPath = right_screen_node.Path.value,
                                              SceneGraph = graph.Name.value,
                                              Resolution = window.Resolution.value,
                                              OutputWindowName = window.Title.value,
                                              EyeDistance = window.EyeDistance.value,
                                              EnableStereo = True)
    head_node.Children.value.append(camera_node)
	
    vt_backend = avango.gua.VTBackend()
    vt_backend.add_camera(camera_node)
    vt_backend.start_backend()

    # pipeline
    resolve_pass = avango.gua.nodes.ResolvePassDescription()
    resolve_pass.BackgroundMode.value = avango.gua.BackgroundMode.COLOR
    resolve_pass.BackgroundColor.value = avango.gua.Color(0.2, 0.2, 0.2)
	
    mlod_pass = avango.gua.lod.nodes.MLodPassDescription()
    
    pipeline_description = avango.gua.nodes.PipelineDescription(
        Passes=[
            mlod_pass,
            avango.gua.nodes.TriMeshPassDescription(),
            avango.gua.nodes.LightVisibilityPassDescription(),
            resolve_pass,
            avango.gua.nodes.SSAAPassDescription(),
            #avango.gua.nodes.DebugViewPassDescription()
        ],
        EnableABuffer=False
    )

    camera_node.PipelineDescription.value = pipeline_description
	
    camera_node.EnableFrustumCulling.value = True

    # button reader
    button_reader = ButtonReader()
    button_reader.assign_window(window)

    # start application/render loop
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]
    viewer.DesiredFPS.value = 500.0
    viewer.run()


if __name__ == '__main__':
    start()
