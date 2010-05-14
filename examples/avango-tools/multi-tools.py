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

import avango.script
import avango.osg.viewer
import avango.moving
import avango.tools


# extend sphere functionality

def jump_home(self):
    self.Matrix.value = self.HomeMatrix.value

def highlight(self):
    self.Color.value = avango.osg.Vec4(1.0, 0.0, 0.0, 1.0)

def unhighlight(self):
    self.Color.value = avango.osg.Vec4(1.0, 1.0, 1.0, 1.0)

def destruct(self):
    obj_trans.Children.value.remove(self)

avango.osg.Sphere.jump_home = jump_home
avango.osg.Sphere.highlight = highlight
avango.osg.Sphere.unhighlight = unhighlight
avango.osg.Sphere.destruct = destruct

def make_sphere(pos):
    sphere = avango.osg.nodes.Sphere(Radius = 0.01, Matrix = avango.osg.make_trans_mat(pos))
    sphere.add_field(sphere.Matrix, "HomeMatrix")
    obj_trans.Children.value.append(sphere)


# set up scene graph

obj_trans = avango.osg.nodes.MatrixTransform()

make_sphere(avango.osg.Vec3( 0.00,  0.00,  0.00))
make_sphere(avango.osg.Vec3( 0.05,  0.05,  0.05))
make_sphere(avango.osg.Vec3( 0.05,  0.05, -0.05))
make_sphere(avango.osg.Vec3( 0.05, -0.05,  0.05))
make_sphere(avango.osg.Vec3( 0.05, -0.05, -0.05))
make_sphere(avango.osg.Vec3(-0.05,  0.05,  0.05))
make_sphere(avango.osg.Vec3(-0.05,  0.05, -0.05))
make_sphere(avango.osg.Vec3(-0.05, -0.05,  0.05))
make_sphere(avango.osg.Vec3(-0.05, -0.05, -0.05))

root_group = avango.osg.nodes.Group(Children = [obj_trans])


# set up viewing

window = avango.osg.viewer.nodes.GraphicsWindow()
camera = avango.osg.viewer.nodes.Camera(Window = window)
viewer = avango.osg.viewer.nodes.Viewer(MasterCamera = camera, Scene = root_group)


# set up event handling

eventfields = avango.osg.viewer.nodes.EventFields(View = viewer)
window.ToggleFullScreen.connect_from(eventfields.KeyAltReturn)
window.DragEvent.connect_from(eventfields.DragEvent)
window.MoveEvent.connect_from(eventfields.MoveEvent)


# set up trackball mover

trackball = avango.moving.nodes.Trackball(Matrix = camera.ViewerTransform.value)
trackball.Direction.connect_from(window.MousePositionNorm)
trackball.RotateTrigger.connect_from(eventfields.MouseButtons_OnlyMiddle)
trackball.ZoomTrigger.connect_from(eventfields.MouseButtons_MiddleAndRight)
trackball.PanTrigger.connect_from(eventfields.MouseButtons_OnlyRight)
trackball.CenterTransform.value = \
  avango.osg.make_scale_mat(0.1, 0.1, 0.1) * \
  avango.osg.make_trans_mat(0, 0, -0.6)

camera.ViewerTransform.connect_from(trackball.Matrix)


# setup sphere creator, creates a sphere with space key at mouse position

class SphereCreator(avango.script.Script):
    Trigger = avango.SFBool()
    Transform = avango.osg.SFMatrix()

    def evaluate(self):
        if self.Trigger.value:
            make_sphere(self.Transform.value.get_translate())

sphere_creator = SphereCreator()
sphere_creator.Trigger.connect_from(eventfields.KeySpace)
sphere_creator.Transform.connect_from(camera.MouseTransform)


# setup drag tool

pick_selector = avango.tools.nodes.PickSelector()
pick_selector.PickTrigger.connect_from(eventfields.MouseButtons_OnlyLeft)
pick_selector.PickRayTransform.connect_from(camera.MouseNearTransform)
pick_selector.RootNode.connect_from(viewer.Scene)

drag_tool = avango.tools.nodes.DragTool()
drag_tool.DragTransform.connect_from(camera.MouseTransform)
drag_tool.Targets.connect_from(pick_selector.SelectedTargets)


# setup jumphome tool

jump_home_selector = avango.tools.nodes.PickSelector()
jump_home_selector.PickTrigger.connect_from(eventfields.MouseButtons_LeftAndRight)
jump_home_selector.PickRayTransform.connect_from(camera.MouseNearTransform)
jump_home_selector.RootNode.connect_from(viewer.Scene)

def jump_home_callback(holder):
    if hasattr(holder.Target.value, "jump_home"):
        holder.Target.value.jump_home()

jump_home_tool = avango.tools.nodes.ScriptTool(TargetCallback = jump_home_callback)
jump_home_tool.Targets.connect_from(jump_home_selector.SelectedTargets)


# setup highlight tool

highlight_selector = avango.tools.nodes.PickSelector(PickTrigger = True,
                                                     TransitionOnly = False,
                                                     FirstHitOnly = False)
highlight_selector.PickRayTransform.connect_from(camera.MouseNearTransform)
highlight_selector.RootNode.connect_from(viewer.Scene)

def highlight_callback(holder):
    if hasattr(holder.Target.value, "highlight"):
        holder.Target.value.highlight()

def unhighlight_callback(holder):
    if hasattr(holder.Target.value, "unhighlight"):
        holder.Target.value.unhighlight()

highlight_tool = avango.tools.nodes.ScriptTool(AddedTargetCallback = highlight_callback,
                                               RemovedTargetCallback = unhighlight_callback)
highlight_tool.Targets.connect_from(highlight_selector.SelectedTargets)


# setup sphere destruct tool, highlighted spheres are deleted with delete key

sphere_destruct_selector = avango.tools.nodes.TriggerSelector()
sphere_destruct_selector.Trigger.connect_from(eventfields.KeyDelete)
sphere_destruct_selector.Targets.connect_from(highlight_selector.SelectedTargets)

def destruct_callback(holder):
    if hasattr(holder.Target.value, "destruct"):
        holder.Target.value.destruct()

sphere_destruct_tool = avango.tools.nodes.ScriptTool(TargetCallback = destruct_callback)
sphere_destruct_tool.Targets.connect_from(sphere_destruct_selector.SelectedTargets)


# run evaluation and render loop

viewer.run()
