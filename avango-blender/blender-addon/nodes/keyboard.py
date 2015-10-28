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
        ("KeyQ", "Q", "", 5),
        ("KeyW", "W", "", 6),
        ("KeyE", "E", "", 7),
        ("KeyR", "R", "", 8),
        ("KeyT", "T", "", 9),
        ("KeyZ", "Z", "", 10),
        ("KeyU", "U", "", 11),
        ("KeyI", "I", "", 12),
        ("KeyO", "O", "", 13),
        ("KeyP", "P", "", 14),
        ("KeyA", "A", "", 15),
        ("KeyS", "S", "", 16),
        ("KeyD", "D", "", 17),
        ("KeyF", "F", "", 18),
        ("KeyG", "G", "", 19),
        ("KeyH", "H", "", 20),
        ("KeyJ", "J", "", 21),
        ("KeyK", "K", "", 22),
        ("KeyL", "L", "", 23),
        ("KeyY", "Y", "", 24),
        ("KeyX", "X", "", 25),
        ("KeyC", "C", "", 26),
        ("KeyV", "V", "", 27),
        ("KeyB", "B", "", 28),
        ("KeyN", "N", "", 29),
        ("KeyM", "M", "", 30),
    ]

    new_key = bpy.props.EnumProperty(
        items=keys,
        name="Key",
        description="choose key",
        default="KeyUp", )

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
