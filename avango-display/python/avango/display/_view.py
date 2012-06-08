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

import avango
import avango.osg
import avango.script
from avango.script import field_has_changed

class View(avango.script.Script):

    Root = avango.osg.SFNode()
    Camera = avango.osg.SFMatrix()
    Viewport = avango.osg.SFVec4()
    Depth = avango.SFFloat()
    UserSelector = avango.SFInt()
    Near = avango.SFFloat()
    Far = avango.SFFloat()
    BackgroundColor = avango.osg.SFVec4()

    def __init__(self):
        self.super(View).__init__()
        self.Viewport.value = avango.osg.Vec4(0, 0, 1, 1)
        self.Depth.value = 0
        self.Near.value = 0.1
        self.Far.value = 500.
        self.BackgroundColor.value = avango.osg.Vec4(0., 0., 0., 1.)
        self.UserSelector.value = 0

    def evaluate(self):
        pass





class LShapeView(avango.script.Script):
    
    RootCamera1 = avango.osg.SFNode()
    RootCamera2 = avango.osg.SFNode()
        
    Root = avango.osg.SFNode()
    Camera = avango.osg.SFMatrix()
    Viewport = avango.osg.SFVec4()
    Depth = avango.SFFloat()
    UserSelector = avango.SFInt()
    Near = avango.SFFloat()
    Far = avango.SFFloat()
    BackgroundColor = avango.osg.SFVec4()

    def __init__(self):
        self.super(IConeView).__init__()
        self.Viewport.value = avango.osg.Vec4(0, 0, 1, 1)
        self.Depth.value = 0
        self.Near.value = 0.1
        self.Far.value = 500.
        self.BackgroundColor.value = avango.osg.Vec4(0., 0., 0., 1.)
        self.UserSelector.value = 0
        
        n = avango.osg.nodes.Group()
        self.RootCamera1.Children.append(n)
    
        n = avango.osg.nodes.Group()
        self.RootCamera2.Children.append(n)
        
    @field_has_changed(Root)
    def root_has_changed(self):
        self.RootCamera1.value.Children[0] = self.Root.value
        self.RootCamera2.value.Children[0] = self.Root.value


class MonitorView(avango.script.Script):
    
    BoundingSphereRoot = avango.osg.SFNode()
    TransformAndScaleNode = avango.osg.SFMatrixTransform()
    ToggleFullScreen = avango.SFBool()
    WindowDecoration = avango.SFBool()
    EnableTrackball = avango.SFBool()
    
    Root = avango.osg.SFNode()
    Camera = avango.osg.SFMatrix()
    Viewport = avango.osg.SFVec4()
    Depth = avango.SFFloat()
    UserSelector = avango.SFInt()
    Near = avango.SFFloat()
    Far = avango.SFFloat()
    BackgroundColor = avango.osg.SFVec4()

    def __init__(self):
        self.super(MonitorView).__init__()
    
        bounding_sphere_root = avango.osg.nodes.BoundingBoxCalculator()
        self.TransformAndScaleNode.value = avango.osg.nodes.MatrixTransform()
        bounding_sphere_root.Children.value.append(self.TransformAndScaleNode.value)
        self.BoundingSphereRoot.value = bounding_sphere_root
        
        self.WindowDecoration.value = True
        self.EnableTrackball.value = False
        
        self.Viewport.value = avango.osg.Vec4(0, 0, 1, 1)
        self.Depth.value = 0
        self.Near.value = 0.1
        self.Far.value = 500.
        self.BackgroundColor.value = avango.osg.Vec4(0., 0., 0., 1.)
        self.UserSelector.value = 0
    
    @field_has_changed(Root)
    def root_has_changed(self):
        self.TransformAndScaleNode.value.Children.value = [self.Root.value]

