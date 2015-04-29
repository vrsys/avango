import bpy
from bpy.types import Node

from .. import node_tree
from .. import export_utils


class FloatMath(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = "FloatMath"
    bl_label = "Float Math"

    operators = [
        ("ADD", "add", "Addition of both values", 1),
        ("SUBTRACT", "subtract", "Subtract the second from the first", 2),
        ("MULTIPLY", "multiply", "Multiplication", 3),
        ("DIVIDE", "divide", "Division first by second", 4),
        ("POWER", "power", "first to the power of the second", 5),
        ("MODULO", "modulo", "first modulo second", 6),
        ("ROOT", "n-th root", "second root of the first", 7),
        ("SINUS", "sinus", "sinus of the first, second is amplitude", 8)
    ]

    operator = bpy.props.EnumProperty(
        items=operators,
        name="Operator",
        description="wich operator is applied",
        default="ADD",
        )

    def init(self, context):
        self.inputs.new("NodeSocketFloat", "First")
        self.inputs.new("NodeSocketFloat", "Second")
        self.outputs.new("NodeSocketFloat", "Result")

    def draw_buttons(self, context, layout):
        col = layout.column()
        col.prop(self, 'operator', text='Operator')

    def to_dict(self):
        return {
            'type': 'FloatMath',
            'name': self.name,
            'values': export_utils.export_values(self),
            'field_connections': export_utils.export_links(self),
            'operator': self.operator
        }

    def execute(self, x):
        pass

    def update(self):
        pass


def register():
    bpy.utils.register_class(FloatMath)


def unregister():
    bpy.utils.unregister_class(FloatMath)
