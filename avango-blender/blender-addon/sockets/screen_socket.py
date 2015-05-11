import bpy
from bpy.types import NodeSocket
from bpy.props import StringProperty


class ScreenSocket(NodeSocket):
    '''Screen node socket type'''
    bl_idname = 'ScreenSocketType'
    bl_label = 'Screen Socket'

    screen_path = StringProperty(
        description='path to screen node',
        default='/Screen'
        )

    def draw(self, context, layout, node, text):
        if self.is_output or self.is_linked:
            layout.label(text)
        else:
            layout.prop(self, "screen_path", text=text)

    # Socket color
    def draw_color(self, context, node):
        return (0.216, 0.4, 1.0, 0.5)


def register():
    bpy.utils.register_class(ScreenSocket)


def unregister():
    bpy.utils.unregister_class(ScreenSocket)
