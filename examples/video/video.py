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

# Description: Shows a video on a sphere
# call it like this:
# python video.py yourExampleVideo.avi

import avango.osg.viewer
import avango.moving
import sys
import math

# set up scene graph
root_group = avango.osg.nodes.Group()

# set up viewing
window = avango.osg.viewer.nodes.GraphicsWindow()

camera = avango.osg.viewer.nodes.Camera()
camera.Window.value = window

viewer = avango.osg.viewer.nodes.Viewer()
viewer.MasterCamera.value = camera
viewer.Scene.value = root_group

# set up event handling

events = avango.osg.viewer.nodes.EventFields(View = viewer)
window.ToggleFullScreen.connect_from(events.KeyAltReturn)
window.DragEvent.connect_from(events.DragEvent)
window.MoveEvent.connect_from(events.MoveEvent)

# load a video
loadimagestream = avango.osg.nodes.LoadImageStream(
                    Filename = sys.argv[1])
viewer.frame()

video = loadimagestream.ImageStream.get_value()
video.Loop.value = True
video.Status.value=avango.osg.streamstatus.playing

videoTexture = avango.osg.nodes.Texture2D(Image = video)
ss = avango.osg.nodes.StateSet(Texture0 = videoTexture)
obj = avango.osg.nodes.Sphere(Name="TexturedSphere", StateSet = ss)

root_group.Children.value = [obj]

# render a frame to update bounding spheres and scale model to fit in window
viewer.frame()
scale = 0.08 / obj.get_bounding_sphere().radius()

obj.Matrix.value = \
  avango.osg.make_rot_mat(math.radians(90), 1,0,0) * \
  avango.osg.make_rot_mat(math.radians(90), 0,1,0) * \
  avango.osg.make_trans_mat(0,0,-5) * \
  avango.osg.make_scale_mat(scale,scale,scale)

viewer.run()
