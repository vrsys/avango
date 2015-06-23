import bpy
from bpy.types import NodeSocket


class AvangoNodeSocket(NodeSocket):
    '''Screen node socket type'''
    bl_idname = 'AvangoNodeSocketType'
    bl_label = 'Avango Node Socket'

    def draw(self, context, layout, node, text):
        #if self.is_output or self.is_linked:
        if self.is_linked:
            layout.label(text)
        else:
            node.draw_socket(self.name, context, layout, text)

    # Socket color
    def draw_color(self, context, node):
        return (0.216, 0.4, 1.0, 0.5)


def register():
    bpy.utils.register_class(AvangoNodeSocket)


def unregister():
    bpy.utils.unregister_class(AvangoNodeSocket)
