import bpy
from bpy.types import NodeTree, Node, NodeSocket
from bpy.props import StringProperty, BoolProperty
import bpy.path

from .. import node_tree
from .. import sockets
from .. import export_utils


class FromObject(Node, node_tree.AvangoCustomTreeNode):
    bl_idname = 'FromObject'
    bl_label = 'Object as FieldContainer'

    def draw_socket(self, which, context, layout, text):
        obj = bpy.data.objects[self.referenced_object]
        adata = obj.data.avango
        layout.prop(adata, which, text=text)

    def update_referenced_object_camera(self, context):
        obj = bpy.data.objects[self.referenced_object]
        s = self.inputs.new("MatrixSocket", "Transform")
#        s.default_value = obj.data.avango.EyeDistance
#        s.draw = drawtest

    def update_referenced_object_mesh(self, context):
        obj = bpy.data.objects[self.referenced_object]
        s = self.inputs.new("MatrixSocket", "Transform")

    def update_referenced_object_empty(self, context):
        obj = bpy.data.objects[self.referenced_object]
        s = self.inputs.new("MatrixSocket", "Transform")

    def update_referenced_object_point_light(self, context):
        obj = bpy.data.objects[self.referenced_object]
        s = self.inputs.new("MatrixSocket", "Transform")

    def update_referenced_object(self, context):
        print("update_referenced_object")
        if self.referenced_object in bpy.data.objects:
            obj = bpy.data.objects[self.referenced_object]
            print(obj.type)
            self.label = obj.name
            if obj.type == 'CAMERA':
                self.update_referenced_object_camera(context)
            elif obj.type == 'MESH':
                self.update_referenced_object_mesh(context)
            elif obj.type == 'EMPTY':
                self.update_referenced_object_empty(context)
            elif obj.type == 'LAMP':
                self.update_referenced_object_point_light(context)

          #EyeDistance.value_property = "EyeOffset"

    referenced_object = StringProperty(
        description='linked mesh',
        #update= todo when update , add my name to blender object
        update=update_referenced_object
        )

    export_action = BoolProperty(default=False)
    loop_action = BoolProperty(default=False)

    animation_path = StringProperty(
        name="Animation Path",
        subtype='FILE_PATH',
    )

    def init(self, context):
        self.referenced_object = ""

    def update(self):
        print("Update Node FromObject " + self.referenced_object)

    # draw_buttons is not allowed to change self
    def draw_buttons(self, context, layout):  # const
        scene = context.scene
        col = layout.column()
        if self.referenced_object in bpy.data.objects:
            obj = bpy.data.objects[self.referenced_object]
            oty = obj.type
            aobj = obj.avango

            if oty == 'CAMERA':
                col.label(text='Name: '+obj.name, icon='CAMERA_DATA')

            elif oty == 'MESH':
                col.label(text='Name: '+obj.name, icon='MESH_DATA')
                if not obj.find_armature() is None:
                    col.prop(self, "animation_path", text="Animation")

            elif oty == 'EMPTY':
                col.label(text='Name: '+obj.name, icon='EMPTY_DATA')

            elif oty == 'LAMP':
                col.label(text='Name: '+obj.name, icon='LAMP_POINT')

            col.prop(self, "export_action", text="Export Action")
            if self.export_action is True:
                col.prop(self, "loop_action", text="Loop Action")

        else:
            col.prop_search(self, 'referenced_object', bpy.data, 'objects',
                            text='', icon='OBJECT_DATA')

#    # a blender mesh is telling us, that it will no longer link to this mesh
#    def unregister(self, blender_mesh):
#      if blender_mesh.name == self.referenced_object:
#          self.referenced_object = ""
#      # else:
#      #   ignore event

    def process(self):
        pass

    def get_args(self):
        pass

    def to_dict(self):
        ref_obj = bpy.data.objects[self.referenced_object]

        new_dict = {
            'type': ref_obj.type,
            'referenced_name': ref_obj.name,
            'import_action': False,
            'field_connections': export_utils.export_links(self)
        }

        if self.export_action and (not ref_obj.animation_data is None):
            print("exporting action for " + self.referenced_object + " ... ",
                  end="", flush=True)
            ani_range = ref_obj.animation_data.action.frame_range
            scene = bpy.context.scene
            keyframe_list = []
            for i in range(int(ani_range[0]), int(ani_range[1])+1):
                scene.frame_set(i)
                keyframe_list.append({
                    'time': frame_to_time(i),
                    'transform': matrixToList(ref_obj.matrix_local)
                })

            new_dict['keyframes'] = keyframe_list
            new_dict['import_action'] = True
            new_dict['loop_action'] = self.loop_action
            print("done")
        bpy.context.scene.frame_set(0)

        if not ref_obj.find_armature() is None:
            new_dict['animation_path'] = bpy.path.abspath(self.animation_path)
        else:
            new_dict['animation_path'] = ""

        return new_dict

    def free(self):
        print("Object unregister shit")
        i = bpy.data.objects.find(self.referenced_object)
        if -1 != i:
            print("Remove link to me")

            obj = bpy.data.objects[self.referenced_object]
            if obj.get("avango_nodes"):
                obj["avango_nodes"] = list(
                    filter((obj["avango_nodes"]).__ne__, self.name)
                    )


def register():
    bpy.utils.register_class(FromObject)


def unregister():
    bpy.utils.unregister_class(FromObject)


def frame_to_time(frame_number):
    scene = bpy.context.scene
    fps = scene.render.fps
    raw_time = (frame_number) / fps
    return round(raw_time, 3)


def matrixToList(matrix):
    return sum(list(list(x) for x in matrix), [])
