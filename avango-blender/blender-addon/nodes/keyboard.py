import bpy
from bpy.types import Node

from .. import node_tree
from .. import export_utils


class NewKeyOperator(bpy.types.Operator):
    bl_idname = "object.new_key_operator"
    bl_label = "New Key"
    bl_description = "add a new key"

    keys = [
        ("KeyLeft", "Left", "", 1),
        ("KeyRight", "Right", "", 2),
        ("KeyUp", "Up", "actually PageUp", 3),
        ("KeyDown", "Down", "actually PageDown", 4),
    ]

    new_key = bpy.props.EnumProperty(
        items=keys,
        name="Key",
        description="choose key",
        default="KeyUp",
        )


    def execute(self, context):
        self.node.outputs.new("NodeSocketBool", self.new_key)

        return {'FINISHED'}

    def invoke(self, context, event):
        self.node = context.node
        return context.window_manager.invoke_props_dialog(self)

class Keyboard(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = "Keyboard"
    bl_label = "Keyboard"

    def draw_buttons(self, context, layout):
        col = layout.column()
        col.operator('object.new_key_operator')

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
    bpy.utils.register_class(NewKeyOperator)


def unregister():
    bpy.utils.unregister_class(Keyboard)
    bpy.utils.unregister_class(NewKeyOperator)
