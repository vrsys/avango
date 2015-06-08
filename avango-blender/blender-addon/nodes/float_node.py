import bpy
from bpy.types import Node

from .. import node_tree
from .. import export_utils


class FloatNode(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = "FloatInputNode"
    bl_label = "Float"

    def init(self, context):
        self.inputs.new("NodeSocketFloat", "Value")
        self.outputs.new("NodeSocketFloat", "Value")

    def to_dict(self):
        return {
            'type': 'Float',
            'name': self.name,
            'values': export_utils.export_values(self),
            'field_connections': export_utils.export_links(self)
        }

    def execute(self, x):
        return x

    def update(self):
        pass


def register():
        bpy.utils.register_class(FloatNode)


def unregister():
        bpy.utils.unregister_class(FloatNode)
