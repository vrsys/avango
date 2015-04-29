import bpy
import bpy.types
# from bpy_extras.io_utils import create_derived_objects, free_derived_objects
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty
import bpy.path
import os
# import shutil
import json
from . import nodes
from mathutils import Matrix


def defaultWindow(scene):
    return {
        'type': 'Window',
        'name': 'Window',
        'title': 'avango-blender',
        'display': ':0.0',
        'left_resolution': [scene.resolution_x, scene.resolution_y],
        'left_position': [0, 0],
        'mode': 'MONO'
    }


# TODO:
# write parent
def matrixToList(matrix):
    return sum(list(list(x) for x in matrix), [])


def export_fieldconnections(obj):
    field_connections = []
    for key in obj.outputs.keys():
        for link in obj.outputs[key].links:
            if link.to_node.bl_idname == 'FromObject':
                ref_obj = bpy.data.objects[link.to_node.referenced_object]
            else:
                ref_obj = link.to_node
                field_connections.append({
                    "from_field": key,
                    "to_node": ref_obj.name,
                    "to_field": link.to_socket.name,
                    })
    return field_connections


def avangoNodeTrees():
    return (x for x in bpy.data.node_groups
            if (x.bl_idname == 'AvangoCustomTreeType'))

    # write out field_connections:
    #   for l in bpy.data.node_groups["NodeTree"].links:
    #      if l.is_field_connection and l.is_valid:
    #        { "from_node"  : l.from_node.name,
    #          "to_node"    : l.to_node.name,
    #          "from_field" : l.from_socket.name,
    #          "to_field"   : l.to_socket.name
    #        }


def to_json(obj):
    # if isinstance(obj, field_container.SceneGraph):
    #     name = obj.name
    #     root = 'null'
    #     i = bpy.data.objects.find(obj.root)
    #     if -1 != i :
    #         root = bpy.data.objects[obj.root].name
    #     return {
    #             'type' : 'SceneGraph',
    #             'name' : obj.name,
    #             'root' : root
    #     }
    if obj.type == 'EMPTY':
        parent = 'null'
        if obj.parent:
            parent = obj.parent.name
        matrix = obj.matrix_local

        return {
            'name': obj.name,
            'parent': parent,
            'transform': matrixToList(matrix)
        }

    if obj.type == 'CAMERA':
        parent = 'null'
        if obj.parent:
            parent = obj.parent.name
        matrix = obj.matrix_local

        acam = obj.data.avango

        passes = acam.pipeline_passes
        json_passes = []

        for p in passes:
            json_passes.append(p.to_dict())

        return {
            'type': 'Camera',
            'name': obj.name,
            'transform': matrixToList(matrix),
            'parent': parent,
            # 'scenegraph': obj.scenegraph,
            # 'output_window_name': obj.output_window_name,
            # 'left_screen_path': obj.left_screen_path,
            'resolution': [acam.resolution[0], acam.resolution[1]],
            'field_of_view': obj.data.angle,
            'enable_abuffer': acam.enable_abuffer,
            'passes': json_passes,
        }

    if isinstance(obj, bpy.types.Material):
        color = obj.diffuse_color

        return {
            'type': 'Material',
            'name': obj.name,
            'color': [color.r, color.g, color.b],
            'roughness': obj.avango.roughness,
            'metalness': obj.avango.metalness,
            'emissivity': obj.avango.emissivity,
            'color_map': bpy.path.abspath(obj.avango.color_texture),
            'metalness_map': bpy.path.abspath(obj.avango.metalness_texture),
            'roughness_map': bpy.path.abspath(obj.avango.roughness_texture),
            'emissivity_map': bpy.path.abspath(obj.avango.emissivity_texture),
            'normal_map': bpy.path.abspath(obj.avango.normal_texture),
            'opacity': obj.avango.opacity,
            'backface_culling': obj.avango.backface_culling,
        }

    if isinstance(obj, nodes.light.Light):
        i = bpy.data.objects.find(obj.referenced_object)
        lamp = None
        o = None
        if -1 != i:
            o = bpy.data.objects[obj.referenced_object]
            lamp = bpy.data.lamps[o.data.name]
        # ty = 'null'
        # if lamp.type == 'POINT':
            # ty = 'PointLight'

        parent = 'null'
        if obj.referenced_object in bpy.data.objects:
            if bpy.data.objects[obj.referenced_object].parent:
                parent = bpy.data.objects[obj.referenced_object].parent.name
            matrix = bpy.data.objects[obj.referenced_object].matrix_local

        return {
            'name': obj.name,
            'type': lamp.type,
            'color': [lamp.color.r, lamp.color.g, lamp.color.b],
            'parent': parent,
            'transform': matrixToList(matrix),
            'brightness': lamp.energy
        }

    if obj.type == 'MESH':
        if obj.find_armature() is None:
            parent = 'null'
            blender_obj = obj
            if obj.name in bpy.data.objects:
                blender_obj = bpy.data.objects[obj.name]
                if blender_obj.parent:
                    parent = blender_obj.parent.name
                matrix = blender_obj.matrix_local
            filename = obj.name + '.obj'

            bpy.ops.object.select_all(action='DESELECT')
            # scene.objects.active = blender_obj
            blender_obj.select = True

            world = blender_obj.matrix_world.copy()
            if not blender_obj.animation_data is None:
                for f in blender_obj.animation_data.action.fcurves:
                    f.mute = True

            Matrix.identity(blender_obj.matrix_world)
            bpy.ops.export_scene.obj(
                filepath=g_tmp_filepath+filename,
                check_existing=False,
                use_selection=True,
                use_normals=True,
                use_triangles=True,
                use_uvs=True,
                use_materials=False,
                axis_forward='Y',
                axis_up='Z',
                path_mode='AUTO',
                )
            blender_obj.matrix_world = world

            if not blender_obj.animation_data is None:
                for f in blender_obj.animation_data.action.fcurves:
                    f.mute = False

            blender_obj.select = False

            if len(blender_obj.material_slots) > 0:
                material = blender_obj.material_slots[0].material.name
            else:
                material = "default_material"

            return {
                'type': 'Mesh',
                'name': obj.name,
                'file': 'tmp/' + filename,
                'parent': parent,
                'transform': matrixToList(matrix),
                'material': material,
                'has_armature': False,
            }
        else:
            arma = obj.find_armature()
            parent = 'null'
            # blender_obj = obj
            # if obj.name in bpy.data.objects:
                # blender_obj = bpy.data.objects[obj.name]
            if arma.parent:
                parent = arma.parent.name
            filename = obj.name + '.fbx'

            bpy.ops.object.select_all(action='DESELECT')
            obj.select = True
            arma.select = True

            world = arma.matrix_world.copy()
            Matrix.identity(arma.matrix_world)
            bpy.ops.export_scene.fbx(
                filepath=g_tmp_filepath+filename,
                check_existing=False,
                use_selection=True,
                version='BIN7400',
                use_mesh_modifiers=False,
                add_leaf_bones=False,
                bake_anim=False,
                use_anim=False,
                use_anim_action_all=False,
                axis_forward='Y',
                axis_up='Z',
                path_mode='AUTO',
                embed_textures=True,
                )
            arma.matrix_world = world
            obj.select = False
            arma.select = False

            materials = []

            if len(obj.material_slots) > 0:
                for mat in obj.material_slots:
                    materials.append(mat.material.name)
            else:
                materials = ["default_material"]

            return {
                'type': 'Mesh',
                'name': obj.name,
                'file': 'tmp/' + filename,
                'parent': parent,
                'transform': matrixToList(arma.matrix_local),
                'material': materials,
                'has_armature': True,
            }

    # if isinstance(obj, nodes.window.Window):
    #     modeSocket = bpy.data.node_groups["NodeTree"] \
    #                 .nodes["Window"] \
    #                 .inputs['StereoMode']
    #     x = bpy.context.scene.render.resolution_x
    #     y = bpy.context.scene.render.resolution_y

    #     if modeSocket.is_linked and modeSocket.links[0].is_valid:
    #       mode = modeSocket.links[0].from_socket.stereo_mode
    #     else:
    #       mode = modeSocket.stereo_mode
    #     return {
    #             'type' : 'Window',
    #             'name' : obj.name,
    #             'title' : obj.title_field,
    #             'display' : obj.display_field,
    #             'left_resolution' : [obj.left_size[0], obj.left_size[1]],
    #             'left_position' : [obj.left_pos[0], obj.left_pos[1]],
    #             'mode' : mode
    #     }

    # if isinstance(obj, nodes.screen.Screen):
    #     parent = 'null'
    #     matrix = []
    #     if obj.referenced_object in bpy.data.objects:
    #       if bpy.data.objects[obj.referenced_object].parent:
    #         parent = bpy.data.objects[obj.referenced_object].parent.name
    #       matrix = bpy.data.objects[obj.referenced_object].matrix_local

    #     return {
    #             'type' : 'Screen',
    #             'name' : obj.name,
    #             'parent' : parent,
    #             'transform' : matrixToList(matrix)
    #     }
    # if isinstance(obj, field_container.Transform):
    #     parent = 'null'
    #     if obj.referenced_object in bpy.data.objects:
    #       if bpy.data.objects[obj.referenced_object].parent:
    #         parent = bpy.data.objects[obj.referenced_object].parent.name
    #       matrix = bpy.data.objects[obj.referenced_object].matrix_local
    #     return {
    #             'type' : 'Transform',
    #             'parent' : parent,
    #             'transform' : matrixToList(matrix),
    #             'name' : obj.name
    #     }

    if isinstance(obj, nodes.time_sensor.TimeSensor):
        return {
            'type': 'TimeSensor',
            'name': obj.name,
            'field_connections': export_fieldconnections(obj),
        }

    # if isinstance(obj, nodes.rotation_matrix.RotationMatrix):
    #   return obj.to_dict()

    # if isinstance(obj, nodes.vec3.Vec3):
    #   return obj.to_dict()

    # if isinstance(obj, nodes.script_node.ScriptNode):
    #   return obj.to_dict()
    try:
        return obj.to_dict()
    except TypeError:
        print(repr(obj) + ' is not JSON serializable')


    # raise TypeError(repr(obj) + ' is not JSON serializable')


def save(filepath, context):
    which = "NodeTree"
    ns = bpy.data.node_groups[which].nodes

    context.scene.frame_set(context.scene.frame_start)

    meshes = []
    materials = {}
    transforms = []
    for obj in bpy.data.objects:
        if obj.type == 'MESH':
            meshes.append(obj)
            if len(obj.material_slots) > 0:
                for mat_slot in obj.material_slots:
                    mat = mat_slot.material
                    materials[mat.name] = mat
        elif obj.type == 'EMPTY':
            transforms.append(obj)
        elif obj.type == 'CAMERA':
            camera = obj

    document = {
        # triMeshes  = (x for x in ns if (x.bl_label == 'Mesh'))
        # screens    = (x for x in ns if (x.bl_label == 'Screen'))
        # 'transforms' : list(x for x in ns if (x.bl_label == 'Transform'))
        # 'scenegraphs' : dict((x.name,x) for x in ns
        #                     if (x.bl_label == 'SceneGraph')),
        'camera': camera,
        'meshes': dict((m.name, m) for m in meshes),
        'materials': materials,
        'transforms': dict((t.name, t) for t in transforms),
        'windows': dict((x.name, x) for x in ns if (x.bl_label == 'Window')),
        'lights': dict((x.name, x) for x in ns if (x.bl_label == 'Light')),
        'screens': dict((x.name, x) for x in ns if (x.bl_label == 'Screen')),
        'time_sensors': dict((x.name, x) for x in ns
                             if (x.bl_label == 'TimeSensor')),
        'keyboards': dict((x.name, x) for x in ns
                          if (x.bl_label == 'Keyboard')),
        'rotation_matrices': dict((x.name, x) for x in ns
                                  if (x.bl_label == 'Rotation Matrix')),
        'translation_matrices': dict((x.name, x) for x in ns
                                     if (x.bl_label == 'Translation Matrix')),
        'vec3s': dict((x.name, x) for x in ns if (x.bl_label == 'Vec3')),
        'floats': dict((x.name, x) for x in ns if (x.bl_label == 'Float')),
        'floatmaths': dict((x.name, x) for x in ns
                           if (x.bl_label == 'Float Math')),
        'from_objects': dict((x.name, x) for x in ns
                             if (x.bl_label == 'Object as FieldContainer')),
        'scripts': dict((x.name, x) for x in ns if (x.bl_label == 'Script')),
    }

    global g_tmp_filepath
    g_tmp_filepath = filepath.rpartition('/')[0]
    g_tmp_filepath += '/tmp/'

    if not os.path.exists(g_tmp_filepath):
        os.makedirs(g_tmp_filepath)

    # export scripts
    for node in ns:
        if node.bl_idname == 'Script':
            with open(g_tmp_filepath + node.name.lower() +
                      ".py", 'w', encoding='utf-8') as f:
                f.write(bpy.data.texts[node.name].as_string())
            print("exported " + node.name)

    with open(filepath, 'w', encoding='utf-8') as f:
        json.dump(document, f, default=to_json, indent=4)
    return {'FINISHED'}


class ExportAvango(bpy.types.Operator, ExportHelper):
    '''Export selected object / scene for Avango (ASCII JSON format).'''

    bl_idname = "export.avango"
    bl_label = "Export Avango"

    filename_ext = ".avango"
    filter_glob = StringProperty(default="*.avango")

    def invoke(self, context, event):
        #restore_settings_export(self.properties)
        return ExportHelper.invoke(self, context, event)

    @classmethod
    def poll(cls, context):
        return context.active_object is not None

    def execute(self, context):
        print("Selected: " + context.active_object.name)
        if not self.properties.filepath:
            raise Exception("filename not set")

        return save(self.filepath, context)  # , **self.properties)

    def draw(self, context):
        layout = self.layout

        row = layout.row()
        row.label(text="Geometry:")


def menu_func_export(self, context):
    default_path = bpy.data.filepath.replace(".blend", ".json")
    self.layout.operator(
        ExportAvango.bl_idname, text="Avango (.avango)"
        ).filepath = default_path


def register():
    bpy.utils.register_class(ExportAvango)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportAvango)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)
