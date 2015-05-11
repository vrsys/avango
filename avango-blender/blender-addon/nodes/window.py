import bpy
from bpy.types import Node
from bpy.props import StringProperty, IntVectorProperty, BoolProperty

from .. import node_tree


class Window(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = 'Window'
    bl_label = 'Window'

    display_field = StringProperty(
        description='display number',
        default=':0.0',
        )

    title_field = StringProperty(
        description='window title',
        default='beautiful'
        )

    left_size = IntVectorProperty(
        name="Resolution",
        description="size",
        default=(1024, 768),
        min=1,
        size=2,
        )

    left_pos = IntVectorProperty(
        name="Position",
        description="size",
        default=(0, 0),
        min=0,
        size=2,
        )

    enabled = BoolProperty(
        name="enabled",
        description="enabled",
        default=True,
        )

    def init(self, context):
        self.inputs.new('StereoModeSocketType', "StereoMode")
        self.outputs.new('WindowSocketType', 'Window')

    def draw_buttons(self, context, layout):
        col = layout.column()
        col.prop(self, 'name', text='Name')
        col.prop(self, 'title_field', text='Title')
        col.prop(self, 'display_field', text='Display')
        col.prop(self, 'left_size', text='LeftSize')
        col.prop(self, 'left_pos', text='LeftPosition')

    def process(self):
        pass

    def get_args(self):
        pass


def register():
    bpy.utils.register_class(Window)


def unregister():
    bpy.utils.unregister_class(Window)
