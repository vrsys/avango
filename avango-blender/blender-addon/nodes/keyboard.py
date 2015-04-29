import bpy
from bpy.types import Node

from .. import node_tree
from .. import export_utils


class Keyboard(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = "Keyboard"
    bl_label = "Keyboard"

    def init(self, context):
        self.outputs.new("NodeSocketBool", "KeyLeft")
        self.outputs.new("NodeSocketBool", "KeyRight")

    def to_dict(self):
        return {
            'type': 'Keyboard',
            'name': self.name,
            'values': export_utils.export_values(self),
            'field_connections': export_utils.export_links(self)
        }

    def update(self):
        pass


def register():
    bpy.utils.register_class(Keyboard)


def unregister():
    bpy.utils.unregister_class(Keyboard)
