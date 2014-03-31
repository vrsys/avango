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
Avango NG 3D-Menu
=================

3D-Menu is a three dimensional, graphical user interface for Virtual Reality
applications. It provides a basic widget set like push-buttuns, radio-buttons,
check-boxes, slider, sub-menus etc. that are shown in a floating 2d menu panel.

Here is a short introduction to the included moduls. You can find further
information within the modules and classes.


Modules you might want to use:
------------------------------
widget
    Widgets like buttons, slider etc. Widgets are the "logical" representation
    of menu functionality and are used to "communicate" with your application
    via field connections. One widget can be used in multiple menus.

data
    Standard data directory for "built-in" icon textures and fonts.

Panel
    Panel is the visible "window" of the 3d menu. You can pass multiple widgets
    to a panel, that creates a layouter for each widget that shows up as a
    graphical representation of the widgets functionality.

PanelGroup
    Collect all your panels in a PanelGroup to show & hide panels, place panels
    in your 3D scene, use a rotating animation for sub-menu navigation and to
    use Tools (see below).

Preferences
    All settings of menu elements are configurable via fields. Most of the
    fields are connected to fields within the Preferences module when
    initialized. This makes it easy to change global menu settings.

RadioButtonGroup
    Instantiate a RadioButtonGroup for each interrelated radio-button widget
    set and pass it to the radio-button's Group field.

Tool
    A set of tools for interacting and manipulating the menu. It contains a
    select tool, a highlight tool, a drag tool and a context menu tool. Create
    a Tool instance for each user in your single- or multi-user environment.

SliderMouseInteractor
    Every Tool set can have it's own SliderInteractor. This one is best suited
    for typical mouse input in a monitor/desktop-based environment. The mouse
    movement along the slider is directly mapped to the slider.

SliderStylusInteractor
    Same as above, but for a 6DOF stylus device in immersive environments with
    pick ray interaction. To make the interaction independent of slider size
    and distance, the hand-rotation about the Y axis is mapped to a change in
    slider value.

PlacementBase
    For placing a panel via the PanelGroup, you have to specify which placement
    tool has to be used. This basic placement helps placing the panel between
    the clicked point (when opening a context menu) and the viewer, rotate the
    panel to the viewer, resize the menu to a reasonable size and making a ray
    intersection test for occluding objects, moving the panel in front of them
    (optional).

PlacementWall
    Specialized placement for immersive wall-like displays. This placement
    can be configured to know the display dimensions and therefore can
    avoid placing the menu outside the visible area. Additionally the
    minimal rendering size of the text within the menu can be guaranteed
    for better readability. Also has all features of PlacementBase.


Additional modules (internal use)
---------------------------------
layout
    Layouters are the "physical" representation of menu widgets and are
    used internally by the Panel class. Each widget has a *create_layouter*
    method, that returns an instance of an appropriate layouter. Overwrite
    this method if you want to use own layouters for given widgets.

Icon
    Class for creating a switchable set of textured quads. Internally used
    by the layouters for creating the icons.

Label
    Class for creating a text label. Internally used by the layouters for
    creating the menu title and widget text labels.

PanelRotator
    Used internally by PanelGroup for rotating the menus on sub-menu
    navigation. You can easily add additional transmission effects for
    showing or hiding panels or for sub-menu navigation. The infrastructure
    for that is not yet implemented, but simple to add.


Examples
========

There is an easy to learn and well documented example of basic menu use in
examples/3d-menu within your Avango NG installation.
'''

import widget
import Preferences
from _Panel import Panel
from _Tools import Tool
from _RadioButtonGroup import RadioButtonGroup
from _PanelGroup import PanelGroup
from _SliderMouseInteractor import SliderMouseInteractor
from _SliderStylusInteractor import SliderStylusInteractor
from _PlacementBase import PlacementBase
from _PlacementWall import PlacementWall
from _PlacementMonitor import PlacementMonitor
