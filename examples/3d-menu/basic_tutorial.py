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
Learn how to use the avango.menu in less than 100 lines.

This example demonstrates how to set up and use a simple 3d-menu in five easy steps.
The basic use of all widget types, container, panel, sub menu and menu tool is shown.
'''
import avango.menu.widget
import avango.menu.Preferences  # only if you want to change settings
import avango.osg.viewer
import avango.script

# First, we have to set up a viewer. This is not considered to be the first
# step of this example - you always have to do this ;)
window = avango.osg.viewer.nodes.GraphicsWindow(NumStencilBits=1) # avango.menu needs a stencil buffer!
camera = avango.osg.viewer.nodes.Camera(Window=window)
viewer = avango.osg.viewer.nodes.Viewer(MasterCamera=camera)

# In case you need to process mouse or key events, you need to create an event handler
# You need to connect some window fields to corresponding event handler fields
eventfields = avango.osg.viewer.nodes.EventFields(View = viewer)
window.ToggleFullScreen.connect_from(eventfields.KeyAltReturn)
window.DragEvent.connect_from(eventfields.DragEvent)
window.MoveEvent.connect_from(eventfields.MoveEvent)

### STEP 1 - create two menu panels and a panel group, then make my_menu visible
my_menu = avango.menu.Panel(Title='My Menu', Name='mymenu') # setting a name is not necessary, see below
sub_menu = avango.menu.Panel(Title='Sub Menu', Orientation=1) # 1=vertical (default), 2=horizontal

panel_group = avango.menu.PanelGroup()
panel_group.add_panel(my_menu)
panel_group.add_panel(sub_menu)
panel_group.show_panel_by_name('mymenu') # you can also use: show_panel(my_menu)

### STEP 2 - create some widgets and add them to the menu
button = avango.menu.widget.PushButton(IconFilenames=['smiley.png']) # yes, it has to be a list!
checkbox = avango.menu.widget.CheckBox(CheckState=True)
divider = avango.menu.widget.Divider()
slider = avango.menu.widget.Slider(Value=25)
image = avango.menu.widget.Image(ImageFilename="AvangoLogo.png", ImageWidth=0.8, ImageAspectRatio=462.0/119.0)

# radio buttons must be assigned to a radio button group
radio_button_group = avango.menu.RadioButtonGroup()
radio1 = avango.menu.widget.RadioButton(Title='Radio Button 1', Group=radio_button_group)
radio2 = avango.menu.widget.RadioButton(Title='Radio Button 2', Group=radio_button_group, CheckState=True)

# create a horizontal container and add the radio buttons
horizontal_container = avango.menu.widget.Container(Orientation=2) # 1=vertical, 2=horizontal
horizontal_container.add_widgets([radio1, radio2])

my_menu.add_widgets([button, checkbox, divider, horizontal_container, divider, slider, sub_menu, divider, image])
sub_menu.add_widget(button)

# edit preferences
avango.menu.Preferences.container.WidgetHorizontalPadding.value = 0.1 # distance between horizontal widgets

### STEP 3 - create scene root and add menu to scene
scene_root = avango.osg.nodes.MatrixTransform()

# the rotation is a workaround for a nasty polygon offset issue. any idea??
scene_root.Matrix.value = avango.osg.make_scale_mat(0.1, 0.1, 0.1) * avango.osg.make_rot_mat(0.01, 1, 0, 0)
scene_root.Children.value.append(panel_group.root)

### STEP 4 - define and set up tools to interact with the menu.
my_tools = avango.menu.Tool(Enable=True)
my_tools.PickTrigger.connect_from(eventfields.MouseButtons_OnlyLeft)
my_tools.Transform.connect_from(camera.MouseNearTransform)
my_tools.MenuRootNode.value = scene_root

### STEP 5 - now it's time to add 'functionality' - a script node reacting on menu input
class DoSomething(avango.script.Script):
    Button = avango.SFBool()
    CheckBox = avango.SFBool()
    Radio1 = avango.SFBool()
    Radio2 = avango.SFBool()
    Slider = avango.SFFloat()
    MyMenuVisible = avango.SFBool()
    SubMenuVisible = avango.SFBool()

    def evaluate(self):
        for field in [self.Button, self.CheckBox, self.Radio1, self.Radio2, self.Slider, self.MyMenuVisible, self.SubMenuVisible]:
            print field._get_name(), ':', field.value
        print '---'

do_something = DoSomething()
do_something.Button.connect_from(button.Select)
do_something.CheckBox.connect_from(checkbox.CheckState)
do_something.Radio1.connect_from(radio1.CheckState)
do_something.Radio2.connect_from(radio2.CheckState)
do_something.Slider.connect_from(slider.Value)
do_something.MyMenuVisible.connect_from(my_menu.Visible)
do_something.SubMenuVisible.connect_from(sub_menu.Visible)

# additionally, let's toggle the Enable field of the container via the CheckBox widget
horizontal_container.Enable.connect_from(checkbox.CheckState)

# start viewer
viewer.Scene.value = scene_root
viewer.run()
