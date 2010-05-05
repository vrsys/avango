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
from avango.script import field_has_changed
import avango.tools
import avango.osg

class Tool(avango.script.Script):
    '''
    Generate a toolset for one user to operate on menus.
    Supports highlighting, dragging and selection of menu entries.
    '''
    PickTrigger = avango.SFBool()
    ContextTrigger = avango.SFBool()
    Transform = avango.osg.SFMatrix()
    MenuRootNode = avango.osg.SFNode()
    Enable = avango.SFBool()
    EnableContext = avango.SFBool()
    SliderInteractor = avango.script.SFObject()

    # This toolset is only allowed to interact with menu panels stored in the Panels field.
    # If Panels field is empty, all menus are allowed
    Panels = avango.MFObject()

    _PickTrigger = avango.SFBool()
    _ContextTrigger = avango.SFBool()

    def __init__(self):
        self.super(Tool).__init__()

        self._panels_dirty = False
        self.PickTrigger.value = False
        self.ContextTrigger.value = False
        self.Enable.value = False
        self.EnableContext.value = True
        self._PickTrigger.value = False
        self._ContextTrigger.value = False

        self.SliderInteractor.value = avango.menu.Preferences.slider_interactor()

        self.pick_selector = avango.tools.nodes.PickSelector()
        self.pick_selector.PickTrigger.connect_from(self._PickTrigger)
        self.pick_selector.PickRayTransform.connect_from(self.Transform)
        self.pick_selector.RootNode.connect_from(self.MenuRootNode)
        self.pick_selector.CreateIntersections.value = True

        self.select_tool = _SelectTool()
        self.select_tool.SliderInteractor.connect_from(self.SliderInteractor)
        self.select_tool.Targets.connect_from(self.pick_selector.SelectedTargets)
        self.select_tool.Panels.connect_from(self.Panels)

        self.menu_drag_selector = _MenuDragSelector()
        self.menu_drag_selector.Targets.connect_from(self.pick_selector.SelectedTargets)

        self.dragtool = avango.tools.nodes.DragTool()
        self.dragtool.DragTransform.connect_from(self.Transform)
        self.dragtool.Targets.connect_from(self.menu_drag_selector.SelectedTargets)

        self.highlight_selector = avango.tools.nodes.PickSelector()
        self.highlight_selector.PickTrigger.connect_from(self.Enable)
        self.highlight_selector.TransitionOnly.value = False
        self.highlight_selector.PickRayTransform.connect_from(self.Transform)
        self.highlight_selector.RootNode.connect_from(self.MenuRootNode)

        self.highlight_tool = _HighlightTool()
        self.highlight_tool.Targets.connect_from(self.highlight_selector.SelectedTargets)
        self.highlight_tool.Panels.connect_from(self.Panels)
        self.highlight_tool.PanelLock.connect_from(self.select_tool.PanelLock)
        self.highlight_tool.PanelLock.connect_from(self.menu_drag_selector.PanelLock)

        self.context_selector = avango.tools.nodes.PickSelector()
        self.context_selector.PickTrigger.connect_from(self._ContextTrigger)
        self.context_selector.PickRayTransform.connect_from(self.Transform)
        self.context_selector.RootNode.connect_from(self.MenuRootNode)
        self.context_selector.CreateIntersections.value = True

        self.context_tool = _ContextTool()
        self.context_tool.Targets.connect_from(self.context_selector.SelectedTargets)

    @field_has_changed(PickTrigger)
    def pick_trigger_changed(self):
        self._PickTrigger.value = self.Enable.value and self.PickTrigger.value

    @field_has_changed(Enable)
    def enable_changed(self):
        self._PickTrigger.value = self.Enable.value and self.PickTrigger.value

    @field_has_changed(ContextTrigger)
    def context_trigger_changed(self):
        self._ContextTrigger.value = self.EnableContext.value and self.ContextTrigger.value

    @field_has_changed(EnableContext)
    def enable_context_changed(self):
        self._ContextTrigger.value = self.EnableContext.value and self.ContextTrigger.value
        
    def set_drag_tool(self, drag_tool):
        self.dragtool.disconnect_all_fields()
        self.dragtool = drag_tool
        self.dragtool.DragTransform.connect_from(self.Transform)
        self.dragtool.Targets.connect_from(self.menu_drag_selector.SelectedTargets)

    def cleanup(self):
        self.disconnect_all_fields()
        self.MenuRootNode.value = None
        self.SliderInteractor.value = None
        self.select_tool.cleanup()
        self.highlight_tool.cleanup()
        self.menu_drag_selector.cleanup()


class _MenuTool(avango.script.Script):
    '''
    Base class for menu specific tools and selectors.
    '''
    Panels = avango.MFObject()
    Targets = avango.tools.MFTargetHolder()
    PanelLock = avango.SFBool()
    PanelVisible = avango.SFBool()

    def __init__(self):
        self.super(_MenuTool).__init__()

        self._targets_dirty = False
        self._lock_dirty = False
        self._selected_object = None
        self._panel = None
        self.PanelLock.value = False
        self.PanelVisible.value = True

    @field_has_changed(Targets)
    def targets_changed(self):
        self._targets_dirty = True

    @field_has_changed(PanelLock)
    def panel_lock_changed(self):
        self._lock_dirty = True

    @field_has_changed(PanelVisible)
    def panel_visible_changed(self):
        self._lock_dirty = True

    def evaluate(self):
        if self._lock_dirty:
            # release lock if panel not locked (device button released)
            # or not visible any more.
            # 'locked and invisible' happens, if a menupanel is closed
            # by clicking a widget (close button or submenu)
            if not self.PanelLock.value or not self.PanelVisible.value:
                self.lock_release()
            self._lock_dirty = False
        if self._targets_dirty:
            self.trigger()
            self._targets_dirty = False

    def trigger(self):
        assert 0, "_MenuTool.trigger() method must be implemented in derived class"

    def lock_release(self):
        assert 0, "_MenuTool.lock_release() method must be implemented in derived class"

    def cleanup(self):
        self.disconnect_all_fields()
        self.Panels.value = []
        self.Targets.value = []
        self._selected_object = None


class _SelectTool(_MenuTool):
    '''
    Tool for selecting (activating) menu entries like buttons, checkboxes etc.
    Locks the menu while Trigger key pressed.
    '''
    SliderInteractor = avango.script.SFObject()

    def trigger(self):
        if len(self.Targets.value)==0:
            if self._selected_object is not None and hasattr(self._selected_object, "deselect"):
                if len(self.Panels.value)==0 or self._selected_object.Proxy.value.Layouter.value.Panel.value in self.Panels.value:
                    self._selected_object.deselect()
                    self._selected_object.TargetHolder.value = None
                    self._selected_object.Caller.value = None
            self._selected_object = None
        for holder in self.Targets.value:
            if hasattr(holder.Target.value, "AvangoMenuBehavior"):
                proxy_behavior = holder.Target.value.AvangoMenuBehavior.value
                self._panel = proxy_behavior.Proxy.value.Layouter.value.Panel.value
                if len(self.Panels.value)==0 or self._panel in self.Panels.value:
                    proxy_behavior.Caller.value = self
                    proxy_behavior.select()
                    proxy_behavior.TargetHolder.value = holder
                    self._selected_object = proxy_behavior
                    self.PanelLock.connect_from(self._panel.Lock)
                    self.PanelVisible.connect_from(self._panel.Visible)

    def lock_release(self):
        self.PanelLock.disconnect()
        self.PanelVisible.disconnect()
        self.PanelLock.value = False
        self.PanelVisible.value = True

    def cleanup(self):
        self.super(_SelectTool).cleanup()
        self.SliderInteractor.value = None
        self.disconnect_all_fields()


class _HighlightTool(_MenuTool):
    '''
    Tool for highlighting menu entries.
    Supports highlight lock and release when menu is locked.
    '''
    def __init__(self):
        self.super(_HighlightTool).__init__()
        self._last_targets = []
        self.clear = False

    @field_has_changed(_MenuTool.Targets)
    def targets_changed(self):
        # The Targets field is updated every frame, if device coordinates change.
        # So we do a check here, if the highlighted target changed from last frame.
        # If it didn't, we do not need to trigger this tool.
        # For performance reasons, it would be better to do this check in the
        # PickSelector node in C++ instead of here... but doing it here is still
        # faster than not doint it at all.
        self._targets_dirty = len(self._last_targets) != len(self.Targets.value)
        if not self._targets_dirty:
            for holder in self.Targets.value:
                self._targets_dirty = self._targets_dirty or (holder.Target.value not in self._last_targets)
        if self._targets_dirty:
            self._last_targets = []
            for holder in self.Targets.value:
                self._last_targets.append(holder.Target.value)

    def trigger(self):
        self.clear = False
        for holder in self.Targets.value:
            if hasattr(holder.Target.value, "AvangoMenuBehavior") \
               and hasattr(holder.Target.value.AvangoMenuBehavior.value, "highlight"):
                proxy_behavior = holder.Target.value.AvangoMenuBehavior.value
                self._panel = proxy_behavior.Proxy.value.Layouter.value.Panel.value
                if len(self.Panels.value)==0 or self._panel in self.Panels.value:
                    if not self.PanelLock.value:
                        proxy_behavior.highlight()
                    self._selected_object = proxy_behavior
            else:
                self.clear = True

        if self.clear or len(self.Targets.value)==0:
            if not self.PanelLock.value and self._selected_object:
                self._selected_object.unhighlight()
            self._selected_object = None

    def lock_release(self):
        if self._selected_object:
            self._selected_object.highlight()
        elif self._panel:
            self._panel.HighlightItem.value = 0
            self._panel = None


class _MenuDragSelector(_MenuTool):
    '''
    Filter an apropriate menu panel out of the Targets list to
    drag menu panels. Locks the menu while dragged.
    '''
    SelectedTargets = avango.tools.MFTargetHolder()

    def trigger(self):
        self.SelectedTargets.value = []
        if self._selected_object:
            self._selected_object.Panel.value.Dragged.value = False
            self._selected_object = None

        for holder in self.Targets.value:
            if hasattr(holder.Target.value,"Panel"):
                object = holder.Target.value
                if object.Panel.value and (len(self.Panels.value)==0 or userobject.panel in self.Panels.value):
                    self.SelectedTargets.value = [avango.tools.nodes.TargetHolder(Target=object.Root.value,
                                                                                  Creator=holder.Creator.value)]
                    object.Panel.value.Dragged.value = True
                    self._selected_object = object
                    self.PanelLock.connect_from(object.Panel.value.Lock)

    def lock_release(self):
        self.PanelLock.disconnect()

    def cleanup(self):
        self.disconnect_all_fields()
        self.SelectedTargets.value = []
        self.super(_MenuDragSelector).cleanup()

class _ContextTool(_MenuTool):
    '''
    Tool for opening a context menu when ContextTrigger is pressed.
    '''
    def trigger(self):
        if len(self.Targets.value)==0:
            return
        holder = self.Targets.value[0]
        if hasattr(holder.Target.value, "MenuToolBehavior") \
           and hasattr(holder.Target.value.MenuToolBehavior.value, "show_context_menu"):
            behavior = holder.Target.value.MenuToolBehavior.value
            behavior.show_context_menu(target_holder=holder)

    def lock_release(self):
        pass
