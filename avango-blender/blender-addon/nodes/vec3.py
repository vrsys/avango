import bpy
from bpy.types import Node

from .. import node_tree
from .. import export_utils


class Vec3(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = "Vec3"
    bl_label = "Vec3"

    def init(self, context):
        self.inputs.new("NodeSocketFloat", "X")
        self.inputs.new("NodeSocketFloat", "Y")
        self.inputs.new("NodeSocketFloat", "Z")
        self.outputs.new("Vec3Socket", "Vector")

    def to_dict(self):
        return {
            'type': 'Vec3',
            'name': self.name,
            'values': export_utils.export_values(self),
            'field_connections': export_utils.export_links(self)
        }

    def update(self):
        pass


def register():
    bpy.utils.register_class(Vec3)


def unregister():
    bpy.utils.unregister_class(Vec3)
