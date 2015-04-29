import bpy
from bpy.types import Node

from .. import node_tree


class TimeSensor(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = "TimeSensor"
    bl_label = "TimeSensor"

    def init(self, context):
        self.outputs.new("NodeSocketFloat", "RealTime")
        self.outputs.new("NodeSocketFloat", "ReferenceTime")
        self.outputs.new("NodeSocketFloat", "Time")

    def update(self):
        pass


def register():
    bpy.utils.register_class(TimeSensor)


def unregister():
    bpy.utils.unregister_class(TimeSensor)
