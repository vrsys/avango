import bpy
import bpy.types
# from bpy_extras.io_utils import create_derived_objects, free_derived_objects
from bpy_extras.io_utils import (
        ExportHelper,
        orientation_helper_factory,
        path_reference_mode,
        axis_conversion,
        )
from bpy.props import StringProperty
import bpy.path
import os
# import shutil
import json
from . import nodes
from mathutils import Matrix, Vector
import math
import mathutils
from mathutils import (Vector, Matrix, Quaternion)


def change_coordinate_system(P, A):
    return P * A * P.inverted()


def to_avango_node_id(name):
    return "node_" + name


"""
def to_avango_camera(bl_camera):
    if bl_camera.type == 'PERSP':
        return {
            "name": bl_camera.name,
            "perspective": {
                "aspectRatio":
                bl_camera.sensor_width / bl_camera.sensor_height,
                "yfov": bl_camera.angle_y,
                "znear": bl_camera.clip_start,
                "zfar": bl_camera.clip_end
            },
            "type": "perspective"
        }
    elif bl_camera.type == 'ORTHO':
        return {
            "name": bl_camera.name,
            "orthographic": {
                "xmag": bl_camera.ortho_scale,
                "ymag": bl_camera.ortho_scale,
                "znear": bl_camera.clip_start,
                "zfar": bl_camera.clip_end,
            },
            "type": "orthographic"
        }
    else:
        return {}
"""


def texture_filepath(texname):
    if texname.image.packed_file is not None:
        print("packed files not supported")
    return bpy.data.textures[texname].image.filepath


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


def to_avango_camera(obj, global_matrix):
    local_matrix = obj.matrix_local
    new_local = change_coordinate_system(global_matrix, local_matrix)
    #new_local = Matrix.Identity(4)
    mat_rot = mathutils.Matrix.Rotation(math.radians(-90.0), 4, 'X')
    new_local = new_local * mat_rot

    parent = 'null'
    if obj.parent:
        parent = obj.parent.name

    #rot_mode = obj.rotation_mode
    #if rot_mode != 'QUATERNION':
    #    obj.rotation_mode = 'QUATERNION'

    #q = obj.rotation_quaternion * Quaternion((1.0, 0.0, 0.0), math.radians(-90.0))


    #obj.rotation_mode = rot_mode
    #obj.matrix_local = local_matrix

    acam = obj.data.avango

    passes = acam.pipeline_passes
    json_passes = []

    for p in passes:
        json_passes.append(p.to_dict())

    cam = {
        'type': 'Camera',
        'name': obj.name,
        'transform': matrixToList(new_local),
        'parent': parent,
        'near_clip': obj.data.clip_start,
        'far_clip': obj.data.clip_end,
        'mode': 0 if obj.data.type == 'PERSP' else 1,
        # 'scenegraph': obj.scenegraph,
        # 'output_window_name': obj.output_window_name,
        # 'left_screen_path': obj.left_screen_path,
        'resolution': [acam.resolution[0], acam.resolution[1]],
        'field_of_view': obj.data.angle,
        'enable_frustum_culling': acam.enable_frustum_culling,
        'enable_abuffer': acam.enable_abuffer,
        'enable_stereo': acam.enable_stereo,
        'passes': json_passes,
    }
    return cam


def to_avango_light(obj, global_matrix):
    local_matrix = obj.matrix_local
    new_local = change_coordinate_system(global_matrix, local_matrix)

    lamp = obj.data

    parent = 'null'
    if obj.parent:
        parent = obj.parent.name

    enable_shadows = True
    if lamp.shadow_method == 'NOSHADOW':
        enable_shadows = False

    return {
        'name': obj.name,
        'parent': parent,
        'transform': matrixToList(new_local),
        'type': lamp.type,
        'color': [lamp.color.r, lamp.color.g, lamp.color.b],
        'brightness': lamp.energy,
        'enable_shadows': enable_shadows,
    }

def to_avango_transform(obj, global_matrix):
    parent = 'null'
    if obj.parent:
        parent = obj.parent.name

    local_matrix = obj.matrix_local
    new_local = change_coordinate_system(global_matrix, local_matrix)

    #obj.matrix_local = new_local
    #rot_mode = obj.rotation_mode

    #matrix = obj.matrix_local
    #obj.rotation_mode = rot_mode
    #obj.matrix_local = local_matrix

    return {
        'name': obj.name,
        'parent': parent,
        'transform': matrixToList(new_local)
    }


def to_json(obj):
    if isinstance(obj, bpy.types.Material):
        color = obj.diffuse_color

        return {
            'type': 'Material',
            'name': obj.name,
            'color': [color.r, color.g, color.b],
            'roughness': obj.avango.roughness,
            'metalness': float(obj.avango.metalness),
            'emissivity': obj.avango.emissivity,
            'color_map': bpy.path.abspath(
                texture_filepath(obj.avango.color_texture))
            if obj.avango.use_color_texture else "",
            'metalness_map': bpy.path.abspath(
                texture_filepath(obj.avango.metalness_map))
            if obj.avango.use_metalness_texture else "",
            'roughness_map': bpy.path.abspath(
                texture_filepath(obj.avango.roughness_map))
            if obj.avango.use_roughness_texture else "",
            'emissivity_map': bpy.path.abspath(
                texture_filepath(obj.avango.emissivity_map))
            if obj.avango.use_emissivity_texture else "",
            'normal_map': "" if obj.avango.normal_texture == "" else
            bpy.path.abspath(texture_filepath(obj.avango.normal_texture)),
            'opacity': obj.avango.opacity,
            'backface_culling': obj.avango.backface_culling,
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

    try:
        return obj.to_dict()
    except (TypeError, AttributeError):
        print(repr(obj) + ' is not JSON serializable')


def to_avango_mesh(obj, global_matrix):
    local_matrix = obj.matrix_local
    new_local = change_coordinate_system(global_matrix, local_matrix)
    if obj.type == 'MESH':
        if obj.find_armature() is None:
            parent = 'null'
            blender_obj = obj
            if obj.name in bpy.data.objects:
                blender_obj = bpy.data.objects[obj.name]
                if blender_obj.parent:
                    parent = blender_obj.parent.name
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
                filepath=g_tmp_filepath + '/' + filename,
                check_existing=False,
                use_selection=True,
                use_normals=True,
                use_triangles=True,
                use_uvs=True,
                use_materials=False,
                axis_forward='-Z',
                axis_up='Y',
                path_mode='AUTO', )
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
                'file': filename,
                'parent': parent,
                'transform': matrixToList(new_local),
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
                filepath=g_tmp_filepath + '/' + filename,
                check_existing=False,
                use_selection=True,
                version='BIN7400',
                use_mesh_modifiers=False,
                add_leaf_bones=False,
                bake_anim=False,
                use_anim=False,
                use_anim_action_all=False,
                axis_forward='-Z',
                axis_up='Y',
                path_mode='AUTO',
                embed_textures=True, )
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
                'file': filename,
                'parent': parent,
                'transform': matrixToList(arma.matrix_local),
                'material': materials,
                'has_armature': True,
            }

    if obj.type == 'FONT':
        parent = 'null'
        blender_obj = obj
        if obj.name in bpy.data.objects:
            blender_obj = bpy.data.objects[obj.name]
            if blender_obj.parent:
                parent = blender_obj.parent.name
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
            filepath=g_tmp_filepath + '/' + filename,
            check_existing=False,
            use_selection=True,
            use_normals=True,
            use_triangles=True,
            use_uvs=True,
            use_materials=False,
            axis_forward='-Z',
            axis_up='Y',
            path_mode='AUTO', )
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
            'file': filename,
            'parent': parent,
            'transform': matrixToList(new_local),
            'material': material,
            'has_armature': False,
        }


def to_avango_node(obj, global_matrix):
    local_matrix = obj.matrix_local
    new_local = change_coordinate_system(global_matrix, local_matrix)

    obj.matrix_local = new_local

    rot_mode = obj.rotation_mode
    if rot_mode != 'QUATERNION':
        obj.rotation_mode = 'QUATERNION'
    node = {
        "name": obj.name,
        # if not armature
        "children": [] if obj.type == 'ARMATURE' else [
           to_avango_node_id(child.name)
            for child in obj.children
        ],
        "rotation": [
            obj.rotation_quaternion.x, obj.rotation_quaternion.y,
            obj.rotation_quaternion.z, obj.rotation_quaternion.w
        ],
        "scale": [
            obj.scale[0], obj.scale[1], obj.scale[2]
        ],
        "translation": [
            obj.location[0], obj.location[1], obj.location[2]
        ]
    }
    obj.rotation_mode = rot_mode
    obj.matrix_local = local_matrix

    joints = []

    if obj.type == 'CAMERA':
        node["camera"] = "camera_" + obj.data.name

        q = obj.rotation_quaternion * Quaternion((1.0, 0.0, 0.0), math.radians(-90.0))
        node["rotation"] = [q.x, q.y, q.z, q.w]
        node["type"] = 'CameraNode'

    # Skin
    elif obj.type == 'MESH' and obj.find_armature():
        node["meshes"] = ["mesh_" + obj.data.name]
        armature = obj.find_armature()
        if armature:  # skinned mesh
            node["skeletons"] = ["joint_" + armature.data.name +
                                 "_" + bone.name
                                 for bone in armature.data.bones
                                 if bone.parent is None]
            node["skin"] = "skin_" + obj.data.name
        node["type"] = 'SkeletalAnimationNode'

    elif obj.type == 'MESH':
        node["meshes"] = ["mesh_" + obj.data.name]
        node["type"] = 'TriMeshNode'

    elif obj.type == 'LAMP':
        node["type"] = 'LightNode'

    elif obj.type == 'EMPTY':
        node["type"] = 'TransformNode'

    elif obj.type == 'ARMATURE':
        armature = obj.data
        prefix = "joint_" + armature.name + "_"

        def to_joint(armature, bone):
            matrix = bone.parent.matrix_local.inverted() * bone.matrix_local if bone.parent else bone.matrix_local
            return {
                "name": bone.name,
                "jointName": prefix + bone.name,
                "children": [prefix + child.name for child in bone.children],
                "matrix": matrix_to_list(
                    change_coordinate_system(global_matrix, matrix))
                }
        node["children"] = [prefix+bone.name
                            for bone in armature.bones
                            if bone.parent is None] + [
                            to_avango_node_id(child.name)
                            for child in obj.children
                            ]
        joints = list((prefix+bone.name, to_joint(armature, bone))
                      for bone in armature.bones)
    return [(to_avango_node_id(obj.name), node)] + joints


def save(operator, context,
        filepath="",
        global_matrix=Matrix(),
        **kwargs
        ):


    base_name, ext = os.path.splitext(filepath)
    base_dir = os.path.dirname(filepath)
    context_name = [base_name, '', '',
                    ext]  # Base name, scene name, frame number, extension

    global g_tmp_filepath
    g_tmp_filepath = base_dir

    def nodes():
        nns = (to_avango_node(obj, global_matrix)
               for obj in context.scene.objects)
        return dict(inner for outer in nns for inner in outer)

    def scenes():
        export_scenes = [context.scene]
        return dict((scene.name, {
                "nodes": list(to_avango_node_id(n.name)
                              for n in scene.objects.values()
                              if not n.parent)
                }) for scene in export_scenes)

    which = "NodeTree"

    ns = []
    if which in bpy.data.node_groups:
      ns = bpy.data.node_groups[which].nodes

    context.scene.frame_set(context.scene.frame_start)

    materials = {}
    transforms = []
    lamps = []

    mes = []
    for obj in bpy.data.objects:
        if obj.type == 'MESH':
            mes.append(obj)
            if len(obj.material_slots) > 0:
                for mat_slot in obj.material_slots:
                    mat = mat_slot.material
                    materials[mat.name] = mat
        elif obj.type == 'EMPTY':
            transforms.append(obj)
        elif obj.type == 'CAMERA':
            camera = obj
        elif obj.type == 'LAMP':
            lamps.append(obj)
        elif obj.type == 'FONT':
            mes.append(obj)
            if len(obj.material_slots) > 0:
                for mat_slot in obj.material_slots:
                    mat = mat_slot.material
                    materials[mat.name] = mat

    #def cameras(context):
    #    return dict(("camera_" + obj.data.name, to_avango_camera(obj.data))
    #                for obj in context.scene.objects if obj.type == 'CAMERA')

    def scripts():
        return dict((x.name, x) for x in ns if (x.bl_label == 'Script'))

    def meshes():
        return dict((obj.name, to_avango_mesh(obj, global_matrix)) for obj in mes)

    def lights():
        return dict((obj.name, to_avango_light(obj, global_matrix)) for obj in lamps)

    def fieldcontainers():
        pass

    def fieldconnections():
        pass

    if not os.path.exists(g_tmp_filepath):
        os.makedirs(g_tmp_filepath)

    document = {
        'camera': to_avango_camera(camera, global_matrix),
        'meshes': meshes(),
        'lights': lights(),
        'materials': materials,

        'transforms': dict((obj.name, to_avango_transform(obj, global_matrix))
            for obj in transforms),
        'windows': dict((x.name, x) for x in ns if (x.bl_label == 'Window')),
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
        'scripts': scripts(),

        "nodes": nodes(),
        "scene": context.scene.name,
        "scenes": scenes(),
        "textures": {},
    }

    # export scripts
    for node in ns:
        if node.bl_idname == 'Script':
            with open(g_tmp_filepath + '/' + node.name.lower() + ".py", 'w',
                      encoding='utf-8') as f:
                f.write(bpy.data.texts[node.name].as_string())
            print("exported " + node.name)

    with open(filepath, 'w', encoding='utf-8') as f:
        #json.dump(document, f, indent=4)
        json.dump(document, f, default=to_json, indent=4)
    return {'FINISHED'}


IOAvangoOrientationHelper = orientation_helper_factory("IOAvangoOrientationHelper", axis_forward='-Z', axis_up='Y')


class ExportAvango(bpy.types.Operator, ExportHelper, IOAvangoOrientationHelper):
    '''Export selected object / scene for Avango (ASCII JSON format).'''

    bl_idname = "export_scene.avango"
    bl_label = "Export Avango"
    bl_description = "Export avango-json file and geoemtry"

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

        global_matrix = (axis_conversion(to_forward=self.axis_forward,
                                        to_up=self.axis_up,
                                        ).to_4x4())
        keywords = self.as_keywords(ignore=("filter_glob",
                                            "check_existing",
                                            "filter_glob",
                                            ))
        keywords["global_matrix"] = global_matrix

        return save(self, context, **keywords)

    def draw(self, context):
        layout = self.layout

        row = layout.row()
        row.label(text="Geometry:")


def menu_func_export(self, context):
    default_path = bpy.data.filepath.replace(".blend", ".json")
    self.layout.operator(
        ExportAvango.bl_idname,
        text="Avango (.avango)").filepath = default_path


def register():
    bpy.utils.register_class(ExportAvango)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportAvango)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)
