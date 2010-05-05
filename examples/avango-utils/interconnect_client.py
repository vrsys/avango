# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       #
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
import avango.osg
import avango.osg.viewer
import avango.tools
import avango.utils
import socket

sphere_count = 0

class ColorSwitcher(avango.script.Script):
	Active = avango.SFBool()
	Color = avango.osg.SFVec4()
	Text = avango.MFString()
	KeyShift = avango.SFBool()
	
	ReferencedObject = avango.script.SFObject()
	ObjectName = avango.SFString()
	PickedTargets = avango.tools.MFTargetHolder()

	def evaluate(self):
		hit = False
		for n in self.PickedTargets.value:
		    if n.Target.value.Name.value == self.ObjectName.value:
		    	hit = True
		if self.Active.value and hit:
			if self.KeyShift.value:
				output_node.remove_node(self.ReferencedObject.value)
				scene.Children.value.remove(self.ReferencedObject.value)
				avango.disconnect_and_clear_all_fields(self.ReferencedObject.value)
			else:
				self.Color.value = avango.osg.Vec4(1, 0, 0, 1)
				self.Text.value.append("active")
		else:
			self.Color.value = avango.osg.Vec4(1, 1, 1, 1)
			self.Text.value.append("inactive")

class InteractionInput(avango.script.Script):
	KeyEnter = avango.SFBool()
	
	@avango.script.field_has_changed
	def field_has_changed(self, name, field):
		if name == "KeyEnter" and self.KeyEnter.value:
			global sphere_count
			sphere_count += 1
			
			sphere_name = "MySphere" + str(sphere_count)
			print "add " + sphere_name
			
			color_switcher = ColorSwitcher()
			color_switcher.Active.connect_from(events.MouseButtonLeft)
			color_switcher.ObjectName.value = sphere_name
			color_switcher.PickedTargets.connect_from(pick_tool.SelectedTargets)
			color_switcher.KeyShift.connect_from(events.KeyShift)
			sphere = avango.osg.nodes.Sphere(Radius = 1, Name = sphere_name)
			sphere.Color.connect_from(color_switcher.Color)
			scene.Children.value.append(sphere)
			output_node.add_node(sphere)
			
			color_switcher.ReferencedObject.value = sphere

scene = avango.osg.nodes.Group()

window = avango.osg.viewer.nodes.GraphicsWindow(Title = "Client")
camera = avango.osg.viewer.nodes.Camera(Window = window)
viewer = avango.osg.viewer.nodes.Viewer(MasterCamera = camera, Scene = scene)
events = avango.osg.viewer.nodes.EventFields(View = viewer)
window.DragEvent.connect_from(events.DragEvent)
window.MoveEvent.connect_from(events.MoveEvent)

camera.ViewerTransform.value = avango.osg.make_trans_mat(0, 0, 50)

interaction_input = InteractionInput()
interaction_input.KeyEnter.connect_from(events.KeyEnter)

pick_tool = avango.tools.nodes.PickSelector()
pick_tool.RootNode.value = scene
pick_tool.PickTrigger.connect_from(events.MouseButtonLeft)
pick_tool.PickRayTransform.connect_from(camera.MouseNearTransform)

drag_tool = avango.tools.nodes.DragTool()
drag_tool.Targets.connect_from(pick_tool.SelectedTargets)
drag_tool.DragTransform.connect_from(camera.MouseNearTransform)




# a node that receives data (from a server)
class Input(avango.utils.InterconnectInputNode):
	pass

# a node that sends data (to a server)
class Output(avango.utils.InterconnectOutputNode):
	pass

try:
	client = avango.utils.InterconnectClient(Host="localhost", Port=19901)

	input_node = Input()
	output_node = Output()

	input_node.set_stream(client)
	output_node.set_stream(client)
except socket.error, msg:
	print "Error: " + str(msg)
	print "Not connected."



viewer.run()
