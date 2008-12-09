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
Example of how to use the avango.menu as a context-menu/context-control

This example demonstrates one possibility of how to use the avango 3d-menu
as a context control for creating and manipulating objects. Many features of
basic_tutorial.py are used and more functionality is introduced:

  - use the menu as a context control
  - add custom fields to objects for defining a context menu behavior
  - create a simple color tool class, that can be added as a menu container

After the app is launched, you will see a blank black window. Right-click on
the background to open a context menu. There you can create simple objects,
that can be dragged around. Right-clicking an object does open a context
control for manipulating the objects fields like size, scale and color.

As the menus are created just in time, the limits of having complex context
menus are visible: creating menu structures with lots of field connections is
somewhat slow. For more complex applications, it is recommended to create all
available context menus at startup time and just connect, show and hide them
when needed.
'''
import avango.menu.widget
import avango.menu.Preferences
import avango.osg.viewer
import avango.script
from avango.script import field_has_changed
import avango.moving
from os.path import join
import math


# Setup world
# Create a catch-all-sphere to hanlde clicks in 'free space'
catch_all_sphere = avango.osg.nodes.Sphere(
                       Radius=500,
                       StateSet=avango.osg.nodes.StateSet(
                           CullFaceMode=True,
                           CullFace = avango.osg.nodes.CullFace(Mode=0x408)))
scene_root = avango.osg.nodes.Group(Children=[catch_all_sphere])


# Setup viewer
window = avango.osg.viewer.nodes.GraphicsWindow(NumStencilBits=1)
camera = avango.osg.viewer.nodes.Camera(Window=window)
viewer = avango.osg.viewer.nodes.Viewer(MasterCamera=camera)
viewer.Scene.value = scene_root


# Edit menu preferences
# When re-opening a menu panel, no entry should be highlighted
avango.menu.Preferences.panel.RemoveHighlightOnHide.value = True


# Setup context menu panel
context_menu = avango.menu.Panel(Title='Context Menu', Name='contextmenu')


# Setup panel group and placement tool
placement_tool = avango.menu.PlacementBase()
placement_tool.set_head_transform(camera.ViewerTransform)
placement_tool.SceneRoot.value = scene_root

panel_group = avango.menu.PanelGroup()
panel_group.add_panel(context_menu)
panel_group.PlacementTool.value = placement_tool
scene_root.Children.value.append(panel_group.root)


# Set up menu tools to interact with the menu and open a context menu
tools = avango.menu.Tool(Enable=True, MenuRootNode=scene_root)
tools.PickTrigger.connect_from(window.MouseButtons_OnlyLeft)
tools.ContextTrigger.connect_from(window.MouseButtons_OnlyRight)
tools.Transform.connect_from(camera.MouseNearTransform)

# Set up tools for dragging objects with name 'draggable'
pick_selector = avango.tools.nodes.PickSelector(RootNode=scene_root)
name_selector = avango.tools.nodes.NameSelector(SelectableNames=['draggable'])
drag_tool = avango.tools.nodes.DragTool()
name_selector.Targets.connect_from(pick_selector.SelectedTargets)
drag_tool.Targets.connect_from(name_selector.SelectedTargets)

pick_selector.PickTrigger.connect_from(window.MouseButtons_OnlyLeft)
pick_selector.PickRayTransform.connect_from(camera.MouseNearTransform)
drag_tool.DragTransform.connect_from(camera.MouseNearTransform)


# Define a context menu behavior for the catch-all-sphere
# This behavior opens the context menu with a button to add a sphere object to the scene
#
# Adding a field called "MenuToolBehavior" to an object triggers some basic functionality
# in conjunction with the menu.Tool:
#   - Context-clicking (usually with right mouse button) calls the show_context_menu method
#     with the target-holder as an argument. That allows to react on the context click
#     and open a context menu in the right position and for the appropriate object
class CatchAllSphereContextBehavior(avango.script.Script):
    ActivePanel = avango.SFObject()
    CreateSphere = avango.SFBool()
    CreatePanel = avango.SFBool()

    def __init__(self):
        self.old_sphere_state = False
        self.old_panel_state = False

        # The menus build-in icon files can be reached through a join of the preference-datapath and the filename
        self.create_sphere_button = avango.menu.widget.PushButton(
          Title="Create New Sphere",
          IconFilenames=[join(avango.menu.Preferences.datapath,"plus.png")])

        self.create_panel_button = avango.menu.widget.PushButton(
          Title="Create New Panel",
          IconFilenames=[join(avango.menu.Preferences.datapath,"plus.png")])

        # to react on the button presses, just connect the widgets Select fields to internal fields
        self.CreateSphere.connect_from(self.create_sphere_button.Select)
        self.CreatePanel.connect_from(self.create_panel_button.Select)

    def show_context_menu(self, target_holder=None):
        context_menu.Title.value = "World Menu"
        hit_point = target_holder.Intersection.value.Point.value

        # close and empty menu before adding widgets
        # closing the menu triggers ActivePanel=None and helps cleaning up
        # old context panel contents
        panel_group.hide_panels()

        # add buttons to menu
        context_menu.add_widget(self.create_sphere_button)
        context_menu.add_widget(self.create_panel_button)

        # show panel
        panel_group.show_placed_panel(context_menu, hit_point)
        self.ActivePanel.connect_from(panel_group.ActivePanel)

    @field_has_changed(CreateSphere)
    def create_sphere_changed(self):
        if self.CreateSphere.value:
            new_sphere = avango.osg.nodes.Sphere(Radius=0.05,
                                                 Name="draggable")
            new_sphere.add_and_init_field(avango.script.SFObject(), "MenuToolBehavior", object_behavior)
            scene_root.Children.value.append(new_sphere)

        # wait for button release to close menu - otherwise the widget will miss the button release,
        # because the widget is removed from panel before the button was released.
        if self.old_sphere_state and not self.CreateSphere.value:
            panel_group.hide_panels()

        self.old_sphere_state = self.CreateSphere.value

    @field_has_changed(CreatePanel)
    def create_panel_changed(self):
        if self.CreatePanel.value:
            new_panel = avango.osg.nodes.Panel(BorderWidth=0.001,
                                               BorderColor=avango.osg.Vec4(0.3,0.3,0.3,1),
                                               EdgeRadius=0.002)
            new_geode = avango.osg.nodes.Geode(Drawables=[new_panel], Name="draggable")
            new_geode.add_and_init_field(avango.SFBool(), "PickIgnore", True)
            new_transform = avango.osg.nodes.MatrixTransform(Children=[new_geode], Name="draggable")

            # to have easier access to the fields of the panel, we add intermediate fields to the
            # MatrixTransform. When picking the panel, the MatrixTransform is returned, not the panel
            new_transform.add_and_init_field(avango.SFFloat(), "Width", 0.05)
            new_panel.Width.connect_from(new_transform.Width)
            new_transform.add_and_init_field(avango.SFFloat(), "Height", 0.05)
            new_panel.Height.connect_from(new_transform.Height)
            new_transform.add_and_init_field(avango.osg.SFVec4(), "Color", avango.osg.Vec4(0.8,0.8,0.2,1))
            new_panel.PanelColor.connect_from(new_transform.Color)

            # As every object should have it's own context menu, just add the MenuToolBehavior field
            # described above and save an instance of an appropriate behavior class into.
            # In this example, there is only one type of context menu, so it's possible to reuse one
            # instance, created globally below.
            new_transform.add_and_init_field(avango.script.SFObject(), "MenuToolBehavior", object_behavior)

            # add new object to scene
            scene_root.Children.value.append(new_transform)

        if self.old_panel_state and not self.CreatePanel.value:
            panel_group.hide_panels()

        self.old_panel_state = self.CreatePanel.value

    @field_has_changed(ActivePanel)
    def active_panel_changed(self):
        if self.ActivePanel.value == None:
            context_menu.remove_all_widgets()
            self.ActivePanel.disconnect()


# Here is the context menu behavior class for the object. Like above, it's main purpose is to
# open a context menu when an object is right-clicked. But this one uses a real "context" for
# showing the needed controls (widgets): It asks the clicked object for available fields and
# creates and connects widgets for manipulating these fields.
class ObjectBehavior(avango.script.Script):
    ActivePanel = avango.SFObject()
    DeleteObject = avango.SFBool()

    def __init__(self):
        self.radius_slider = avango.menu.widget.Slider(Min=0.01, Max=0.1, Title="Radius")
        self.width_slider = avango.menu.widget.Slider(Min=0.01, Max=0.1, Title="Width")
        self.height_slider = avango.menu.widget.Slider(Min=0.01, Max=0.1, Title="Height")
        self.color_tool = ColorTool()
        self.size_tool = SizeTool()
        self.divider = avango.menu.widget.Divider()
        self.delete_button = avango.menu.widget.PushButton(
          Title="Delete Object",
          IconFilenames=[join(avango.menu.Preferences.datapath,"delete.png")])

        self.delete_old_state = False

    def show_context_menu(self, target_holder=None):
        panel_group.hide_panels()

        # Instead of calling it "Object Menu", you can make the menu title dependent of
        # object types or names.
        context_menu.Title.value = "Object Menu"
        hit_point = target_holder.Intersection.value.Point.value
        self.hit_object = target_holder.Target.value

        # connect widgets to object and add widgets to menu
        if hasattr(self.hit_object, "Radius"):
            self.radius_slider.Value.value = self.hit_object.Radius.value
            self.hit_object.Radius.connect_from(self.radius_slider.Value)
            context_menu.add_widget(self.radius_slider)

        if hasattr(self.hit_object, "Width"):
            self.width_slider.Value.value = self.hit_object.Width.value
            self.hit_object.Width.connect_from(self.width_slider.Value)
            context_menu.add_widget(self.width_slider)

        if hasattr(self.hit_object, "Height"):
            self.height_slider.Value.value = self.hit_object.Height.value
            self.hit_object.Height.connect_from(self.height_slider.Value)
            context_menu.add_widget(self.height_slider)

        if hasattr(self.hit_object, "Matrix"):
            self.size_tool.Object.value = self.hit_object
            self.hit_object.Matrix.connect_from(self.size_tool.MatrixOut)
            context_menu.add_widget(self.size_tool.size_slider)

        if hasattr(self.hit_object, "Color"):
            self.color_tool.ColorIn.value = self.hit_object.Color.value
            self.hit_object.Color.connect_from(self.color_tool.ColorOut)
            context_menu.add_widget(self.color_tool.container)

        context_menu.add_widgets([self.divider, self.delete_button])
        self.DeleteObject.connect_from(self.delete_button.Select)

        # show panel
        panel_group.show_placed_panel(context_menu, hit_point)
        self.ActivePanel.connect_from(panel_group.ActivePanel)


    @field_has_changed(ActivePanel)
    def active_panel_changed(self):
        if self.ActivePanel.value == None:
            context_menu.remove_all_widgets()
            self.disconnect_all_fields()
            self.hit_object.disconnect_all_fields()

    @field_has_changed(DeleteObject)
    def delete_object_changed(self):
        # react on butten release: close panel and delete object
        if self.delete_old_state and not self.DeleteObject.value:
            scene_root.Children.value.remove(self.hit_object)
            self.delete_old_state = False
            panel_group.hide_panels()

        self.delete_old_state = self.DeleteObject.value

# Define a very simple RGB color tool as a menu container with slider for each color component
class ColorTool(avango.script.Script):
    ColorOut = avango.osg.SFVec4()
    ColorIn = avango.osg.SFVec4()
    R = avango.SFFloat()
    G = avango.SFFloat()
    B = avango.SFFloat()

    def __init__(self):
        self.divider = avango.menu.widget.Divider()

        # Small, yellow, non-clickable textlabel
        # Remember to set the IconFilenames to empty list AND TextOnly to True to have a simple textlabel
        self.label = avango.menu.widget.PushButton(Title="RGB Color",
                                                   TextSize=0.07,
                                                   Enable=False,
                                                   IconFilenames=[],
                                                   TextOnly=True,
                                                   TextDisabledColor=avango.osg.Vec4(1,1,0,1))

        # The slider label is hidden if it is set to empty string.
        # If your slider icon is description enough, hide the label to save space
        self.r_slider = avango.menu.widget.Slider(Min=0.1, Max=1.0,
                                                  Title="", IconFilenames=[join(avango.menu.Preferences.datapath,"red.png")])
        self.g_slider = avango.menu.widget.Slider(Min=0.1, Max=1.0,
                                                  Title="", IconFilenames=[join(avango.menu.Preferences.datapath,"green.png")])
        self.b_slider = avango.menu.widget.Slider(Min=0.1, Max=1.0,
                                                  Title="", IconFilenames=[join(avango.menu.Preferences.datapath,"blue.png")])
        self.container = avango.menu.widget.Container()
        self.container.add_widgets([self.divider, self.label, self.r_slider, self.g_slider, self.b_slider])
        self.R.connect_from(self.r_slider.Value)
        self.G.connect_from(self.g_slider.Value)
        self.B.connect_from(self.b_slider.Value)

    @field_has_changed(R)
    def r_changed(self):
        self.ColorOut.value = avango.osg.Vec4(self.R.value,self.ColorOut.value.y,self.ColorOut.value.z,self.ColorIn.value.w)

    @field_has_changed(G)
    def g_changed(self):
        self.ColorOut.value = avango.osg.Vec4(self.ColorOut.value.x,self.G.value,self.ColorOut.value.z,self.ColorIn.value.w)

    @field_has_changed(B)
    def b_changed(self):
        self.ColorOut.value = avango.osg.Vec4(self.ColorOut.value.x,self.ColorOut.value.y,self.B.value,self.ColorIn.value.w)

    @field_has_changed(ColorIn)
    def color_in_changed(self):
        self.r_slider.Value.value = self.ColorIn.value.x
        self.g_slider.Value.value = self.ColorIn.value.y
        self.b_slider.Value.value = self.ColorIn.value.z


# Define a tool for object resizing, obtaining the actual size and position
class SizeTool(avango.script.Script):
    Object = avango.SFObject()
    MatrixOut = avango.osg.SFMatrix()
    Size = avango.SFFloat()

    def __init__(self):
        self.size_slider = avango.menu.widget.Slider(Min=0.1, Max=2.5, Title="Transform Size")
        self.Size.connect_from(self.size_slider.Value)

    @field_has_changed(Object)
    def matrix_in_changed(self):
        if self.Object.value is not None:
            self.size_slider.Value.value = self.Object.value.Matrix.value.get_scale().length()

    @field_has_changed(Size)
    def size_changed(self):
        if self.Object.value is None:
            return
        original_mat = self.Object.value.Matrix.value
        trans_mat = avango.osg.make_trans_mat(original_mat.get_translate())
        inv_trans_mat = avango.osg.Matrix()
        inv_trans_mat.invert(trans_mat)
        old_scale_mat = avango.osg.make_scale_mat(original_mat.get_scale().length())
        inv_old_scale_mat = avango.osg.Matrix()
        inv_old_scale_mat.invert(old_scale_mat)
        new_scale_mat = avango.osg.make_scale_mat(self.Size.value)
        self.MatrixOut.value = original_mat * inv_trans_mat * inv_old_scale_mat * new_scale_mat * trans_mat


# create behavior instances
object_behavior = ObjectBehavior()
catch_all_sphere_behavior = CatchAllSphereContextBehavior()
catch_all_sphere.add_and_init_field(avango.script.SFObject(), "MenuToolBehavior", catch_all_sphere_behavior)

# Start application
viewer.run()
