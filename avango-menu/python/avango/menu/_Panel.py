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

from avango.menu.widget import WidgetBase
import avango.menu.Preferences
import layout
import avango.osg
import avango.script
from avango.script import field_has_changed

class Panel(WidgetBase):
    '''
Panel Class (derived from WidgetBase)
=====================================

Panel is the visible menu "Window". It consists of a background panel, a menu
title and a vertical widget container. Optionally it contains a close button
and a back button next to the panel title.

A Panel can be added to other menus and containers to provide a hierarchical
menu structure.

Many fields are connected to fields in the menu.Preferences module (tagged with
an asterisk *). Note that Panel has additional fields derived from WidgetBase.

Fields for general panel setup:
-------------------------------
Title (avango.SFString)
    The Panel headline. Set to empty string to remove panel title.

Orientation (avango.SFInt)
    Set to 1 for a vertical widget layout (default), 2 for horizontal

TitleSize (avango.SFFloat) *
    Size of menu headline

TitlePadding (avango.SFFloat) *
    Space around the menu headline

TitleColor (avango.osg.SFVec4) *
    Color of the menu headline

TitleDisabledColor *avango.osg.SFVec4) *
    Color of the menu headline while disabled (Enable=False)

TitleFontname (avango.SFString) *
    Full or relative path to a OSG-compatible font file

BorderWidth (avango.SFFloat) *
    Width of the menu panel border

BorderRadius (avango.SFFloat) *
    Radius of the rounded edges of the menu panel

BorderColor (avango.osg.SFVec4) *
    Color of the menu panel border

PanelColor (avango.osg.SFVec4) *
    Color of the menu panel

PanelPadding (avango.SFFloat) *
    Space between menu panel and the inner content area

ContentPanelColor (avango.osg.SFVec4) *
    Color of the content area

ContentPanelPadding (avango.SFFloat) *
    Space between content area and widgets

ContentPolygonOffset (avango.osg.SFPolygonOffset) *
    PolygonOffset for a proper rendering of the menu content (widget layouters)

ContentCullFace (avango.osg.SFCullFace) *
    CullFace of the menu content (widget layouters)

HighlightColor (avango.osg.SFVec4) *
    Color of the menu highlight

HighlightBorderColor (avango.osg.SFVec4) *
    Color of the menu highlight border

HighlightBorderWidth (avango.SFFloat) *
    Size of the menu highlight border

HighlightPadding (avango.SFFloat) *
    Space around the widgets dimensions for menu highlight

BackButtonFilenames (avango.MFString) *
    List of two file paths to textures for the back button (normal, hover)

CloseButtonFilenames (avango.MFString) *
    List of two file paths to textures for the close button (normal, hover)

DecorationSize (avango.SFFloat) *
    Size of panel decorators (back button, close button)

DecorationPadding (avango.SFFloat) *
    Space around panel decorators (back button, close button)

DecorationEnable (avango.SFBool) *
    Enable/disable the panel decorators (back button, close button)

RemoveHighlightOnHide (avango.SFBool) *
    Remove the highlight (HighlightItem=0) when menu is hidden. If True, the
    re-opened Panel does not highlight any widget. When used with keyboard
    input, it might be useful to remember the last highlighted item.


Fields for panel setup as a sub-menu widget:
--------------------------------------------
IconFilenames (avango.MFString) *
    List of one path to an OSG-compatible texture file

IconSize (avango.SFFloat) *
    Size of the icon

IconPadding (avango.SFFloat) *
    Space around the icon

IconColor (avango.osg.SFVec4) *
    Color of the icon. This color is multiplied with the icon texture and
    should be white for a full color icon.

IconDisabledColor (avango.osg.SFVec4) *
    Color if the icon while Panel is disabled (Enable=False). This color is
    multiplied with the icon texture and should be gray for a dimmed color
    icon.

TextSize (avango.SFFloat) *
    Size of the sub-menu text label

TextColor (avango.osg.SFVec4) *
    Color of the sub-menu text label

TextDisabledColor (avango.osg.SFVec4) *
    Color of the sub-menu text label while Panel is disabled (Enable=False)

TextFontname (avango.SFString) *
    Path to an OSG-compatible font file

ArrowFilenames (avango.MFString) *
    List of one path to an OSG-compatible texture file. This is used for the
    symbol at the right of the widget to mark it as a sub-menu

ArrowSize (avango.SFFloat) *
    Size of sub-menu symbol

ArrowPadding (avango.SFFloat) *
    Space around the sub-menu symbol


Fields for keyboard input:
--------------------------
As it is recommended to use a PanelGroup for interacting with the menu, you
usually should connect your key inputs to the PanelGroup. The key states
are then "forwarded" to the visible menu panel.

HighlightNext (avango.SFBool)
    Highlight the next widget (for keyboard input)

HighlightPrev (avango.SFBool)
    Highlight the previous widget (for keyboard input)

EntrySelect (avango.SFBool)
    Select/activate the highlighted widget (for keyboard input)

GoBack (avango.SFBool)
    Return to parent panel, if panel was activated as a sub-menu entry (for keyboard input)

Close (avango.SFBool)
    Close/hide the panel (for keyboard input)

Visible (avango.SFBool)
    Get/set visibility state of the menu panel


Fields for panel states:
------------------------
HighlightItem (avango.SFInt)
    Number of the actually highlighted widget in the menu.

HighlightWidget (avango.SFContainer)
    The actually highlighted widget

Dragged (avango.SFBool)
    Locks panel while dragged (used by menu.Tool)

Transition (avango.SFBool)
    Locks panel while in transition (e.g. sub-menu animation)

Lock (avango.SFBool)
    Overall lock state of the panel. Read-only!

Group (avango.script.SFObject)
    If a Panel is a member in a PanelGroup, the group is stored in this field.

ParentPanel (avango.script.SFObject)
    If a Panel was activated as a sub-menu, the parent panel is stored in this field.

PanelWidth (avango.SFFloat)
    Width of the Panel. Read-only!

PanelHeight (avango.SFFloat)
    Height of the Panel. Read-only!


Private field:
---------------
_ContainerWidth (avango.SFFloat)
    Width of the parent container in sub-menu widget mode. Read-only!
    Used to right-align the arrow icon

_ContainerHeight (avango.SFFloat)
    Height of the parent container in sub-menu widget mode. Read-only!

_HighlightVisible (avango.SFBool)
    Visibility state of the highlight.

_HighlightPosition (avango.osg.SFMatrix)
    Actual position of the highlight within menu panel

_HighlightWidth (avango.SFFloat)
    Actual width of the highlight

_HighlightHeight (avango.SFFloat)
    Actual height of the highlight

_VisibleChild (avango.SFInt)
    Actual value connected to the avango.osg.nodes.Switch node for hiding the Panel.
    '''
    Title = avango.SFString()
    Orientation = avango.SFInt()
    PanelWidth = avango.SFFloat()
    PanelHeight = avango.SFFloat()
    Visible = avango.SFBool()
    HighlightItem = avango.SFInt()
    HighlightWidget = avango.script.SFObject()
    HighlightNext = avango.SFBool()
    HighlightPrev = avango.SFBool()
    EntrySelect = avango.SFBool()
    GoBack = avango.SFBool()
    Close = avango.SFBool()
    Dragged = avango.SFBool()
    Transition = avango.SFBool()
    Lock = avango.SFBool()
    Group = avango.script.SFObject()
    ParentPanel = avango.script.SFObject()

    # preference fields
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

    # fields for menu panel as submenu widget
    IconFilenames = avango.MFString()
    IconSize = avango.SFFloat()
    IconPadding = avango.SFFloat()
    IconColor = avango.osg.SFVec4()
    IconDisabledColor = avango.osg.SFVec4()
    TextSize = avango.SFFloat()
    TextColor = avango.osg.SFVec4()
    TextDisabledColor = avango.osg.SFVec4()
    TextFontname = avango.SFString()
    ArrowFilenames = avango.MFString()
    ArrowSize = avango.SFFloat()
    ArrowPadding = avango.SFFloat()

    # private fields
    _ContainerWidth = avango.SFFloat()
    _ContainerHeight = avango.SFFloat()
    _HighlightVisible = avango.SFBool()
    _HighlightPosition = avango.osg.SFMatrix()
    _HighlightWidth = avango.SFFloat()
    _HighlightHeight = avango.SFFloat()

    _VisibleChild = avango.SFInt()

    def __init__(self):
        self.super(Panel).__init__()

        self._visible_dirty = False
        self._dimensions_dirty = False
        self._decoration_dirty = False
        self._highlight_dirty = False
        self._entryselect_dirty = False

        self._select_dirty = getattr(self, '_select_dirty', False)
        self._go_back_dirty = False
        self._close_dirty = False
        self._lock_dirty = False
        self._highlight_widget_updated_internally = False

        self._last_select = False
        self._last_go_back = False
        self._last_close = False

        self._layouters = []
        self._highlight_enabled = False

        self.HighlightItem.value = 0 # Off
        self.Dragged.value = False
        self.Transition.value = False

        self.panel = avango.osg.nodes.Panel()
        self.content_panel = avango.osg.nodes.Panel()
        self.highlight_panel = avango.osg.nodes.Panel()
        self.container = avango.menu.layout.ContainerLayouter(Panel=self, ContainerEnable=True)

        self.label = avango.menu._Label.Label()
        self.close_button = avango.menu.layout.PanelDecorationLayouter()
        self.back_button = avango.menu.layout.PanelDecorationLayouter()

        self.panel_geode = avango.osg.nodes.LayerGeode()
        self.panel_transform = avango.osg.nodes.MatrixTransform()
        self.content_transform = avango.osg.nodes.MatrixTransform()
        self.decoration_transform = avango.osg.nodes.MatrixTransform()
        self.switch = avango.osg.nodes.Switch()
        self.root = avango.osg.nodes.MatrixTransform()
        self.root.Children.value = [self.switch]

        # panel_geode dynamic fields for dragging the Panel
        self.panel_geode.add_field(avango.script.SFObject(), "Panel")
        self.panel_geode.Panel.value = self
        self.panel_geode.add_field(avango.SFContainer(), "Root")
        self.panel_geode.Root.value = self.root

        # preferences field connections
        self.TitleSize.connect_from(avango.menu.Preferences.panel.TitleSize)
        self.TitlePadding.connect_from(avango.menu.Preferences.panel.TitlePadding)
        self.TitleColor.connect_from(avango.menu.Preferences.panel.TitleColor)
        self.TitleDisabledColor.connect_from(avango.menu.Preferences.panel.TitleDisabledColor)
        self.TitleFontname.connect_from(avango.menu.Preferences.panel.TitleFontname)
        self.BorderWidth.connect_from(avango.menu.Preferences.panel.BorderWidth)
        self.BorderRadius.connect_from(avango.menu.Preferences.panel.BorderRadius)
        self.BorderColor.connect_from(avango.menu.Preferences.panel.BorderColor)
        self.PanelColor.connect_from(avango.menu.Preferences.panel.PanelColor)
        self.PanelPadding.connect_from(avango.menu.Preferences.panel.PanelPadding)
        self.ContentPanelColor.connect_from(avango.menu.Preferences.panel.ContentPanelColor)
        self.ContentPanelPadding.connect_from(avango.menu.Preferences.panel.ContentPanelPadding)
        self.ContentPolygonOffset.connect_from(avango.menu.Preferences.panel.ContentPolygonOffset)
        self.ContentCullFace.connect_from(avango.menu.Preferences.panel.ContentCullFace)
        self.HighlightColor.connect_from(avango.menu.Preferences.panel.HighlightColor)
        self.HighlightBorderColor.connect_from(avango.menu.Preferences.panel.HighlightBorderColor)
        self.HighlightBorderWidth.connect_from(avango.menu.Preferences.panel.HighlightBorderWidth)
        self.HighlightPadding.connect_from(avango.menu.Preferences.panel.HighlightPadding)
        self.CloseButtonFilenames.connect_from(avango.menu.Preferences.panel.CloseButtonFilenames)
        self.BackButtonFilenames.connect_from(avango.menu.Preferences.panel.BackButtonFilenames)
        self.DecorationSize.connect_from(avango.menu.Preferences.panel.DecorationSize)
        self.DecorationPadding.connect_from(avango.menu.Preferences.panel.DecorationPadding)
        self.DecorationEnable.connect_from(avango.menu.Preferences.panel.DecorationEnable)
        self.RemoveHighlightOnHide.connect_from(avango.menu.Preferences.panel.RemoveHighlightOnHide)

        # preference field connection for submenu widget
        self.IconFilenames.value = avango.menu.Preferences.panel_icon
        self.IconSize.connect_from(avango.menu.Preferences.widget.IconSize)
        self.IconPadding.connect_from(avango.menu.Preferences.widget.IconPadding)
        self.IconColor.connect_from(avango.menu.Preferences.widget.IconColor)
        self.IconDisabledColor.connect_from(avango.menu.Preferences.widget.IconDisabledColor)
        self.TextSize.connect_from(avango.menu.Preferences.widget.TextSize)
        self.TextColor.connect_from(avango.menu.Preferences.widget.TextColor)
        self.TextDisabledColor.connect_from(avango.menu.Preferences.widget.TextDisabledColor)
        self.TextFontname.connect_from(avango.menu.Preferences.widget.TextFontname)
        self.ArrowSize.connect_from(avango.menu.Preferences.widget.SubMenuArrowSize)
        self.ArrowPadding.connect_from(avango.menu.Preferences.widget.SubMenuArrowPadding)
        self.ArrowFilenames.connect_from(avango.menu.Preferences.widget.SubMenuArrowFilenames)

        self.setup_panel()
        self.setup_content_panel()
        self.setup_highlight_panel()
        self.setup_container()
        self.setup_label()
        self.setup_close_button()
        self.setup_back_button()
        self.setup_node_tree()

        self.Visible.value = False # new panels are always invisible after creation
        self.Close.value = False
        self.GoBack.value = False
        self.HighlightNext.value = False
        self.HighlightPrev.value = False
        self.ValueUp.value = False
        self.ValueDown.value = False
        self.EntrySelect.value = False
        self.Select.value = False
        self.Dragged.value = False
        self.Transition.value = False
        self.Lock.value = True

        self._VisibleChild.value = False

        self.switch.VisibleChild.connect_from(self._VisibleChild)

        # Trigger muted field_has_changed events
        self.enable_changed()

    def setup_panel(self):
        self.panel.Width.connect_from(self.PanelWidth)
        self.panel.Height.connect_from(self.PanelHeight)
        self.panel.PanelColor.connect_from(self.PanelColor)
        self.panel.BorderColor.connect_from(self.BorderColor)
        self.panel.BorderWidth.connect_from(self.BorderWidth)
        self.panel.EdgeRadius.connect_from(self.BorderRadius)

    def setup_content_panel(self):
        self.content_panel.PanelColor.connect_from(self.ContentPanelColor)
        self.content_panel.EdgeSmooth.value = 1 # rectangle without rounded corners
        self.content_panel.BorderWidth.value = 0.0

    def setup_highlight_panel(self):
        self.highlight_panel.PanelColor.connect_from(self.HighlightColor)
        self.highlight_panel.BorderColor.connect_from(self.HighlightBorderColor)
        self.highlight_panel.BorderWidth.connect_from(self.HighlightBorderWidth)
        self.highlight_panel.EdgeSmooth.value = 1 # rectangle without rounded corners

    def setup_container(self):
        self._ContainerWidth.connect_from(self.container.Width)
        self._ContainerHeight.connect_from(self.container.Height)
        self.content_transform.Children.value.append(self.container.root)

    def setup_label(self):
        self.label.Fontname.connect_from(self.TitleFontname)
        self.label.Size.connect_from(self.TitleSize)
        self.Title.value = avango.menu.Preferences.panel_title
        self.label.String.connect_from(self.Title)
        self.label.Alignment.value = 3 # center-top

    def setup_close_button(self):
        self.close_button.IconFilenames.connect_from(self.CloseButtonFilenames)
        self.close_button.IconSize.connect_from(self.DecorationSize)
        self.close_button.Panel.value = self
        self.close_button.callback = self.close

    def setup_back_button(self):
        self.back_button.IconFilenames.connect_from(self.BackButtonFilenames)
        self.back_button.IconSize.connect_from(self.DecorationSize)
        self.back_button.Panel.value = self
        self.back_button.callback = self.go_back

    def setup_node_tree(self):
        '''
        NOTE ON FUTURE IMPLEMENTATION:
        As soon as there is a working implementation of LayerGroup, it is not
        necessary anymore to use PolygonOffset or a higher RenderBin. Simply
        add a LayerGroup as child of switch, with StateSet(RenderingHint=2,
        LightingMode=0, BlendMode=1) and make self.panel_geode a normal Geode.
        Then remove the StateSets from content/decoration_transform and panel_geode.
        That should be sufficient for the whole menu rendering.
        '''
        self.content_transform.StateSet.value = avango.osg.nodes.StateSet(
                                                  PolygonOffsetMode=1,
                                                  RenderingHint=2, # depth sorted bin
                                                  LightingMode=0,  # off
                                                  BlendMode=1,     # transparent
                                                  RenderBin=12,    # higher bin for correct sorting
                                                  CullFaceMode=1,
                                                  )
        self.decoration_transform.StateSet.value = avango.osg.nodes.StateSet(
                                                     PolygonOffsetMode=1,
                                                     RenderingHint=2, # depth sorted bin
                                                     LightingMode=0,  # off
                                                     BlendMode=1,     # transparent
                                                     RenderBin=12,    # higher bin for correct sorting
                                                     CullFaceMode=1,
                                                     )

        self.content_transform.StateSet.value.PolygonOffset.connect_from(self.ContentPolygonOffset)
        self.content_transform.StateSet.value.CullFace.connect_from(self.ContentCullFace)
        self.decoration_transform.StateSet.value.PolygonOffset.connect_from(self.ContentPolygonOffset)
        self.decoration_transform.StateSet.value.CullFace.connect_from(self.ContentCullFace)

        self.panel_geode.Drawables.value = [self.panel,
                                            self.content_panel]
        self.panel_geode.StateSet.value = avango.osg.nodes.StateSet(RenderingHint=2, # depth sorted bin
                                                                    LightingMode=0,  # off
                                                                    BlendMode=1,)    # transparent
        self.panel_transform.Children.value = [self.panel_geode,
                                               self.content_transform,
                                               self.decoration_transform]

        self.decoration_transform.Children.value.append(self.label.root)

        if self.DecorationEnable.value:
            self.decoration_transform.Children.value.append(self.close_button.root)
        self.switch.Children.value = [self.panel_transform]

    @field_has_changed(_ContainerWidth)
    def container_width_changed(self):
        self._dimensions_dirty = True

    @field_has_changed(_ContainerHeight)
    def container_height_changed(self):
        self._dimensions_dirty = True

    @field_has_changed(Orientation)
    def orientation_changed(self):
        self.container.Orientation.value = self.Orientation.value

    @field_has_changed(Title)
    def title_changed(self):
        self._dimensions_dirty = True

    @field_has_changed(TitleSize)
    def title_size_changed(self):
        self._dimensions_dirty = True

    @field_has_changed(TitlePadding)
    def title_padding_changed(self):
        self._dimensions_dirty = True

    @field_has_changed(TitleFontname)
    def title_fontname_changed(self):
        self._dimensions_dirty = True

    @field_has_changed(PanelPadding)
    def panel_padding_changed(self):
        self._dimensions_dirty = True

    @field_has_changed(ContentPanelPadding)
    def panel_padding_changed(self):
        self._dimensions_dirty = True

    @field_has_changed(HighlightPadding)
    def highlight_padding_changed(self):
        self._highlight_dirty = True

    @field_has_changed(HighlightItem)
    def highlight_changed(self):
        self._highlight_dirty = True

    @field_has_changed(HighlightWidget)
    def highlight_widget_changed(self):
        self.update_highlight_from_widget()
        self._highlight_dirty = True

    @field_has_changed(HighlightNext)
    def highlight_next_changed(self):
        if self.HighlightNext.value:
            self.highlight_next()

    @field_has_changed(HighlightPrev)
    def highlight_prev_changed(self):
        if self.HighlightPrev.value:
            self.highlight_prev()

    @field_has_changed(WidgetBase.ValueUp)
    def value_up_changed(self):
        if self.HighlightWidget.value is not None:
            self.HighlightWidget.value.ValueUp.value = self.ValueUp.value


    @field_has_changed(WidgetBase.ValueDown)
    def value_down_changed(self):
        if self.HighlightWidget.value is not None:
            self.HighlightWidget.value.ValueDown.value = self.ValueDown.value

    @field_has_changed(EntrySelect)
    def entry_select_changed(self):
        self._entryselect_dirty = True
        self._lock_dirty = True

    @field_has_changed(WidgetBase.Select)
    def select_changed(self):
        self._select_dirty = True

    @field_has_changed(GoBack)
    def go_back_changed(self):
        if not self._last_go_back and self.GoBack.value:
            self._go_back_dirty = True
        self._last_go_back = self.GoBack.value

    @field_has_changed(Close)
    def close_changed(self):
        if not self._last_close and self.Close.value:
            self._close_dirty = True
        self._last_close = self.Close.value

    @field_has_changed(Transition)
    def transition_changed(self):
        self._lock_dirty = True

    @field_has_changed(Dragged)
    def dragged_changed(self):
        self._lock_dirty = True

    @field_has_changed(Visible)
    def visible_changed(self):
        self._visible_dirty = True

    @field_has_changed(Group)
    def group_changed(self):
        if self.Group.value is not None:
            self.panel_geode.Root.value = self.Group.value.root
        else:
            self.panel_geode.Root.value = self.root

    @field_has_changed(ParentPanel)
    def parent_panel_changed(self):
        self._decoration_dirty = True

    @field_has_changed(DecorationEnable)
    def decoration_enable_changed(self):
        self._dimensions_dirty = True
        self._decoration_dirty = True

    @field_has_changed(WidgetBase.Enable)
    def enable_changed(self):
        if not hasattr(self, "label"):
            return
        self.label.Color.disconnect()
        if self.Enable.value:
            self.label.Color.connect_from(self.TitleColor)
        else:
            self.label.Color.connect_from(self.TitleDisabledColor)
        self.container.WidgetEnable.value = self.Enable.value

    def evaluate(self):
        if self._visible_dirty:
            self.update_visibility()
        if self._decoration_dirty:
            self.update_decoration()
        if self._dimensions_dirty:
            self.update_dimensions()
        if self._highlight_dirty:
            self.update_highlight()
        if self._entryselect_dirty:
            self.entry_select()
        if self._select_dirty:
            self.select()
        if self._go_back_dirty:
            self.go_back()
        if self._close_dirty:
            self.close()
        if self._lock_dirty:
            self.update_lock()

    def update_visibility(self):
        if self.Visible.value:
            self._VisibleChild.value = 0 # all children on
        else:
            self._VisibleChild.value = -1 # all children off
            self.close_button.Highlight.value = False
            self.back_button.Highlight.value = False
            if self.RemoveHighlightOnHide.value:
                self.HighlightItem.value = 0
        self._lock_dirty = True
        self._visible_dirty = False

    def update_dimensions(self):
        '''
        Calculate and set size and placement of menu components:
        menu panel, content panel, menu title, content position
        '''
        # calculate height of menu title
        if self.label.text.get_bounding_box().valid and self.Title.value!="":
            label_height = 2 * self.TitlePadding.value + self.label.get_height()
        else:
            label_height = self.PanelPadding.value

        # calculate content width with respect to menu title width
        label_width = self.label.get_width()
        decoration_width = 2 * self.DecorationSize.value + 4 * self.DecorationPadding.value - 2 * self.ContentPanelPadding.value
        content_width = self._ContainerWidth.value + 2 * self.ContentPanelPadding.value

        # calculate titlebar dimensions with respect to panel decorations
        if self.DecorationEnable.value:
            titlebar_height = max(label_height, self.DecorationSize.value + 2 * self.DecorationPadding.value)
            titlebar_width = label_width + decoration_width
        else:
            titlebar_height = label_height
            titlebar_width = label_width

        width = max(titlebar_width, content_width)

        # if panel is wider than inner container
        if (titlebar_width - 0.0001) > self.container.Width.value:
            self.container.PanelBaseWidth.value = titlebar_width

        # height and width of menu panel and content panel
        self.content_panel.Width.value = width
        self.PanelWidth.value = self.content_panel.Width.value + 2 * self.PanelPadding.value
        self.content_panel.Height.value = self._ContainerHeight.value + 2 * self.ContentPanelPadding.value
        self.PanelHeight.value = self.content_panel.Height.value + self.PanelPadding.value + titlebar_height

        # position of content panel
        self.content_panel.Position.value = avango.osg.Vec3(0, (titlebar_height - self.PanelPadding.value) / -2, 0)
        offset_x = width / -2 + self.ContentPanelPadding.value
        offset_y = self._ContainerHeight.value / 2 + (titlebar_height - self.PanelPadding.value) / -2
        self.content_transform.Matrix.value = avango.osg.make_trans_mat(offset_x, offset_y, 0)

        # position of panel title and decoration
        self.label.WidgetHeight.value = titlebar_height
        offset = avango.osg.Vec3(0, self.PanelHeight.value / 2 - self.DecorationPadding.value, 0)
        self.decoration_transform.Matrix.value = avango.osg.make_trans_mat(offset)
        offset = avango.osg.Vec3(self.PanelWidth.value / -2 + self.DecorationPadding.value, 0, 0)
        self.back_button.root.Matrix.value = avango.osg.make_trans_mat(offset)
        offset = avango.osg.Vec3(self.PanelWidth.value / 2 - self.DecorationPadding.value - self.DecorationSize.value, 0, 0)
        self.close_button.root.Matrix.value = avango.osg.make_trans_mat(offset)

        # place pivot point of menu top center
        self.panel_transform.Matrix.value = avango.osg.make_trans_mat(0, self.PanelHeight.value / -2, 0)
        self._layouters = []
        self._highlight_dirty = True
        self._dimensions_dirty = False

    def update_decoration(self):
        if self.DecorationEnable.value:
            if self.ParentPanel.value is not None:
                if self.back_button.root not in self.decoration_transform.Children.value:
                    self.decoration_transform.Children.value.append(self.back_button.root)
                else:
                    self.decoration_transform.Children.value.remove(self.close_button.root)
            if self.close_button.root not in self.decoration_transform.Children.value:
                self.decoration_transform.Children.value.append(self.close_button.root)
        else:
            self.decoration_transform.Children.value.remove(self.close_button.root)
            self.decoration_transform.Children.value.remove(self.back_button.root)
        self._decoration_dirty = False

    def update_layouters(self):
        if len(self._layouters) == 0:
            self._layouters = self.container.get_widget_layouters()
            index = 1
            for layouter in self._layouters:
                layouter.entry_number = index
                index += 1

    def force_update_layouters(self):
        self._layouters = []

    def get_layouters(self):
        self.update_layouters()
        return self._layouters

    def update_highlight(self):
        self.update_layouters()

        if self.HighlightItem.value == 0 or (len(self._layouters) < self.HighlightItem.value):
            if self.HighlightWidget.value is not None:
                if self.HighlightWidget.value.Highlighted.value:
                    self.HighlightWidget.value.Highlighted.value = False
            self.enable_highlight(False)
            return

        self.enable_highlight(True)

        # update HighlightWidget field
        self._highlight_widget_updated_internally = True
        layouter = self._layouters[self.HighlightItem.value - 1]
        if self.HighlightWidget.value != layouter.Widget.value:
            self.HighlightWidget.value = layouter.Widget.value

        # calculate position and size of highlight
        if layouter.Container.value.Orientation.value == 1: # Vertical
            width = layouter.Container.value.Width.value
            height = layouter.Height.value
        elif layouter.Container.value.Orientation.value == 2: # Horizontal
            width = layouter.Width.value
            height = layouter.Container.value.Height.value

        offset = self.content_transform.Matrix.value.get_translate() + layouter.position
        offset += avango.osg.Vec3(width/2, height/-2, 0)
        self.highlight_panel.Position.value = offset
        self.highlight_panel.Width.value = width + self.HighlightPadding.value
        self.highlight_panel.Height.value = height + self.HighlightPadding.value

        # unhighlight all widgets if they are still highlighted
        for item in self._layouters:
            if item.Widget.value.Highlighted.value:
                item.Widget.value.Highlighted.value = False
        self.close_button.Highlight.value = False
        self.back_button.Highlight.value = False

        # highlight widget if it's not highlighted yet
        if not layouter.Widget.value.Highlighted.value:
            layouter.Widget.value.Highlighted.value = True

        self._highlight_dirty = False

    def update_highlight_from_widget(self):
        if self._highlight_widget_updated_internally:
            self._highlight_widget_updated_internally = False
            return
        self.update_layouters()
        index = 1
        for item in self._layouters:
            if self.HighlightWidget.value == item.Widget.value and self.HighlightItem.value != index:
                self.HighlightItem.value = index
            index += 1

    def enable_highlight(self, state):
        if not state and self._highlight_enabled:
            self._highlight_enabled = False
            self.panel_geode.Drawables.value.remove(self.highlight_panel)
        if state and not self._highlight_enabled:
            self._highlight_enabled = True
            self.panel_geode.Drawables.value.append(self.highlight_panel)

    def highlight_next(self):
        if self.Lock.value:
            return
        self.update_layouters()
        self.HighlightItem.value += 1
        if self.HighlightItem.value > len(self._layouters):
            self.HighlightItem.value = 1
        self._highlight_dirty = True

    def highlight_prev(self):
        if self.Lock.value:
            return
        self.update_layouters()
        self.HighlightItem.value -= 1
        if self.HighlightItem.value <= 0:
            self.HighlightItem.value = len(self._layouters)
        self._highlight_dirty = True

    def entry_select(self):
        '''activate a widget and set its 'Select' state if it has changed'''
        self._entryselect_dirty = False
        if self.Lock.value:
            if not self.EntrySelect.value and self.HighlightItem!=0 and self.HighlightWidget.value is not None \
               and self.HighlightWidget.value.Select.value != self.EntrySelect.value:
                self.HighlightWidget.value.Select.value = self.EntrySelect.value
            return
        if self.HighlightItem != 0 and self.HighlightWidget.value is not None \
           and self.HighlightWidget.value.Select.value != self.EntrySelect.value:
            self.HighlightWidget.value.Select.value = self.EntrySelect.value

    def select(self):
        '''show this panel as sub menu'''
        if not self._last_select and self.Select.value and self.Group.value is not None:
            self.Group.value.show_sub(self)
        self._select_dirty = False
        self._last_select = self.Select.value

    def go_back(self):
        '''show parent panel'''
        if self.ParentPanel.value is not None:
            if self.Group.value is not None:
                self.Group.value.show_parent(self.ParentPanel.value)
            else:
                self.ParentPanel.Visible.value = True
                self.Visible.value = False
        self._go_back_dirty = False

    def close(self):
        if self.Group.value is not None:
            self.Group.value.hide_panel(self)
        else:
            self.Visible.value = False

    def add_widget(self, widget):
        self._layouters = []
        self.HighlightItem.value = 0
        self._highlight_dirty = True
        self.container.add_widget(widget)

    def add_widgets(self, widgetlist):
        self._layouters = []
        self.HighlightItem.value = 0
        self._highlight_dirty = True
        self.container.add_widgets(widgetlist)

    def remove_widget(self, widget):
        self._layouters = []
        self.HighlightItem.value = 0
        self._highlight_dirty = True
        self.container.remove_widget(widget)

    def remove_widgets(self, widgetlist):
        self._layouters = []
        self.HighlightItem.value = 0
        self._highlight_dirty = True
        self.container.remove_widgets(widgetlist)

    def remove_all_widgets(self):
        self._layouters = []
        self.HighlightItem.value = 0
        self.HighlightWidget.value = None
        self._highlight_dirty = True
        self.container.remove_all_widgets()

    def update_lock(self):
        self.Lock.value = self.EntrySelect.value or self.Transition.value or self.Dragged.value or (not self.Visible.value)
        self._lock_dirty = False

    def create_layouter(self):
        layouter = avango.menu.layout.SubMenuLayouter(Widget = self)
        return layouter

    def cleanup(self):
        self.panel.disconnect_all_fields()
        self.content_panel.disconnect_all_fields()
        self.highlight_panel.disconnect_all_fields()
        self.label.disconnect_all_fields()
        self.content_transform.disconnect_all_fields()
        self.content_transform.StateSet.value.disconnect_all_fields()
        if self.root.StateSet.value is not None:
            self.root.StateSet.value.disconnect_all_fields()
        self.decoration_transform.disconnect_all_fields()
        self.decoration_transform.StateSet.value.disconnect_all_fields()
        self.disconnect_all_fields()
        self.close_button.cleanup()
        self.back_button.cleanup()
        if self.container is not None:
            self.container.cleanup()
        if self.panel_geode is not None:
            self.panel_geode.Panel.value = None
            self.panel_geode.Root.value = None
        self._layouters = []

        self.HighlightWidget.value = None
        self.super(Panel).cleanup()

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "Panel deleted"
