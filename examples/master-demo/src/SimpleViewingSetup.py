#!/usr/bin/python

### import guacamole libraries
import avango
import avango.gua
import avango.daemon

### import application libraries
from lib.GuaVE import GuaVE
#from lib.FPSGui import FPSGui


class SimpleViewingSetup:

    ### constructor
    def __init__(self,
        SCENEGRAPH = None,
        WINDOW_RESOLUTION = avango.gua.Vec2ui(2560, 1440), # default resolution in pixel (DELL monitor)
        SCREEN_DIMENSIONS = avango.gua.Vec2(0.595, 0.335), # default screen size in meter (DELL monitor)
        SCREEN_MATRIX = avango.gua.make_identity_mat(),
        STEREO_MODE = "mono",
        HEADTRACKING_SENSOR_STATION = "",
        TRACKING_TRANSMITTER_OFFSET = avango.gua.make_identity_mat(),
        ):


        ## parameter quards
        if SCENEGRAPH is None:
            print("ERROR: scengraph instance missing")            
            quit()


        ### parameters ###
        self.window_resolution = WINDOW_RESOLUTION
        self.screen_dimensions = SCREEN_DIMENSIONS
        self.screen_mat = SCREEN_MATRIX


        ### resources ###
        
        self.shell = GuaVE()

        ## init window
        self.window = avango.gua.nodes.GlfwWindow(Title = "window")
        self.window.EnableVsync.value = False
        #self.window.EnableFullscreen.value = True
        self.window.Size.value = self.window_resolution
        self.window.LeftResolution.value = self.window_resolution
        
        avango.gua.register_window(self.window.Title.value, self.window) 


        ## init viewer
        self.viewer = avango.gua.nodes.Viewer()
        self.viewer.SceneGraphs.value = [SCENEGRAPH]
        self.viewer.Windows.value = [self.window]
        self.viewer.DesiredFPS.value = 200.0 # in Hz


        ## init passes & render pipeline description
        self.resolve_pass = avango.gua.nodes.ResolvePassDescription()
        self.resolve_pass.EnableSSAO.value = True
        self.resolve_pass.SSAOIntensity.value = 3.0
        self.resolve_pass.SSAOFalloff.value = 10.0
        self.resolve_pass.SSAORadius.value = 2.0
        #self.resolve_pass.EnableScreenSpaceShadow.value = True
        self.resolve_pass.EnvironmentLightingColor.value = avango.gua.Color(0.2, 0.2, 0.2)
        self.resolve_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
        self.resolve_pass.Exposure.value = 1.0

        #self.resolve_pass.BackgroundMode.value = avango.gua.BackgroundMode.COLOR
        #self.resolve_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)
        self.resolve_pass.BackgroundMode.value = avango.gua.BackgroundMode.SKYMAP_TEXTURE
        self.resolve_pass.BackgroundTexture.value = "/opt/guacamole/resources/skymaps/DH216SN.png"
        #self.resolve_pass.BackgroundTexture.value = "/opt/guacamole/resources/skymaps/warehouse.jpg"

        self.pipeline_description = avango.gua.nodes.PipelineDescription(Passes = [])
        self.pipeline_description.EnableABuffer.value = False
        self.pipeline_description.Passes.value.append(avango.gua.nodes.TriMeshPassDescription())
        #self.pipeline_description.Passes.value.append(avango.gua.nodes.TexturedQuadPassDescription())
        self.pipeline_description.Passes.value.append(avango.gua.nodes.Video3DPassDescription())
        self.pipeline_description.Passes.value.append(avango.gua.nodes.LightVisibilityPassDescription())
        #self.pipeline_description.Passes.value.append(avango.gua.nodes.BBoxPassDescription())
        self.pipeline_description.Passes.value.append(self.resolve_pass)
        self.pipeline_description.Passes.value.append(avango.gua.nodes.TexturedScreenSpaceQuadPassDescription())               
        self.pipeline_description.Passes.value.append(avango.gua.nodes.SSAAPassDescription())
        self.pipeline_description.EnableABuffer.value = True # enable transparency support
        

        ## init navigation node
        self.navigation_node = avango.gua.nodes.TransformNode(Name = "navigation_node")
        SCENEGRAPH.Root.value.Children.value.append(self.navigation_node)
        
        ## init head node
        self.head_node = avango.gua.nodes.TransformNode(Name = "head_node")
        self.head_node.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0.6)
        self.navigation_node.Children.value.append(self.head_node)

        if HEADTRACKING_SENSOR_STATION != "": # headtracking station defined
            self.headtracking_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
            self.headtracking_sensor.Station.value = HEADTRACKING_SENSOR_STATION
            self.headtracking_sensor.TransmitterOffset.value = TRACKING_TRANSMITTER_OFFSET
            self.headtracking_sensor.ReceiverOffset.value = avango.gua.make_identity_mat()

            self.head_node.Transform.connect_from(self.headtracking_sensor.Matrix)


        ## init screen node
        self.screen_node = avango.gua.nodes.ScreenNode(Name = "screen_node")
        self.screen_node.Width.value = self.screen_dimensions.x
        self.screen_node.Height.value = self.screen_dimensions.y
        self.screen_node.Transform.value = self.screen_mat
        self.navigation_node.Children.value.append(self.screen_node)
        

        ## init camera node
        self.camera_node = avango.gua.nodes.CameraNode(Name = "camera_node")
        self.camera_node.SceneGraph.value = SCENEGRAPH.Name.value
        self.camera_node.LeftScreenPath.value = self.screen_node.Path.value
        self.camera_node.NearClip.value = 0.1 # in meter
        self.camera_node.FarClip.value = 100.0 # in meter
        self.camera_node.Resolution.value = self.window_resolution
        self.camera_node.OutputWindowName.value = self.window.Title.value
        self.camera_node.PipelineDescription.value = self.pipeline_description
        self.head_node.Children.value = [self.camera_node]


        if STEREO_MODE == "anaglyph":
            self.camera_node.EnableStereo.value = True
            self.camera_node.RightScreenPath.value = self.screen_node.Path.value

            self.window.StereoMode.value = avango.gua.StereoMode.ANAGLYPH_RED_CYAN
            self.window.RightResolution.value = self.window_resolution

            self.set_eye_distance(0.064)

        elif STEREO_MODE == "checkerboard":
            self.camera_node.EnableStereo.value = True
            self.camera_node.RightScreenPath.value = self.screen_node.Path.value

            self.window.StereoMode.value = avango.gua.StereoMode.CHECKERBOARD
            self.window.RightResolution.value = self.window_resolution

            self.set_eye_distance(0.064)


        '''
        ## intit FPS gui sub-class
        self.fpsGUI = FPSGui( PARENT_NODE = self.screen_node
                            , WINDOW = self.window
                            , VIEWER = self.viewer
                            )
        '''


    ### functions ###
    def set_eye_distance(self, FLOAT):
        self.camera_node.EyeDistance.value = FLOAT


    def run(self, LOCALS, GLOBALS):
        self.shell.start(LOCALS, GLOBALS)
        self.viewer.run()


    def list_variabels(self):
        self.shell.list_variables()


    def get_head_position(self): # get relative head position (towards screen)
        return self.head_node.Transform.value.get_translate()


