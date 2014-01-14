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

import avango.osg
import avango.script
from _SliderMouseInteractor import *
from os.path import dirname, join

# get path
datapath = join(dirname(__file__), 'data')


# some debug settings
print_destruction_of_menu_objects = False
enable_debug_output = False

# Default Settings
panel_transition_duration = 0.5 # time for rotation of submenu navigation

panel_title = 'Avango NG Menu Panel'
panel_icon = [join(datapath,'submenu.png')]

pushbutton_text = 'PushButton'
pushbutton_icon = [join(datapath,'delete.png')]
pushbutton_close_on_release = False # close menu on button release

checkbox_text = 'CheckBox'
checkbox_icons = [join(datapath,'checkbox_unchecked.png'), join(datapath,'checkbox_checked.png')]

radiobutton_text = 'RadioButton'
radiobutton_icons = [join(datapath,'radiobutton_unchecked.png'), join(datapath,'radiobutton_checked.png')]

slider_text = 'Slider'
slider_icon = [join(datapath,'slider.png')]
slider_min = 0
slider_max = 100
slider_value = 50
slider_interactor = SliderMouseInteractor

class PanelPreferenceFields(avango.script.Script):
    '''
    Default Values for Menu Panels
    '''
    TitleSize = avango.SFFloat()
    TitlePadding = avango.SFFloat()
    TitleColor = avango.osg.SFVec4()
    TitleDisabledColor = avango.osg.SFVec4()
    TitleFontname = avango.SFString()
    BorderWidth = avango.SFFloat()
    BorderRadius = avango.SFFloat()
    BorderColor = avango.osg.SFVec4()
    PanelColor = avango.osg.SFVec4()
    PanelPadding = avango.SFFloat()
    ContentPanelColor = avango.osg.SFVec4()
    ContentPanelPadding = avango.SFFloat()
    ContentPolygonOffset = avango.osg.SFPolygonOffset()
    ContentCullFace = avango.osg.SFCullFace()
    HighlightColor = avango.osg.SFVec4()
    HighlightBorderColor = avango.osg.SFVec4()
    HighlightBorderWidth = avango.SFFloat()
    HighlightPadding = avango.SFFloat()
    BackButtonFilenames = avango.MFString()
    CloseButtonFilenames = avango.MFString()
    DecorationSize = avango.SFFloat()
    DecorationPadding = avango.SFFloat()
    DecorationEnable = avango.SFBool()
    RemoveHighlightOnHide = avango.SFBool()

    def __init__(self):
        self.super(PanelPreferenceFields).__init__()

        self.TitleSize.value = 0.08
        self.TitlePadding.value = 0.02
        self.TitleColor.value = avango.osg.Vec4(0,0,0,1)
        self.TitleDisabledColor.value = avango.osg.Vec4(0.2,0.2,0.2,0.5)
        self.TitleFontname.value = join(datapath,'VeraBI.ttf')
        self.BorderWidth.value = 0.007
        self.BorderRadius.value = 0.02
        self.BorderColor.value = avango.osg.Vec4(0.1, 0.1, 0.1, 0.8)
        self.PanelColor.value = avango.osg.Vec4(0.84, 0.84, 0.84, 0.5)
        self.PanelPadding.value = 0.02
        self.ContentPanelColor.value = avango.osg.Vec4(0.24, 0.24, 0.24, 0.65)
        self.ContentPanelPadding.value = 0.02
        self.ContentPolygonOffset.value = avango.osg.nodes.PolygonOffset(Factor=-4, Units=4)
        self.ContentCullFace.value = avango.osg.nodes.CullFace(Mode=1029) # GL_BACK 1029
        self.HighlightColor.value = avango.osg.Vec4(0.6, 0.6, 0.0, 0.6)
        self.HighlightBorderColor.value = avango.osg.Vec4(1, 1, 0.0, 0.8)
        self.HighlightBorderWidth.value = 0.01
        self.HighlightPadding.value = 0.02 # should be same as ContentPanelPadding
        self.BackButtonFilenames.value = [join(datapath,'panel-back.png'), join(datapath,'panel-back-hl.png')]
        self.CloseButtonFilenames.value = [join(datapath,'panel-close.png'), join(datapath,'panel-close-hl.png')]
        self.DecorationSize.value = 0.12
        self.DecorationPadding.value = 0.01
        self.DecorationEnable.value = True
        self.RemoveHighlightOnHide.value = False


class WidgetPreferenceFields(avango.script.Script):
    '''
    Default Values for Widgets
    '''
    IconSize = avango.SFFloat()
    IconPadding = avango.SFFloat()
    IconColor = avango.osg.SFVec4()
    IconDisabledColor = avango.osg.SFVec4()
    TextSize = avango.SFFloat()
    TextColor = avango.osg.SFVec4()
    TextDisabledColor = avango.osg.SFVec4()
    TextFontname = avango.SFString()
    TextOnly = avango.SFBool()
    DividerThickness = avango.SFFloat()
    DividerColor = avango.osg.SFVec4()
    SubMenuArrowSize = avango.SFFloat()
    SubMenuArrowPadding = avango.SFFloat()
    SubMenuArrowFilenames = avango.MFString()
    SliderTextSize = avango.SFFloat()
    SliderBarFilenames = avango.MFString()
    SliderBarHeight = avango.SFFloat()
    SliderBarMinLength = avango.SFFloat()
    SliderBarMaxLength = avango.SFFloat()
    SliderBarBorderWidth = avango.SFFloat()
    SliderBarBorderColor = avango.osg.SFVec4()
    SliderStep = avango.SFFloat() # step for ValueUp/ValueDown keyboard input

    def __init__(self):
        self.super(WidgetPreferenceFields).__init__()

        self.IconSize.value = 0.1
        self.IconPadding.value = 0.02
        self.IconColor.value = avango.osg.Vec4(1,1,1,1)
        self.IconDisabledColor.value = avango.osg.Vec4(0.6,0.6,0.6,1)
        self.TextSize.value = 0.08
        self.TextColor.value = avango.osg.Vec4(1,1,1,1)
        self.TextDisabledColor.value = avango.osg.Vec4(0.6,0.6,0.6,1)
        self.TextFontname.value = join(datapath,'VeraBd.ttf')
        self.TextOnly.value = False
        self.DividerThickness.value = 0.01
        self.DividerColor.value = avango.osg.Vec4(0.5,0.5,0.5,0.4)
        self.SubMenuArrowSize.value = 0.1 # should be same as IconSize
        self.SubMenuArrowPadding.value = 0.05
        self.SubMenuArrowFilenames.value = [join(datapath,'submenu_arrow.png')]
        self.SliderTextSize.value = 0.07 # less or equal TextSize
        self.SliderBarFilenames.value = [join(datapath,'slider_background.png'), join(datapath,'slider_foreground.png')]
        self.SliderBarHeight.value = 0.08
        self.SliderBarMinLength.value = 1.0
        self.SliderBarMaxLength.value = 999
        self.SliderBarBorderWidth.value = 0.01
        self.SliderBarBorderColor.value = avango.osg.Vec4(0.2,0.2,0.2,1)
        self.SliderStep.value = 1.0


class ContainerPreferenceFields(avango.script.Script):
    '''
    Default Values for Container
    '''
    WidgetVerticalPadding = avango.SFFloat()
    WidgetHorizontalPadding = avango.SFFloat()

    def __init__(self):
        self.super(ContainerPreferenceFields).__init__()
        self.WidgetVerticalPadding.value = 0.03
        self.WidgetHorizontalPadding.value = 0.03


class LayouterPreferenceFields(avango.script.Script):
    '''
    Default Values for Layouter
    '''
    ProxyDisplacement = avango.SFFloat()
    ProxyVerticalPadding = avango.SFFloat()
    ProxyHorizontalPadding = avango.SFFloat()
    ProxyCullFace = avango.osg.SFCullFace()
    ProxyColor = avango.osg.SFVec4()

    def __init__(self):
        self.super(LayouterPreferenceFields).__init__()

        self.ProxyDisplacement.value = 0.003
        self.ProxyVerticalPadding.value = 0.015 # should be half of ContainerPreferenceFields.WidgetVerticalPadding
        self.ProxyHorizontalPadding.value = 0.015 # should be half of ContainerPreferenceFields.WidgetHorizontalPadding
        self.ProxyCullFace.value = avango.osg.nodes.CullFace(Mode=1032) # GL_FRONT_AND_BACK 1032
        self.ProxyColor.value = avango.osg.Vec4(1,0,0,0.1) # for debugging, if Proxy visible


class InteractionPreferenceFields(avango.script.Script):
    '''
    Default Values for Interacting with the menu
    '''
    SliderStylusInteractorFullRangeAngle = avango.SFFloat()

    def __init__(self):
        self.super(InteractionPreferenceFields).__init__()
        self.SliderStylusInteractorFullRangeAngle.value = 30.0


class PlacementPreferenceFields(avango.script.Script):
    '''
    Default Values for Menu Placement
    '''
    MenuSize = avango.SFFloat()
    MinDistance = avango.SFFloat() # minimal distance from viewer
    MaxDistance = avango.SFFloat() # maximum distance from viewer
    ObjectDistance = avango.SFFloat() # minimal distance between clicked point (object) and menu
    FixedRoll = avango.SFBool() # False rotates menu to the viewer (Monitor), True fixes roll axis (Wall)
    EnableRayBasedPlacement = avango.SFBool() # placement by ray intersections; performance vastly depending on scene complexity
    InterpolationDistance = avango.SFFloat() # distance between interpolated points
    WallScreenWidth = avango.SFFloat() # X dimension of wall
    WallScreenHeight = avango.SFFloat() # Y dimension of wall
    WallScreenCenter = avango.osg.SFVec3() # center position of wall
    WallScreenPadding = avango.SFFloat() # safe distance to screen edges for placing the menu
    ScaleDebugSpheres = avango.SFFloat() # in debug mode (Preferences.enable_debug_output=True), scale spheres
    WallMinTextSize = avango.SFFloat() # minimal text rendering size in screen plane

    def __init__(self):
        self.super(PlacementPreferenceFields).__init__()

        self.MenuSize.value = 0.10
        self.MinDistance.value = 0.2
        self.MaxDistance.value = 1.0
        self.ObjectDistance.value = 0.05
        self.FixedRoll.value = False
        self.EnableRayBasedPlacement.value = True
        self.InterpolationDistance.value = 0.3 # something like MenuSize * 2 or 3, depending on scene complexity
        self.WallScreenWidth.value = 3.0 # width of TwoView screen
        self.WallScreenHeight.value = 2.4 # height of TwoView screen
        self.WallScreenCenter.value = avango.osg.Vec3(0,1.2,0) # center of TwoView screen
        self.WallScreenPadding.value = 0.1
        self.ScaleDebugSpheres.value = 1.0
        self.WallMinTextSize.value = 0.03 # 3cm text size, approx. 12 pixels in TwoView


panel = PanelPreferenceFields()
widget = WidgetPreferenceFields()
container = ContainerPreferenceFields()
layouter = LayouterPreferenceFields()
interaction = InteractionPreferenceFields()
placement = PlacementPreferenceFields()
