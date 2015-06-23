import bpy
from bpy.types import Node

from .. import node_tree
from .. import export_utils


class RotationMatrix(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = "RotationMatrix"
    bl_label = "Rotation Matrix"

    def init(self, context):
        self.inputs.new("NodeSocketFloat", "Angle")
        self.inputs.new("Vec3Socket", "Axis")
        self.outputs.new("MatrixSocket", "Matrix")

    def to_dict(self):
        return {
            'type': 'RotationMatrix',
            'name': self.name,
            'values': export_utils.export_values(self),
            'field_connections': export_utils.export_links(self)
        }

    def execute(self, x):
        pass

    def update(self):
        pass


def register():
    bpy.utils.register_class(RotationMatrix)


def unregister():
    bpy.utils.unregister_class(RotationMatrix)
