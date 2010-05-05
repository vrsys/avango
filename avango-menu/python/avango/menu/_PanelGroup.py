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

import avango.menu._PanelRotator
import avango.menu.Preferences
import avango.script
from avango.script import field_has_changed
import avango.osg
import math

class PanelGroup(avango.script.Script):
    Panels = avango.MFObject() # read only! use add/remove_panel methods
    ActivePanel = avango.SFContainer()
    HighlightNext = avango.SFBool()
    HighlightPrev = avango.SFBool()
    ValueUp = avango.SFBool()
    ValueDown = avango.SFBool()
    EntrySelect = avango.SFBool()
    GoBack = avango.SFBool()
    Close = avango.SFBool()
    PlacementTool = avango.SFContainer()

    def __init__(self):
        self.super(PanelGroup).__init__()

        self.root = avango.osg.nodes.MatrixTransform()
        self.panels = []
        self.panels_dirty = False
        self.tool = None
        self.active_transitions = []
        self.Panels.value = []
        self.HighlightNext.value = False
        self.HighlightPrev.value = False
        self.ValueUp.value = False
        self.ValueDown.value = False
        self.EntrySelect.value = False
        self.GoBack.value = False
        self.Close.value = False

    def add_panel(self, panel):
        if panel not in self.panels:
            self.root.Children.value.append(panel.root)
            self.panels.append(panel)
            panel.Group.value = self
        self.panels_dirty = True

    def add_panels(self, panellist):
        for panel in panellist:
            self.add_panel(panel)

    def remove_panel(self, panel):
        self.root.Children.value.remove(panel.root)
        self.panels.remove(panel)
        if self.ActivePanel.value==panel:
            self.set_active_panel(None)
        self.panels_dirty = True

    def remove_panels(self, panellist):
        for panel in panellist:
            self.remove_panel(panel)

    def set_tool(self, tool):
        if self.tool is not None:
            self.remove_tool()
        self.tool = tool
        if self.ActivePanel.value is not None:
            self.set_tool_enable(True)

    def remove_tool(self):
        if self.tool is not None:
            self.set_tool_enable(False)
            self.tool = None

    def show_panel(self, panel):
        for p in self.panels:
            p.Visible.value = panel==p
        panel.root.Matrix.value = avango.osg.make_ident_mat()
        self.set_active_panel(panel)
        panel.ParentPanel.value = None

    def show_panel_by_name(self, name):
        for p in self.panels:
            p.Visible.value = p.Name.value==name
            if p.Name.value==name:
                p.root.Matrix.value = avango.osg.make_ident_mat()
                self.set_active_panel(p)
                p.ParentPanel.value = None

    def show_placed_panel(self, panel, position, placement_tool=None):
        if placement_tool is None:
            placement_tool = self.PlacementTool.value
        if placement_tool is not None:
            placement_tool.place(self.root, panel, position)
        else:
            print "PanelGroup: Please set a placement tool before placing a panel"
        self.show_panel(panel)

    def hide_panel(self, panel):
        panel.Visible.value = False
        self.set_active_panel(None)

    def hide_panels(self):
        for p in self.panels:
            if p.Visible.value:
                p.Visible.value = False
        self.set_active_panel(None)
        self.abort_transitions()

    def show_sub(self, panel):
        if len(self.active_transitions)>0:
            self.abort_transitions()

        panel.ParentPanel.value = self.ActivePanel.value

        t1 = avango.menu._PanelRotator.PanelRotator(Duration=avango.menu.Preferences.panel_transition_duration,
                                                    StartAngle=0,
                                                    EndAngle=math.pi,
                                                    Axis=avango.osg.Vec3(0,1,0),
                                                    Panel=self.ActivePanel.value,
                                                    EndVisible=False,
                                                    Caller=self)

        t2 = avango.menu._PanelRotator.PanelRotator(Duration=avango.menu.Preferences.panel_transition_duration,
                                                    StartAngle=-math.pi,
                                                    EndAngle=0,
                                                    Axis=avango.osg.Vec3(0,1,0),
                                                    Panel=panel,
                                                    EndVisible=True,
                                                    Caller=self)

        self.active_transitions = [t1, t2]
        self.set_active_panel(panel)

    def show_parent(self, panel):
        if len(self.active_transitions)>0:
            self.abort_transitions()

        t1 = avango.menu._PanelRotator.PanelRotator(Duration=avango.menu.Preferences.panel_transition_duration,
                                                    StartAngle=0,
                                                    EndAngle=-math.pi,
                                                    Axis=avango.osg.Vec3(0,1,0),
                                                    Panel=self.ActivePanel.value,
                                                    EndVisible=False,
                                                    Caller=self)

        t2 = avango.menu._PanelRotator.PanelRotator(Duration=avango.menu.Preferences.panel_transition_duration,
                                                    StartAngle=math.pi,
                                                    EndAngle=0,
                                                    Axis=avango.osg.Vec3(0,1,0),
                                                    Panel=panel,
                                                    EndVisible=True,
                                                    Caller=self)

        self.active_transitions = [t1, t2]
        self.set_active_panel(panel)


    ###
    ### internal methods
    ###

    def transition_finished(self, transition):
        if transition in self.active_transitions:
            self.active_transitions.remove(transition)

    def abort_transitions(self):
        tlist = self.active_transitions[:]
        for t in tlist:
            t.abort()
            self.active_transitions.remove(t)

    def set_active_panel(self, panel):
        if self.ActivePanel.value is not None:
            self.clear_panel(self.ActivePanel.value)
        self.ActivePanel.value = panel
        self.set_tool_enable(panel is not None)

    def set_tool_enable(self, state):
        if self.tool is not None:
            self.tool.Enable.value = state

    @field_has_changed(HighlightNext)
    def highlight_next_changed(self):
        if self.ActivePanel.value is not None:
            self.ActivePanel.value.HighlightNext.value = self.HighlightNext.value

    @field_has_changed(HighlightPrev)
    def highlight_prev_changed(self):
        if self.ActivePanel.value is not None:
            self.ActivePanel.value.HighlightPrev.value = self.HighlightPrev.value

    @field_has_changed(ValueUp)
    def value_up_changed(self):
        if self.ActivePanel.value is not None:
            self.ActivePanel.value.ValueUp.value = self.ValueUp.value

    @field_has_changed(ValueDown)
    def value_down_changed(self):
        if self.ActivePanel.value is not None:
            self.ActivePanel.value.ValueDown.value = self.ValueDown.value

    @field_has_changed(EntrySelect)
    def entry_select_changed(self):
        if self.ActivePanel.value is not None:
            self.ActivePanel.value.EntrySelect.value = self.EntrySelect.value

    @field_has_changed(GoBack)
    def go_back_changed(self):
        if self.ActivePanel.value is not None:
            self.ActivePanel.value.GoBack.value = self.GoBack.value

    @field_has_changed(Close)
    def close_changed(self):
        if self.ActivePanel.value is not None:
            self.ActivePanel.value.Close.value = self.Close.value

    def evaluate(self):
        if self.panels_dirty:
            self.update_panels_field()
            self.panels_dirty = False

    def clear_panel(self, panel):
        panel.HighlightNext.value = False
        panel.HighlightPrev.value = False
        panel.ValueUp.value = False
        panel.ValueDown.value = False
        panel.EntrySelect.value = False
        panel.GoBack.value = False
        panel.GoBack.value = False
        panel.Close.value = False

    def update_panels_field(self):
        self.Panels.value = self.panels

    def cleanup(self):
        self.disconnect_all_fields()
        self.ActivePanel.value = None
        self.panels = None
        self.Panels.value = []
        self.PlacementTool.value = None
        self.tool = None
        self.abort_transitions()
        self.active_transitions = None

    def __del__(self):
        if avango.menu.Preferences.print_destruction_of_menu_objects:
            print "PanelGroup deleted"
