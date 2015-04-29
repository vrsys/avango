import bpy
from bpy.types import Node

from .. import node_tree
from .. import export_utils


class TranslationMatrix(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = "TranslationMatrix"
    bl_label = "Translation Matrix"

    def init(self, context):
        self.inputs.new("Vec3Socket", "Vector")
        self.outputs.new("MatrixSocket", "Matrix")

    def to_dict(self):
        return {
            'type': 'TranslationMatrix',
            'name': self.name,
            'values': export_utils.export_values(self),
            'field_connections': export_utils.export_links(self)
        }

    def execute(self, x):
        pass

    def update(self):
        pass


def register():
    bpy.utils.register_class(TranslationMatrix)


def unregister():
    bpy.utils.unregister_class(TranslationMatrix)
