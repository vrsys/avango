import bpy
from bpy.types import NodeSocket
from bpy.props import FloatVectorProperty


class MatrixSocket(NodeSocket):

    bl_idname = "MatrixSocket"
    bl_label = "Matrix Socket"
    MatProp = FloatVectorProperty(
        name='Mat',
        description='Vector of 16 floats representing the Matrix',
        default=(
            1.0, 0.0, 0.0, 1.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
            ),
        size=16,
        )

    def get_default_value(self):
        if 'MatProp' in self.keys():
            matrix = self['MatProp']
            return [
                matrix[0], matrix[1], matrix[2], matrix[3],
                matrix[4], matrix[5], matrix[6], matrix[7],
                matrix[8], matrix[9], matrix[10], matrix[11],
                matrix[12], matrix[13], matrix[14], matrix[15]
            ]
        else:
            return [
                1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0,
            ]

    def draw(self, context, layout, node, text):
        if self.is_output or self.is_linked:
            layout.label(text)
        else:
            col = layout.column(align=True)
            for i in range(4):
                row = col.row(align=True)
                for j in range(i*4, i*4 + 4):
                    row.prop(self, 'MatProp', text='', index=j)

    def draw_color(self, context, node):
        return(.2, .8, .2, 1.0)


def register():
    bpy.utils.register_class(MatrixSocket)


def unregister():
    bpy.utils.unregister_class(MatrixSocket)
