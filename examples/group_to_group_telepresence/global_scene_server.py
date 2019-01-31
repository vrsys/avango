# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################
'''
A distributed viewer setup: This Python scripts starts an
avango.osg.simpleviewer to load a given geometry. Any client connected to the
group "testgroup" should receive this model. (see also simpleviewer-clnt.py)
'''

import avango
import avango.script
import avango.gua
import avango.gua.lod
import avango.daemon
from avango.script import field_has_changed

import examples_common.navigator
import examples_common.default_views

from examples_common.GuaVE import GuaVE

import netvaluepy
from time import sleep

#avango.enable_logging(4, "server.log")


OBSERVER_MODE = "3_CLIENTS_SIMULATED"
#OBSERVER_MODE = "3_CLIENTS_LIVE"
#OBSERVER_MODE = "VIDEO_CAMERA"

SCENE_MODE = "EVALUATION"
#SCENE_MODE = "SIMPLE"


#USED SERVER IPS
VR16      = "141.54.147.16"
PAN       = "141.54.147.52"
LOCALHOST = "127.0.0.1"
DAEDALOS  = "141.54.147.34"

HEKATE    = "141.54.147.42"

CURRENTLY_USED_SERVER = DAEDALOS

nettrans = avango.gua.nodes.NetTransform(Name="net",
                                         # specify role, ip, and port
                                         Groupname="AVSERVER|"+CURRENTLY_USED_SERVER+"|7432")
                                         #Groupname="AVSERVER|127.0.0.1|7432")
                                         #Groupname="AVSERVER|141.54.147.52|7432") # server -> pan
                                         #Groupname="AVSERVER|141.54.147.54|7432")





class TimedUpdateTrackingInfo(avango.script.Script):
    TimeIn = avango.SFFloat()
    CentralUserMatrixOut = avango.gua.SFMatrix4()

    #CentralUser = avango.gua.SFMatrix4()

    DeviceSensorGlassesA = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
    DeviceSensorGlassesA.Station.value = "tracking-dbl-glasses-A"
    DeviceSensorGlassesA.TransmitterOffset.value = avango.gua.make_trans_mat(0.0,0.045,0.0)



    """
    keyboard_events = 0
    logging_node = 0

    frame_time_dict = dict()

    num_entries = 0

    def set_logging_node(self, ln):
        self.logging_node = ln

    def set_keyboard_events(self, keyboard):
        self.keyboard_events = keyboard
    """
    @field_has_changed(TimeIn)
    def update(self):
        self.CentralUserMatrixOut.value = self.DeviceSensorGlassesA.Matrix.value
        #print("TM: ")
        #print(self.DeviceSensorGlassesA.Matrix.value)
        



class TimedKeyToggling(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    keyboard_events = 0
    logging_node = 0

    frame_time_dict = dict()

    num_entries = 0

    def set_logging_node(self, ln):
        self.logging_node = ln

    def set_keyboard_events(self, keyboard):
        self.keyboard_events = keyboard

    @field_has_changed(TimeIn)
    def update(self):
        if (0 != self.logging_node) and (0 != self.keyboard_events):
            if self.keyboard_events.logging_enabled:
                #print("XXXXXX")
                self.logging_node.Tags.value = ["X"]

                if self.num_entries == 0:
                    self.frame_time_dict[4.13123123] = 0.0
                else:
                    self.frame_time_dict[4.13123123] += 1.1

                self.num_entries = self.num_entries + 1
            else:
                self.logging_node.Tags.value = []

                if self.num_entries != 0:
                    pass
                    #print("FRAMETIME WAS: " + str(self.frame_time_dict[4.13123123]))          
        #self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value * 30.0,
        #                                               0.0, 1.0, 0.0) * avango.gua.make_scale_mat(1.0)


class TimedKeyframePathAnimation(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    walking_speed_units_per_second = 1.666667

    right_offset = 1.0

    indexed_keyframe_positions = [(    0, avango.gua.Vec3(0 + right_offset, 0, -11), 180),
                                  (10690, avango.gua.Vec3(0 + right_offset, 0, 7.2), 180),
                                  (14360, avango.gua.Vec3(0 + right_offset, 0, 7.2), 180),
                                  (17095, avango.gua.Vec3(0 + right_offset, 0, 2.4), 180),
                                  (17695, avango.gua.Vec3(0 + right_offset, 0, 2.4), 180),                                  
                                  (20230, avango.gua.Vec3(-2.2 + right_offset, 0, 2.4), 180),
                                  (26430, avango.gua.Vec3(-2.2 + right_offset, 0, 2.4), 180),
                                  (33000, avango.gua.Vec3(0 + right_offset, 0, 2.4), 180),
                                  (34000, avango.gua.Vec3(0 + right_offset, 0, 2.4), 180),
                                  (35500, avango.gua.Vec3(0 + right_offset, 0, 2.4), 0),
                                  (39000, avango.gua.Vec3(0 + right_offset, 0, -11), 0),
                                  (41000, avango.gua.Vec3(0 + right_offset, 0, -11), 0)
                                  #(0, avango.gua.Vec3(0, 0, -10), 180),
                                  #(7000, avango.gua.Vec3(0, 0, 5),180),

                                  #(15000, avango.gua.Vec3(0, 0, 5),180),
                                  #(18000, avango.gua.Vec3(0, 0.0, 2.4),180 ),
                                  #(20000, avango.gua.Vec3(-1.5, 0.0, 2.4),180 ),
                                  #(30000, avango.gua.Vec3(-1.5, 0.0, 2.4),180 ),
                                  #(2000, avango.gua.Vec3(0, 0.0, 5),180 ),
                                  
                                  #(3000, avango.gua.Vec3(0, 0.0, 5),180 ),
                                  #(4000, avango.gua.Vec3(0, 0.0, 2.4),180 ),
                                  #(5000, avango.gua.Vec3(0, 0.0, 2.4),180 ),
                                  #(6000, avango.gua.Vec3(-1.5, 0, 2.4),180 ),
                                  #(8000, avango.gua.Vec3(-1.5, 0, 2.4), 180 ),
                                  #(9000, avango.gua.Vec3(0, 0, 2.4), 30),
                                  #(10000, avango.gua.Vec3(0, 0, 2.4), 90),
                                  #(11500, avango.gua.Vec3(0, 0, -10), 180 ),
                                  
                                  #(12000, avango.gua.Vec3(0, 0, -10), 180 )
                                ]
    

    animation_length_in_ms = indexed_keyframe_positions[-1][0]

    #nv = netvaluepy.NetValue("127.0.0.1:8000")
    #nv = netvaluepy.NetValue("141.54.147.52:8000") # hier socket passend zu ./play 
    #nv = netvaluepy.NetValue(CURRENTLY_USED_SERVER+":8000")
    nv = netvaluepy.NetValue(HEKATE+":8000")

    @field_has_changed(TimeIn)
    def update(self):
      #  print("NetValue: " + str(self.nv.getValue()) )
        #current_time_point = int(self.TimeIn.value*1000) % self.animation_length_in_ms
        current_time_point = int(self.nv.getValue()*1000) % self.animation_length_in_ms
        #print("Time:" + str(current_time_point) )

        keyframe_tuple_x = 0
        keyframe_tuple_x_plus_1 = 0

        found_first_time_point = False

        current_keyframe_id = 0
        for ikfp in self.indexed_keyframe_positions:
            if not found_first_time_point:
                if current_time_point == ikfp[0]:
                    found_first_time_point = True
                    keyframe_tuple_x = keyframe_tuple_x_plus_1 = ikfp
                elif current_time_point > ikfp[0]:
                    if (len(self.indexed_keyframe_positions) == current_keyframe_id + 1) or (self.indexed_keyframe_positions[current_keyframe_id + 1][0] > current_time_point) :
                        found_first_time_point = True
                        keyframe_tuple_x = ikfp
            else:
                if current_time_point < ikfp[0]:
                    keyframe_tuple_x_plus_1 = ikfp
                    break
            current_keyframe_id = current_keyframe_id + 1

        interpolation_weight = ((current_time_point) - keyframe_tuple_x[0]) / ((keyframe_tuple_x_plus_1[0]) - keyframe_tuple_x[0])
        #print( str(keyframe_tuple_x[0]) + "; " + str(current_time_point) + "; " + str(keyframe_tuple_x_plus_1[0]) )
        #print("interpolation_weight" + str(interpolation_weight))

        interpolated_position = keyframe_tuple_x[1] * (1.0 - interpolation_weight) + keyframe_tuple_x_plus_1[1] * (interpolation_weight)
        curr_pos = interpolated_position

        interpolated_angle = keyframe_tuple_x[2] * (1.0 - interpolation_weight) + keyframe_tuple_x_plus_1[2] * (interpolation_weight)
        curr_angle_around_y = interpolated_angle
        self.MatrixOut.value = avango.gua.make_trans_mat(curr_pos[0], curr_pos[1], curr_pos[2]) * avango.gua.make_rot_mat(curr_angle_around_y, 0.0, 1.0, 0.0)

"""
class FeedbackSender(avango.script.Script):
    AvatarWorldMatrixIn         = avango.gua.SFMatrix4()
    ClientCamCenterMatrixIn     = avango.gua.SFMatrix4()
    ClientCamLeftMatrixIn       = avango.gua.SFMatrix4()
    ClientCamRightMatrixIn      = avango.gua.SFMatrix4()
    ClientScreenMatrixIn        = avango.gua.SFMatrix4()

    AvatarFeedbackWorldMatrix = avango.gua.SFMatrix4()

    CamNodeCenter = 0
    CamNodeLeft   = 0
    CamNodeRight  = 0
    SceneGraph    = 0

    ClientCamCenter_LeftEyeFeedbackViewMatrix = avango.gua.SFMatrix4()
    ClientCamCenter_LeftEyeFeedbackProjectionMatrix = avango.gua.SFMatrix4()
    ClientCamCenter_RightEyeFeedbackViewMatrix = avango.gua.SFMatrix4()
    ClientCamCenter_RightEyeFeedbackProjectionMatrix = avango.gua.SFMatrix4()

    ClientCamLeft_LeftEyeFeedbackViewMatrix = avango.gua.SFMatrix4()
    ClientCamLeft_LeftEyeFeedbackProjectionMatrix = avango.gua.SFMatrix4()
    ClientCamLeft_RightEyeFeedbackViewMatrix = avango.gua.SFMatrix4()
    ClientCamLeft_RightEyeFeedbackProjectionMatrix = avango.gua.SFMatrix4()

    ClientCamRight_LeftEyeFeedbackViewMatrix = avango.gua.SFMatrix4()
    ClientCamRight_LeftEyeFeedbackProjectionMatrix = avango.gua.SFMatrix4()
    ClientCamRight_RightEyeFeedbackViewMatrix = avango.gua.SFMatrix4()
    ClientCamRight_RightEyeFeedbackProjectionMatrix = avango.gua.SFMatrix4()
    

    def registerSceneGraph(self, scene_graph):
        self.SceneGraph = scene_graph

    def registerCamNodeCenter(self, cam_node):
        self.CamNodeCenter = cam_node
    #frame_time_dict = dict()

    #num_entries = 0

    @field_has_changed(AvatarWorldMatrixIn)
    def updateAvatarFeedbackMatrix(self):
        self.AvatarFeedbackWorldMatrix.value = self.AvatarWorldMatrixIn.value

    @field_has_changed(ClientCamCenterMatrixIn)
    def updateClientCamCenterMatrices(self):
        if (0 != self.CamNodeCenter) and (0 != self.SceneGraph)  :
            CentralUser_LeftEyeFrustum = self.CamNodeCenter.get_frustum(self.SceneGraph,  avango.gua.CameraMode.LEFT)
            self.ClientCamCenter_LeftEyeFeedbackViewMatrix.value = CentralUser_LeftEyeFrustum.ViewMatrix.value
            self.ClientCamCenter_LeftEyeFeedbackProjectionMatrix.value = CentralUser_LeftEyeFrustum.ProjectionMatrix.value
            print("View Left")
            print(self.ClientCamCenter_LeftEyeFeedbackViewMatrix.value)
            print("Proj Left")
            print(self.ClientCamCenter_LeftEyeFeedbackProjectionMatrix.value)

            CentralUser_RightEyeFrustum = self.CamNodeCenter.get_frustum(self.SceneGraph,  avango.gua.CameraMode.RIGHT)
            self.ClientCamCenter_RightEyeFeedbackViewMatrix.value = CentralUser_RightEyeFrustum.ViewMatrix.value
            self.ClientCamCenter_RightEyeFeedbackProjectionMatrix.value = CentralUser_RightEyeFrustum.ProjectionMatrix.value
            print("View Right")
            print(self.ClientCamCenter_RightEyeFeedbackViewMatrix.value)
            print("Proj Right")
            print(self.ClientCamCenter_RightEyeFeedbackProjectionMatrix.value)

    @field_has_changed(ClientCamLeftMatrixIn)
    def updateClientCamLeftMatrices(self):
        if (0 != self.CamNodeCenter) and (0 != self.SceneGraph)  :
            CentralUser_LeftEyeFrustum = self.CamNodeCenter.get_frustum(self.SceneGraph,  avango.gua.CameraMode.LEFT)
            self.ClientCamCenter_LeftEyeFeedbackViewMatrix.value = CentralUser_LeftEyeFrustum.ViewMatrix.value
            self.ClientCamCenter_LeftEyeFeedbackProjectionMatrix.value = CentralUser_LeftEyeFrustum.ProjectionMatrix.value
            print("View Left")
            print(self.ClientCamCenter_LeftEyeFeedbackViewMatrix.value)
            print("Proj Left")
            print(self.ClientCamCenter_LeftEyeFeedbackProjectionMatrix.value)

            CentralUser_RightEyeFrustum = self.CamNodeCenter.get_frustum(self.SceneGraph,  avango.gua.CameraMode.RIGHT)
            self.ClientCamCenter_RightEyeFeedbackViewMatrix.value = CentralUser_RightEyeFrustum.ViewMatrix.value
            self.ClientCamCenter_RightEyeFeedbackProjectionMatrix.value = CentralUser_RightEyeFrustum.ProjectionMatrix.value
            print("View Right")
            print(self.ClientCamCenter_RightEyeFeedbackViewMatrix.value)
            print("Proj Right")
            print(self.ClientCamCenter_RightEyeFeedbackProjectionMatrix.value)
    #@field_has_changed(ClientCamCenterMatrixIn)
    #def printMatrices(self):
    #    pass
        #self.AvatarFeedbackWorldMatrix.value = self.AvatarWorldMatrixIn.value
"""
def make_node_distributable(node):
    for child in node.Children.value:
        make_node_distributable(child)
    nettrans.distribute_object(node)


def make_material_distributable(mat):
    nettrans.distribute_object(mat)

# setup scenegraph
graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
loader = avango.gua.nodes.TriMeshLoader()


#kaisersaal = loader.create_geometry_from_file("kaisersaal", "/mnt/data_internal/geometry_data/confidential/Kaisersaal/Ktris_7500/Bam_Kai_o_L_12_ct_0750.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
#kaisersaal = loader.create_geometry_from_file("kaisersaal", "/opt/3d_models/paperHouses/Modelle_Baschdi/Modell/Baum.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
#/opt/3d_models/paperHouses/Modelle_Baschdi/Modell/Baum.obj
#kaisersaal.Transform.value = avango.gua.make_trans_mat(-2.5, 0.0, 1.0) * avango.gua.make_scale_mat(1.0, 1.0, 1.0) #* avango.gua.make_rot_mat(90.0, 0.0, -1.0, 0.0) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)

#kaisersaal = loader.create_geometry_from_file("kaisersaal", "/mnt/data_internal/geometry_data/confidential/Kaisersaal/Ktris_7500/Bam_Kai_o_L_12_ct_0750.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
street_plane = loader.create_geometry_from_file("street_plane", "/home/wabi7015/Programming/avango/examples/group_to_group_telepresence/data/objects/street_plane.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
street_plane.Material.value.set_uniform("Emissivity", 3.0)

def set_hauptgebaeude_emissivity(node):
  children             = node.Children.value
  current_num_children = len(node.Children.value)

  if current_num_children > 0:
    for child in children:
      set_hauptgebaeude_emissivity(child)
  else:
    node.Material.value.set_uniform("Emissivity", 3.0)


if "EVALUATION" == SCENE_MODE:
    hauptgebaeude = loader.create_geometry_from_file("hauptgeb", "/opt/3d_models/architecture/BHU_MainBuilding/BHU_cut_again.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
    hauptgebaeude.Transform.value = avango.gua.make_trans_mat(-15.0, 0.0, -25.0) * avango.gua.make_rot_mat(180.0, 0.0, 1.0, 0.0) * avango.gua.make_scale_mat(1.0, 1.0, 1.0)

    set_hauptgebaeude_emissivity(hauptgebaeude)

    lion = loader.create_geometry_from_file("loewe", "/home/wabi7015/Desktop/250k_hq_texture_loewe_quickfix_3.obj", avango.gua.LoaderFlags.LOAD_MATERIALS)
    lion.Transform.value = avango.gua.make_trans_mat(-1.5, 0.0, 3.0)  * avango.gua.make_scale_mat(1, 1, 1) * avango.gua.make_rot_mat(-90.0, 0.0, 1.0, 0.0) * avango.gua.make_scale_mat(1.3, 1.3, 1.3) * avango.gua.make_scale_mat(1, 1, -1)

    lion.Material.value.EnableBackfaceCulling.value = False
    lion.Material.value.set_uniform("Emissivity", 3.0)

    lion2 = loader.create_geometry_from_file("loewe2", "/home/wabi7015/Desktop/250k_hq_texture_loewe_quickfix_3.obj", avango.gua.LoaderFlags.LOAD_MATERIALS)
    lion2.Transform.value = avango.gua.make_trans_mat(1.5, 0.0, 3.0) * avango.gua.make_rot_mat(-90.0, 0.0, 1.0, 0.0) * avango.gua.make_scale_mat(1.3, 1.3, 1.3) 


mat_desc = avango.gua.nodes.MaterialShaderDescription()
mat_desc.load_from_file("data/materials/SimpleMaterial.gmd")
avango.gua.register_material_shader(mat_desc, "mat")

mat = avango.gua.nodes.Material(ShaderName="mat")

#teapot.Material.value = mat
nettrans.distribute_object(mat)

spointsloader = avango.gua.nodes.SPointsLoader()
#avatar_geode = spointsloader.load("kinect", "/home/wabi7015/Programming/avango/examples/group_to_group_telepresence/spoints_resource_localhost_without_feedback.sr")
#avatar_geode = spointsloader.load("kinect", "/home/wabi7015/Programming/avango/examples/group_to_group_telepresence/spoints_resource_vr16_for_vr16.sr")
#avatar_geode = spointsloader.load("kinect", "/home/wabi7015/Programming/avango/examples/group_to_group_telepresence/spoints_resource_hekate_for_artemis.sr")
#avatar_geode = spointsloader.load("kinect", "/home/wabi7015/Programming/avango/examples/group_to_group_telepresence/spoints_resource_hekate_for_hekate.sr")
avatar_geode = spointsloader.load("kinect", "/home/wabi7015/Programming/avango/examples/group_to_group_telepresence/spoints_resource_hekate_for_argos.sr")

scene_transform = avango.gua.nodes.TransformNode(Name="scene_transform")
scene_transform.Transform.value = avango.gua.make_trans_mat(-1.0, 0.0, 4.3)



avatar_transform = avango.gua.nodes.TransformNode(Name="avatar_transform")
avatar_transform.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0.0)
avatar_transform.Children.value = [avatar_geode]

non_avatar_scene_transform = avango.gua.nodes.TransformNode(Name="non_avatar_scene_transform")
non_avatar_scene_transform.Transform.value = avango.gua.make_trans_mat(1.0, 0.0, 0.0)

if "EVALUATION" == SCENE_MODE:
    non_avatar_scene_transform.Children.value = [street_plane, hauptgebaeude, lion, lion2]
else:
    non_avatar_scene_transform.Children.value = [street_plane]

def append_trees_to_list(parent_node):
    tm_loader = avango.gua.nodes.TriMeshLoader()
    
    depth_offset_pre_tree = 4.0

    tree_label = 0
    for i in range(3):
        tree_to_append = loader.create_geometry_from_file("tree_" + str(tree_label), "/opt/3d_models/paperHouses/Modelle_Baschdi/Modell/Baum.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
        #/opt/3d_models/paperHouses/Modelle_Baschdi/Modell/Baum.obj
        tree_to_append.Transform.value = avango.gua.make_trans_mat(-4.5, 0.0, 1.2 - depth_offset_pre_tree*i) * avango.gua.make_scale_mat(1.0, 1.0, 1.0) #* avango.gua.make_rot_mat(90.0, 0.0, -1.0, 0.0) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
        tree_to_append.Material.value.set_uniform("Emissivity", 3.0)

        #lion.Material.value.set_uniform("Emissivity", 3.0)

        parent_node.Children.value.append(tree_to_append)
        tree_label += 1

    for i in range(3):
        tree_to_append = loader.create_geometry_from_file("tree_" + str(tree_label), "/opt/3d_models/paperHouses/Modelle_Baschdi/Modell/Baum.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
        #/opt/3d_models/paperHouses/Modelle_Baschdi/Modell/Baum.obj
        tree_to_append.Transform.value =  avango.gua.make_trans_mat(4.5, 0.0, 1.2 - depth_offset_pre_tree*i) * avango.gua.make_rot_mat(180.0, 0.0, 1.0, 0.0)#* avango.gua.make_rot_mat(90.0, 0.0, -1.0, 0.0) * avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
        tree_to_append.Material.value.set_uniform("Emissivity", 3.0)

        parent_node.Children.value.append(tree_to_append)
        tree_label += 1


append_trees_to_list(non_avatar_scene_transform)


scene_transform.Children.value = [non_avatar_scene_transform, avatar_transform]

#screen = avango.gua.nodes.ScreenNode(Name="screen", Width=4, Height=3)
#screen = avango.gua.nodes.ScreenNode(Name="screen", Width=0.292, Height=0.22)

#SCREEN_WIDTH  = 4.40
#SCREEN_HEIGHT = 2.70

#screen = avango.gua.nodes.ScreenNode(Name="screen", Width=SCREEN_WIDTH, Height=SCREEN_HEIGHT)


SCREEN_WIDTH  = 4.91
SCREEN_HEIGHT = 2.78

screen = avango.gua.nodes.ScreenNode(Name="screen", Width=SCREEN_WIDTH, Height=SCREEN_HEIGHT)



#size = avango.gua.Vec2ui(491, 278)
#size = avango.gua.Vec2ui(1600, 1200)
#size = avango.gua.Vec2ui(1400, 1600) # vive pro
size = avango.gua.Vec2ui(3840, 2160)
#size = avango.gua.Vec2ui(4096, 2160)

tri_pass = avango.gua.nodes.TriMeshPassDescription()
tquad_pass = avango.gua.nodes.TexturedQuadPassDescription()
lvis_pass = avango.gua.nodes.LightVisibilityPassDescription()
res_pass = avango.gua.nodes.ResolvePassDescription()

res_pass.BackgroundMode.value = avango.gua.BackgroundMode.SKYMAP_TEXTURE
res_pass.BackgroundTexture.value = "/opt/guacamole/resources/skymaps/bright_sky.jpg"

res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
tscreenspace_pass = avango.gua.nodes.TexturedScreenSpaceQuadPassDescription()
spoints_pass_description = avango.gua.nodes.SPointsPassDescription()
occlusion_slave_res_pass = avango.gua.nodes.OcclusionSlaveResolvePassDescription()

pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes=[
        avango.gua.nodes.TriMeshPassDescription(),
        avango.gua.nodes.LightVisibilityPassDescription(),
        avango.gua.nodes.SPointsPassDescription(),
        res_pass
    ])

occlusion_slave_pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes=[
        avango.gua.nodes.TriMeshPassDescription(),
        avango.gua.nodes.LightVisibilityPassDescription(),
        avango.gua.nodes.SPointsPassDescription(),
        occlusion_slave_res_pass
    ])



eye_height = 1.7

camera_translations_right = avango.gua.Vec3( 1.0, eye_height, 2.0)
camera_translations_center = avango.gua.Vec3(0.0, eye_height, 2.0)
camera_translations_left = avango.gua.Vec3(-1.0, eye_height, 2.0)

"""
camera_translations_right = avango.gua.Vec3( 0.0, 1.70, 0.0)
camera_translations_center = avango.gua.Vec3( 0.0, 1.70, 0.0)
camera_translations_left = avango.gua.Vec3( 0.0, 1.70, 0.0)
"""


BASE_USER_GEOM_SCALING = 0.2
USER_GEOM_SCALING = avango.gua.Vec3( BASE_USER_GEOM_SCALING*(1.0/SCREEN_WIDTH), BASE_USER_GEOM_SCALING*(1.0/SCREEN_HEIGHT), BASE_USER_GEOM_SCALING*1.0)

camera_geometry_translations_right = avango.gua.Vec3(camera_translations_right[0]/SCREEN_WIDTH, 0.0, 2.0)
camera_geometry_translations_center = avango.gua.Vec3(0.0/SCREEN_WIDTH, 0.0, 2.0)
camera_geometry_translations_left = avango.gua.Vec3(-1.0/SCREEN_WIDTH, 0.0, 2.0)


central_user_geometry = loader.create_geometry_from_file("central_user_geom", "/home/wabi7015/Programming/avango/examples/group_to_group_telepresence/data/objects/user_position_proxy.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
central_user_geometry.Transform.value = avango.gua.make_trans_mat(camera_geometry_translations_center) * avango.gua.make_scale_mat( USER_GEOM_SCALING )

left_user_geometry = loader.create_geometry_from_file("central_user_geom", "/home/wabi7015/Programming/avango/examples/group_to_group_telepresence/data/objects/user_position_proxy.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
left_user_geometry.Transform.value = avango.gua.make_trans_mat(camera_geometry_translations_left) * avango.gua.make_scale_mat( USER_GEOM_SCALING )

right_user_geometry = loader.create_geometry_from_file("central_user_geom", "/home/wabi7015/Programming/avango/examples/group_to_group_telepresence/data/objects/user_position_proxy.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
right_user_geometry.Transform.value = avango.gua.make_trans_mat(camera_geometry_translations_right) * avango.gua.make_scale_mat( USER_GEOM_SCALING )


screen_geometry = loader.create_geometry_from_file("screen_geometry", "/home/wabi7015/Programming/avango/examples/group_to_group_telepresence/data/objects/screen.obj", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.LOAD_MATERIALS)
screen_geometry.Transform.value = avango.gua.make_trans_mat(0.0, eye_height, 8.5) * avango.gua.make_scale_mat(4.4, 2.7, 1.0)
screen_geometry.Children.value = [central_user_geometry, left_user_geometry, right_user_geometry]


screen_path = "/net/grouped_view_setups_and_scene/navigation/screen"

eye_distance = 0.06

if "VIDEO_CAMERA" == OBSERVER_MODE:
    eye_distance = 0.0

server_cam = avango.gua.nodes.CameraNode(
    ViewID=1,
    LeftScreenPath=screen_path,
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="server_window",
    Transform=avango.gua.make_trans_mat(camera_translations_center),
    PipelineDescription=pipeline_description,
    BlackList = ["invisible_osaka_avatar"]
    )


client_cam_center = avango.gua.nodes.CameraNode(
    ViewID=5,
    Name="viewer_0_weimar_center",
    LeftScreenPath=screen_path,
    RightScreenPath=screen_path,
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="client_window_weimar_center",
    Transform=avango.gua.make_trans_mat(camera_translations_center),
    PipelineDescription=pipeline_description,

    EyeDistance = eye_distance,
    EnableStereo = True,
    )

occlusion_slave_client_cam_center = avango.gua.nodes.CameraNode(
    ViewID=5,
    Name="os_weimar_v0_osaka_center",
    LeftScreenPath=screen_path,
    RightScreenPath=screen_path,
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="slave_weimar_v0_osaka_center",
    Transform=avango.gua.make_trans_mat(camera_translations_center),
    PipelineDescription=occlusion_slave_pipeline_description,
    #PipelineDescription=pipeline_description,
    EyeDistance = eye_distance,
    EnableStereo = True,
    BlackList = ["invisible_osaka_avatar"], 
    #needs to be invisible as soon as the real render-client comes into play
    )


client_cam_left = avango.gua.nodes.CameraNode(
    ViewID=4,
    Name="viewer_0_weimar_left",
    LeftScreenPath=screen_path,
    RightScreenPath=screen_path,
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="client_window_weimar_left",
    Transform=avango.gua.make_trans_mat(camera_translations_left),
    PipelineDescription=pipeline_description,

    EyeDistance = eye_distance,
    EnableStereo = True,
    )

occlusion_slave_client_cam_left = avango.gua.nodes.CameraNode(
    ViewID=4,
    Name="os_weimar_v0_osaka_left",
    LeftScreenPath=screen_path,
    RightScreenPath=screen_path,
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="slave_weimar_v0_osaka_left",
    Transform=avango.gua.make_trans_mat(camera_translations_left),
    PipelineDescription=occlusion_slave_pipeline_description,
    #PipelineDescription=pipeline_description,
    EyeDistance = eye_distance,
    EnableStereo = True,
    BlackList = ["invisible_osaka_avatar"], 
    #needs to be invisible as soon as the real render-client comes into play
    )

client_cam_right = avango.gua.nodes.CameraNode(
    ViewID=3,
    Name="viewer_0_weimar_right",
    LeftScreenPath=screen_path,
    RightScreenPath=screen_path,
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="client_window_weimar_right",
    Transform=avango.gua.make_trans_mat(camera_translations_right),
    PipelineDescription=pipeline_description,

    EyeDistance = eye_distance,
    EnableStereo = True,
    )

occlusion_slave_client_cam_right = avango.gua.nodes.CameraNode(
    ViewID=3,
    Name="os_weimar_v0_osaka_right",
    LeftScreenPath=screen_path,
    RightScreenPath=screen_path,
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="slave_weimar_v0_osaka_right",
    Transform=avango.gua.make_trans_mat(camera_translations_right),
    PipelineDescription=occlusion_slave_pipeline_description,
    #PipelineDescription=pipeline_description,
    EyeDistance = eye_distance,
    EnableStereo = True,
    BlackList = ["invisible_osaka_avatar"], 
    #needs to be invisible as soon as the real render-client comes into play
    )





scene_view_transform = avango.gua.nodes.TransformNode(Name="scene_view_transform")
#scene_view_transform.Transform.value = avango.gua.make_rot_mat(90.0, 0.0, 1.0, 0.0)
scene_view_transform.Children.value = [scene_transform]#, screen_geometry]



loggings_indicator = avango.gua.nodes.TransformNode(Name="logging_indicator")

navigation = avango.gua.nodes.TransformNode(Name="navigation")
navigation.Children.value = [screen, server_cam, client_cam_left, client_cam_center, client_cam_right, occlusion_slave_client_cam_left, occlusion_slave_client_cam_center, occlusion_slave_client_cam_right]

navigation.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 11.0)

grouped_view_setups_and_scene = avango.gua.nodes.TransformNode(Name="grouped_view_setups_and_scene")
grouped_view_setups_and_scene.Children.value = [navigation, scene_view_transform, loggings_indicator]

screen.Transform.value = avango.gua.make_trans_mat(0.0, 1.445, -2.0)
#screen.Children.value = [occlusion_slave_client_cam_center, client_cam_center, occlusion_slave_client_cam_left, client_cam_left, occlusion_slave_client_cam_right, client_cam_right, server_cam]
#screen.Transform.value = avango.gua.make_trans_mat(0.0, eye_height, 8.5)

nettrans.Children.value = [grouped_view_setups_and_scene]

#grouped_view_setups_and_scene.Transform.value = avango.gua.make_trans_mat(0.0, 1.0, 2.5)


keyboard_based_default_viewer = examples_common.default_views.DefaultViews()
scene_view_transform.Transform.connect_from(keyboard_based_default_viewer.OutTransform)



#navigator = examples_common.navigator.Navigator()
#navigator.StartLocation.value = screen.Transform.value.get_translate()
#navigator.OutTransform.connect_from(screen.Transform)

#navigator.RotationSpeed.value = 0.12/30.0
#navigator.MotionSpeed.value = 0.07/10.0

avatar_geode.Tags.value = ["invisible_osaka_avatar"]


#screen.Transform.connect_from(navigator.OutTransform)

graph.Root.value.Children.value = [nettrans]

make_node_distributable(grouped_view_setups_and_scene)
#make_node_distributable(non_avatar_scene_transform)
#make_node_distributable(avatar_transform)
#make_node_distributable(scene_view_transform)
#make_node_distributable(scene_transform)
#make_node_distributable(screen)

#make_node_distributable(server_cam)

#make_node_distributable(client_cam_center)
#make_node_distributable(occlusion_slave_client_cam_center)
#make_node_distributable(client_cam_left)
#make_node_distributable(occlusion_slave_client_cam_left)
#make_node_distributable(client_cam_right)
#make_node_distributable(occlusion_slave_client_cam_right)

nettrans.distribute_object(tri_pass)
nettrans.distribute_object(tquad_pass)
nettrans.distribute_object(lvis_pass)
nettrans.distribute_object(res_pass)
nettrans.distribute_object(tscreenspace_pass)
nettrans.distribute_object(spoints_pass_description)
nettrans.distribute_object(occlusion_slave_res_pass)


for p in pipeline_description.Passes.value:
    nettrans.distribute_object(p)
nettrans.distribute_object(pipeline_description)

for p in occlusion_slave_pipeline_description.Passes.value:
    nettrans.distribute_object(p)
nettrans.distribute_object(occlusion_slave_pipeline_description)

# setup viewing
window = avango.gua.nodes.GlfwWindow(Size=size,
                                     LeftResolution=size,
                                     Title="server_window")

window.EnableVsync.value = False

avango.gua.register_window("server_window", window)

#setup viewer
viewer = avango.gua.nodes.Viewer()
viewer.SceneGraphs.value = [graph]
viewer.Windows.value = [window]



timer = avango.nodes.TimeSensor()

avatar_path_animator = TimedKeyframePathAnimation()
avatar_path_animator.TimeIn.connect_from(timer.Time)
avatar_transform.Transform.connect_from(avatar_path_animator.MatrixOut)


key_event_logger = TimedKeyToggling()
key_event_logger.TimeIn.connect_from(timer.Time)
key_event_logger.set_keyboard_events(keyboard_based_default_viewer)
key_event_logger.set_logging_node(loggings_indicator)


#tracking_info_updater = TimedUpdateTrackingInfo()
#tracking_info_updater.TimeIn.connect_from(timer.Time)




if not ("3_CLIENTS_SIMULATED" == OBSERVER_MODE):

    #tracking_target id 18 = video cam
    DeviceSensorVideoCamera = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
    DeviceSensorVideoCamera.Station.value = "tracking-dbl-video-camera"
    DeviceSensorVideoCamera.TransmitterOffset.value = avango.gua.make_trans_mat(0.0,0.045,0.0)

    #glasses id=22
    DeviceSensorGlassesRight = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
    DeviceSensorGlassesRight.Station.value = "tracking-dbl-glasses-A"
    DeviceSensorGlassesRight.TransmitterOffset.value = avango.gua.make_trans_mat(0.0,0.045,0.0)

    #glasses id=23
    DeviceSensorGlassesCenter = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
    DeviceSensorGlassesCenter.Station.value = "tracking-dbl-glasses-B"
    DeviceSensorGlassesCenter.TransmitterOffset.value = avango.gua.make_trans_mat(0.0,0.045,0.0)

    #glasses id=24
    DeviceSensorGlassesLeft = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
    DeviceSensorGlassesLeft.Station.value = "tracking-dbl-glasses-C"
    DeviceSensorGlassesLeft.TransmitterOffset.value = avango.gua.make_trans_mat(0.0,0.045,0.0)



    if "3_CLIENTS_LIVE" == OBSERVER_MODE:
        client_cam_right.Transform.connect_from(DeviceSensorGlassesRight.Matrix)
        occlusion_slave_client_cam_right.Transform.connect_from(DeviceSensorGlassesRight.Matrix)
        client_cam_center.Transform.connect_from(DeviceSensorGlassesCenter.Matrix)
        occlusion_slave_client_cam_center.Transform.connect_from(DeviceSensorGlassesCenter.Matrix)
        client_cam_left.Transform.connect_from(DeviceSensorGlassesLeft.Matrix)
        occlusion_slave_client_cam_left.Transform.connect_from(DeviceSensorGlassesLeft.Matrix)
    else:
        client_cam_right.Transform.connect_from(DeviceSensorVideoCamera.Matrix)
        occlusion_slave_client_cam_right.Transform.connect_from(DeviceSensorVideoCamera.Matrix)
        client_cam_center.Transform.connect_from(DeviceSensorVideoCamera.Matrix)
        occlusion_slave_client_cam_center.Transform.connect_from(DeviceSensorVideoCamera.Matrix)
        client_cam_left.Transform.connect_from(DeviceSensorVideoCamera.Matrix)
        occlusion_slave_client_cam_left.Transform.connect_from(DeviceSensorVideoCamera.Matrix)

"""
feedback_sender = FeedbackSender()

feedback_sender.registerSceneGraph(graph)
feedback_sender.registerCamNodeCenter(client_cam_center)

feedback_sender.AvatarWorldMatrixIn.connect_from(avatar_geode.WorldTransform)
feedback_sender.ClientCamCenterMatrixIn.connect_from(client_cam_center.WorldTransform)
"""
#guaVE = GuaVE()
#guaVE.start(locals(), globals())

viewer.DesiredFPS.value = 1000.0
viewer.run()
