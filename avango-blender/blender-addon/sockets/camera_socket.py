import bpy
from bpy.types import NodeSocket


class CameraSocket(NodeSocket):
    '''Camera node socket type'''
    bl_idname = 'CameraSocketType'
    bl_label = 'Camera Socket'

#    def camera_select(self, context):
#        cams = bpy.data.cameras
#        return [(c.name,c.name,"") for c in cams]
#
#    cameraProperty = bpy.props.EnumProperty(items=camera_select)

  # Optional function for drawing the socket input value
    def draw(self, context, layout, node, text):
#        if self.is_output or self.is_linked:
        layout.label(text)
#        else:
#            layout.prop(self, "cameraProperty", text=text)

  # Socket color
    def draw_color(self, context, node):
        return (1.0, 1.0, 0.8, 1.0)


def register():
    bpy.utils.register_class(CameraSocket)


def unregister():
    bpy.utils.unregister_class(CameraSocket)
