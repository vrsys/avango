import bpy
from bpy.types import Node
from bpy.props import StringProperty

from .. import node_tree


class Light(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = 'Light'
    bl_label = 'Light'

    def update_name(self, context):
        if self.referenced_object in bpy.data.objects:
            bpy.data.objects[self.referenced_object].name = self.name
            self.referenced_object = self.name
        else:
            print("Error: failed referenced_object")

    name = StringProperty(description='name', update=update_name)

    referenced_object = StringProperty(
        default='',
        description='name of referenced blender object'
        #update=update_node
        )
        #update= todo when update , add my name to blender object

    def init(self, context):
        bpy.ops.object.lamp_add(type='POINT')
        obj = bpy.context.object
        self.referenced_object = obj.name
        self.name = obj.name
        obj["avango_nodes"] = self.name

    def draw_buttons(self, context, layout):
        col = layout.column()
        col.prop(self, 'name', text='Name')
        #if referenced_object == '':
        #    col.prop(self, ...oplus, button new light
        # browse lights
        #col.prop_search(self, 'referenced_object', bpy.data, 'lamps',
        #        text='', icon='LAMP_DATA')
        col.label(text='Light: '+self.referenced_object, icon='LAMP_DATA')

    def free(self):
        print("Light unregister shit")
        i = bpy.data.objects.find(self.referenced_object)
        if -1 != i:
            print("Remove link to me")

            obj = bpy.data.objects[self.referenced_object]
            if obj.get("avango_nodes"):
                obj["avango_nodes"] = list(
                    filter((obj["avango_nodes"]).__ne__, self.name)
                    )

    def process(self):
        pass

    def get_args(self):
        pass


def register():
    bpy.utils.register_class(Light)


def unregister():
    bpy.utils.unregister_class(Light)
