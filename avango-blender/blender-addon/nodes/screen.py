import bpy
from bpy.types import Node
from bpy.props import StringProperty, FloatProperty

from .. import node_tree


class Screen(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = 'Screen'
    bl_label = 'Screen'

    def update_name(self, context):
        if self.referenced_object in bpy.data.objects:
            bpy.data.objects[self.referenced_object].name = self.name
            self.referenced_object = self.name
        else:
            print("Error: failed referenced_object")

    width = FloatProperty(default=2, step=0.001, min=0.01)

    height = FloatProperty(default=1.5, step=0.001, min=0.01)

    referenced_object = StringProperty(
        default='',
        description='name of referenced blender object'
        )

    name = StringProperty(description='name', update=update_name)

    def init(self, context):
        bpy.ops.object.empty_add(type='CUBE')
        obj = bpy.context.object
        #obj.scale = (2*0.5, 0.0, 1.5 * 0.5)
        #obj.location = (0.0, 0.0, 0.75)
        self.referenced_object = obj.name
        self.name = obj.name
        obj["avango_nodes"] = self.name
        self.outputs.new('ScreenSocketType', 'Screen')

    def draw_buttons(self, context, layout):
        col = layout.column()
        col.prop(self, 'name', text='Name')
        col.prop(self, 'width', text='Width')
        col.prop(self, 'height', text='Heigth')

    def get_path(self):
        path = "/"
        return path + self.name

    def process(self):
        pass

    def get_args(self):
        pass


def register():
    bpy.utils.register_class(Screen)


def unregister():
    bpy.utils.unregister_class(Screen)
