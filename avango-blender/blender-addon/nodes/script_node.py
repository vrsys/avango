import bpy
import copy
from bpy.types import Node

from bpy.props import StringProperty,  BoolProperty

from .. import node_tree
from .. import export_utils

SCRIPT_TEMPLATE = """
import avango
import avango.gua
import avango.script
from avango.script import field_has_changed


class %CLASSNAME%(avango.script.Script):

%FIELD_DECLARATIONS%

    def __init__(self):
        self.super(%CLASSNAME%).__init__()

    def evaluate(self):
        #code here
        pass
"""


SOCKET_TO_FIELD = {
    "NodeSocketFloat": "avango.SFFloat",
    "Vec3Socket": "avango.gua.SFVec3",
    "MatrixSocket": "avango.gua.SFMatrix4",
    "NodeSocketBool": "avango.SFBool",
    "NodeSocketInt": "avango.SFInt",
    }


class NewSocketOperator(bpy.types.Operator):
    bl_idname = "object.new_socket_operator"
    bl_label = "New Field"
    bl_description = "add a new field to this script field container"

    types = [
        ("FLOAT", "Float", "single float value", 1),
        ("VEC3", "Vec3", "vector of 3 float values", 2),
        ("MATRIX4X4", "Matrix4x4", "4x4 Matrix", 3),
        ("BOOL", "Bool", "Boolean", 4),
        ("INT", "Int", "Integer", 5),
    ]

    directions = [
        ("INPUT",   "Input",   "", 1),
        ("OUTPUT",   "Output",   "", 2),
    ]

    new_socket_type = bpy.props.EnumProperty(
        items=types,
        name="Type",
        description="choose the type of the new field",
        default="FLOAT",
        )

    new_socket_direction = bpy.props.EnumProperty(
        items=directions,
        name="Input-Output",
        description="choose Input or Output",
        default="INPUT",
        )

    new_socket_name = bpy.props.StringProperty(
        name="Field Name",
        description="should start with an upper case letter",
        default='NewField',
        )

    def execute(self, context):
        if self.new_socket_type == "FLOAT":
            socket_type = "NodeSocketFloat"
        elif self.new_socket_type == "VEC3":
            socket_type = "Vec3Socket"
        elif self.new_socket_type == "MATRIX4X4":
            socket_type = "MatrixSocket"
        elif self.new_socket_type == "BOOL":
            socket_type = "NodeSocketBool"
        elif self.new_socket_type == "INT":
            socket_type = "NodeSocketInt"

        if self.new_socket_direction == "INPUT":
            self.node.inputs.new(socket_type, self.new_socket_name)
        else:
            self.node.outputs.new(socket_type, self.new_socket_name)
        self.node.field_connections_dirty = True

        return {'FINISHED'}

    def invoke(self, context, event):
        self.node = context.node
        return context.window_manager.invoke_props_dialog(self)


class RemoveSocketOperator(bpy.types.Operator):
    bl_idname = "object.remove_socket_operator"
    bl_label = "Remove Field"
    bl_description = "remove a new field by name from this script field container"

    remove_socket_name = bpy.props.StringProperty(
        name="Field Name",
        description="enter name of field to remove",
        default='NewField',
        )

    def execute(self, context):
        for field in self.node.inputs:
            if field.name == self.remove_socket_name:
                self.node.inputs.remove(field)
        for field in self.node.outputs:
            if field.name == self.remove_socket_name:
                self.node.outputs.remove(field)
        
        return {'FINISHED'}

    def invoke(self, context, event):
        self.node = context.node
        return context.window_manager.invoke_props_dialog(self)


class EditSourceOperator(bpy.types.Operator):
    bl_idname = "object.edit_source_operator"
    bl_label = "Edit Source"
    bl_description =\
        "opens source code, make sure to have a text editor view open"

    def execute(self, context):
        context.node.update_script_source()

        for area in bpy.context.screen.areas:
            if area.type == 'TEXT_EDITOR':
                area.spaces.active.text = bpy.data.texts[context.node.name]

        return {'FINISHED'}


def update_name(self, context):
    self.update_script_source()
    old_name = self.name
    self.name = self.name_prop
    text = bpy.data.texts[old_name]
    text.name = self.name
    content = text.as_string()
    text.from_string(content.replace(old_name, self.name))


def update_field_connections(self, contex):
    if self.field_connections_dirty is True:
        old_src = bpy.data.texts[self.name].as_string()
        new_src = self.create_script_source()
        new_src_parts = new_src.partition("__init__()\n")
        combined_src = new_src_parts[0] + new_src_parts[1]
        combined_src += old_src.partition("__init__()\n")[2]
        bpy.data.texts[self.name].from_string(combined_src)


class ScriptNode(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = "Script"
    bl_label = "Script"

    name_prop = StringProperty(
        name="Name",
        default='MyScript',
        update=update_name
        )

    field_connections_dirty = BoolProperty(
        default=False,
        update=update_field_connections
        )

    def init(self, context):
        self.name = "MyScript"
        self.update_script_source()

    def draw_buttons(self, context, layout):
        col = layout.column()
        col.prop(self, 'name_prop')
        col.operator('object.new_socket_operator')
        col.operator('object.remove_socket_operator')
        col.operator('object.edit_source_operator')

    def to_dict(self):
        return {
            'type': 'Script',
            'name': self.name,
            'module': self.name.lower(),
            'values': export_utils.export_values(self),
            'field_connections': export_utils.export_links(self),
        }

    def update_script_source(self):
        if not self.name in bpy.data.texts.keys():
            txt = bpy.data.texts.new(self.name)
            txt.from_string(self.create_script_source())

    def create_script_source(self):
        field_declarations = "  # Inputs:\n"
        for socket in self.inputs:
            field_declarations += "    " + socket.name + " = " \
                + SOCKET_TO_FIELD[socket.bl_idname] + "()\n"

        field_declarations += "  # Outputs:\n"
        for socket in self.outputs:
            field_declarations += "    " + socket.name + " = " \
                + SOCKET_TO_FIELD[socket.bl_idname] + "()\n"

        new_src = copy.deepcopy(SCRIPT_TEMPLATE)
        new_src = new_src.replace("%CLASSNAME%", self.name_prop)
        new_src = new_src.replace("%FIELD_DECLARATIONS%", field_declarations)

        self.field_connections_dirty = False
        return new_src

    def execute(self, x):
        pass

    def update(self):
        pass

    def free(self):
        bpy.data.texts.remove(bpy.data.texts[self.name])


def register():
    bpy.utils.register_class(ScriptNode)
    bpy.utils.register_class(NewSocketOperator)
    bpy.utils.register_class(RemoveSocketOperator)
    bpy.utils.register_class(EditSourceOperator)


def unregister():
    bpy.utils.unregister_class(ScriptNode)
    bpy.utils.unregister_class(NewSocketOperator)
    bpy.utils.unregister_class(RemoveSocketOperator)
    bpy.utils.unregister_class(EditSourceOperator)
