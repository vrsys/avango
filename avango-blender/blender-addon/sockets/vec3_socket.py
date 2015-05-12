import bpy
from bpy.types import NodeSocket
from bpy.props import FloatVectorProperty


class Vec3Socket(NodeSocket):

    bl_idname = "Vec3Socket"
    bl_label = "Vec3 Socket"
    VecProp = FloatVectorProperty(
        name='Vec',
        description='Vector of three floats',
        default=(0.0, 0.0, 0.0),
        size=3,
        )

    def get_default_value(self):
        if 'VecProp' in self.keys():
            return [self['VecProp'][0], self['VecProp'][1], self['VecProp'][2]]
        else:
            return [0.0, 0.0, 0.0]

    def draw(self, context, layout, node, text):
        if self.is_output or self.is_linked:
            layout.label(text)
        else:
            col = layout.column()
            col.prop(self, "VecProp", text=text)

    def draw_color(self, context, node):
        return(.8, .2, .2, 1.0)


def register():
    bpy.utils.register_class(Vec3Socket)


def unregister():
    bpy.utils.unregister_class(Vec3Socket)
