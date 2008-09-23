# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# Avango is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# Avango is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Avango. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# Avango is a trademark owned by FhG.                                    #
#                                                                        #
##########################################################################

'''
Exemplary setup of a head-tracked two viewer display where the screens
of the two viewers are overlayed. The viewpoint of each user is
determined via tracking devices.
'''

import sys
import math
import avango.osg.viewer
import avango.daemon


# set up head tracking

device_service = avango.daemon.DeviceService()
head_sensor0 = avango.daemon.nodes.DeviceSensor(DeviceService = device_service,
                                               Station = "ve-dtrack-head2")
head_sensor0.ReceiverOffset.value = avango.osg.make_trans_mat(0.076, -0.016, 0.025)
head_sensor0.TransmitterOffset.value = avango.osg.make_rot_mat(math.radians(-90), 1, 0, 0)
head_sensor1 = avango.daemon.nodes.DeviceSensor(DeviceService = device_service,
                                               Station = "ve-dtrack-head3")
head_sensor1.ReceiverOffset.value = avango.osg.make_trans_mat(-0.073, -0.016, 0.025)
head_sensor1.TransmitterOffset.value = avango.osg.make_rot_mat(math.radians(-90), 1, 0, 0)


# set up scene graph

obj_trans = avango.osg.nodes.MatrixTransform()

def make_sphere(mat):
    sphere = avango.osg.nodes.Sphere()
    sphere.Radius.value = 0.1
    sphere.Matrix.value = mat
    obj_trans.Children.value.append(sphere)

if len(sys.argv) == 1:
    make_sphere(avango.osg.make_trans_mat( 0.0,  0.0,  0.0))
    make_sphere(avango.osg.make_trans_mat( 0.5,  0.5,  0.5))
    make_sphere(avango.osg.make_trans_mat( 0.5,  0.5, -0.5))
    make_sphere(avango.osg.make_trans_mat( 0.5, -0.5,  0.5))
    make_sphere(avango.osg.make_trans_mat( 0.5, -0.5, -0.5))
    make_sphere(avango.osg.make_trans_mat(-0.5,  0.5,  0.5))
    make_sphere(avango.osg.make_trans_mat(-0.5,  0.5, -0.5))
    make_sphere(avango.osg.make_trans_mat(-0.5, -0.5,  0.5))
    make_sphere(avango.osg.make_trans_mat(-0.5, -0.5, -0.5))
else:
    obj = avango.osg.nodes.LoadFile(Filename=sys.argv[1])
    obj_trans.Children.value = [obj]
    obj_trans.Matrix.value = avango.osg.make_scale_mat(0.1, 0.1, 0.1)


# set up viewer in scene graph

viewer_abs = avango.osg.nodes.AbsoluteTransform()
viewer_trans = avango.osg.nodes.MatrixTransform()
viewer_trans.Children.value = [viewer_abs]
viewer_trans.Matrix.value = avango.osg.make_trans_mat(0, -1.2, 2.4)

root_group = avango.osg.nodes.Group()
root_group.Children.value = [viewer_trans, obj_trans]


# set up viewing

window0 = avango.osg.viewer.nodes.GraphicsWindow()
window0.ScreenIdentifier.value = ":0.0"
window0.Decoration.value = False
window0.QuadBufferStereo.value = True
window0.ShowCursor.value = False
window0.WantedPositionX.value = 0
window0.WantedPositionY.value = 0
window0.WantedWidth.value = 1280
window0.WantedHeight.value = 1024
window0.RealScreenWidth.value = 6
window0.RealScreenHeight.value = 2.4
window0.AutoHeight.value = False

camera0 = avango.osg.viewer.nodes.Camera()
camera0.Window.value = window0
camera0.ViewerTransform.connect_from(viewer_abs.AbsoluteMatrix)
camera0.ScreenTransform.value = avango.osg.make_trans_mat(0, 1.2, -2.4)
camera0.EyeTransform.connect_from(head_sensor0.Matrix)
#camera0.EyeTransform.value = avango.osg.make_trans_mat(-0.5, 1.7, 0)
camera0.EyeOffset.value = 0.03

window1 = avango.osg.viewer.nodes.GraphicsWindow()
window1.ScreenIdentifier.value = ":0.0"
window1.Decoration.value = False
window1.QuadBufferStereo.value = True
window1.ShowCursor.value = False
window1.WantedPositionX.value = 1280
window1.WantedPositionY.value = 0
window1.WantedWidth.value = 1280
window1.WantedHeight.value = 1024
window1.RealScreenWidth.value = 6
window1.RealScreenHeight.value = 2.4
window1.AutoHeight.value = False

camera1 = avango.osg.viewer.nodes.Camera()
camera1.Window.value = window1
camera1.ViewerTransform.connect_from(viewer_abs.AbsoluteMatrix)
camera1.ScreenTransform.value = avango.osg.make_trans_mat(0, 1.2, -2.4)
camera1.EyeTransform.connect_from(head_sensor1.Matrix)
#camera1.EyeTransform.value = avango.osg.make_trans_mat(0.5, 1.7, 0)
camera1.EyeOffset.value = 0.03

viewer = avango.osg.viewer.nodes.Viewer()
viewer.MasterCamera.value = camera0
viewer.SlaveCameras.value = [camera1]
viewer.Scene.value = root_group

viewer.run()
