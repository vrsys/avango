import avango
import avango.gua
# import avango.gua.skelanim
# from avango.gua.skelanim.AnimationControl import AnimationControl
from examples_common import device

import field_containers
import application

import json
import math


class jsonloader:

    def __init__(self):
        self.json_data = None
        self.root_node = None
        self.TriMeshLoader = avango.gua.nodes.TriMeshLoader()
        # self.SkelMeshLoader =\
            # avango.gua.skelanim.nodes.SkeletalAnimationLoader()

    def create_application_from_json(self, json_path):
        print("creating application from", json_path)

        self.open_json(json_path)

        self.file_path = json_path.rpartition('/')[0] + '/'

        self.app = application.Application()

        # self.app.window = self.load_window()

        self.create_scenegraph_nodes()
        self.create_field_containers()

        self.app.basic_setup()
        self.app.apply_field_connections()

        return self.app

    def open_json(self, path):
        json_file = open(path)
        self.json_data = json.load(json_file)

    def create_scenegraph_nodes(self):
        self.nodes = {}
        child_parent_pairs = []

        new_camera, new_screen, parent_name = self.load_camera()
        self.nodes[new_camera.Name.value] = new_camera
        child_parent_pairs.append([new_camera.Name.value, parent_name])
        self.app.set_camera(new_camera)
        self.app.screen = new_screen

        self.load_materials()

        for mesh in self.json_data["meshes"]:
            new_mesh, parent_name = self.load_mesh(mesh)
            self.nodes[new_mesh.Name.value] = new_mesh
            child_parent_pairs.append([new_mesh.Name.value, parent_name])

        for transform in self.json_data["transforms"]:
            new_transform, parent_name = self.load_transform(transform)
            self.nodes[new_transform.Name.value] = new_transform
            child_parent_pairs.append([new_transform.Name.value, parent_name])

        for light in self.json_data["lights"]:
            new_light, parent_name = self.load_light(light)
            self.nodes[new_light.Name.value] = new_light
            child_parent_pairs.append([new_light.Name.value, parent_name])

        self.create_scenegraph_structure(child_parent_pairs)

    def create_field_containers(self):
        # TODO do something not useless
        self.create_time_sensor()
        for ts in self.json_data["time_sensors"]:
            self.setup_time_sensor(ts)

        self.create_keyboard()
        for k in self.json_data["keyboards"]:
            self.setup_keyboard(k)

        for rm in self.json_data["rotation_matrices"]:
            self.create_rotation_matrix(rm)

        for tm in self.json_data["translation_matrices"]:
            self.create_translation_matrix(tm)

        for fcfo in self.json_data["from_objects"]:
            self.create_field_container_from_object(fcfo)

        for vec3 in self.json_data["vec3s"]:
            self.create_vec3(vec3)

        for script in self.json_data["scripts"]:
            field_containers.script.create_new_script(
                self.json_data["scripts"][script],
                self.app,
                self.file_path,
            )

        for fm in self.json_data["floatmaths"]:
            self.create_float_math(fm)

    # TIME SENSOR
    def create_time_sensor(self):
        name = "time_sensor"
        new_time_sensor = avango.nodes.TimeSensor(Name=name)
        self.app.add_field_container(new_time_sensor)

    def setup_time_sensor(self, time_sensor):
        json_time_sensor = self.json_data["time_sensors"][time_sensor]
        for fieldconnection in json_time_sensor["field_connections"]:
            self.app.plan_field_connection(
                "time_sensor",
                fieldconnection["from_field"],
                fieldconnection["to_node"],
                fieldconnection["to_field"],
            )

    # KEYBOARD
    def create_keyboard(self):
        new_keyboard = device.KeyboardDevice()
        new_keyboard.Name.value = "keyboard"
        self.app.add_field_container(new_keyboard)

    def setup_keyboard(self, keyboard):
        json_keyboard = self.json_data["keyboards"][keyboard]
        for fieldconnection in json_keyboard["field_connections"]:
            self.app.plan_field_connection(
                "keyboard",
                fieldconnection["from_field"],
                fieldconnection["to_node"],
                fieldconnection["to_field"],
                )

    def create_rotation_matrix(self, rotation_matrix):
        new_field_container = field_containers.rotation_matrix.RotationMatrix()
        new_field_container.constructor(
            self.json_data["rotation_matrices"][rotation_matrix], self.app)

    def create_translation_matrix(self, translation_matrix):
        new_field_container =\
            field_containers.translation_matrix.TranslationMatrix()
        new_field_container.constructor(
            self.json_data["translation_matrices"][translation_matrix],
            self.app,
            )

    def create_field_container_from_object(
            self,
            field_containers_from_objects):

        json_fcfo =\
            self.json_data["from_objects"][field_containers_from_objects]

        ref_name = json_fcfo["referenced_name"]
        ref_name = ref_name.replace('.', '_')
        obj = self.nodes[ref_name]

        if json_fcfo["import_action"]:
            new_action = field_containers.action.Action()
            new_action.constructor(json_fcfo['keyframes'])
            new_action.Name.value = ref_name + '_action'
            new_action.loop = json_fcfo['loop_action']

            self.app.add_field_container(new_action)
            self.app.plan_field_connection(
                new_action.Name.value,
                "OutTransform",
                ref_name,
                "Transform",
                )
            self.app.plan_field_connection(
                "time_sensor",
                "Time",
                new_action.Name.value,
                "Time",
                )

        if not json_fcfo["animation_path"] == "":
            print("Animation not supported right now")
            # self.SkelMeshLoader.load_animation(
                # obj, json_fcfo["animation_path"], "idle")
            # AC = AnimationControl()
            # AC.Name.value = "animation_control_for_" + ref_name
            # AC.my_constructor(obj)
            # self.app.add_field_container(AC)

        self.app.add_field_container(obj)

    def create_vec3(self, vec3):
        new_field_container = field_containers.vec3.Vec3()
        new_field_container.constructor(
            self.json_data["vec3s"][vec3], self.app)

    def create_float_math(self, fm):
        new_field_container = field_containers.float_math.FloatMath()
        new_field_container.constructor(
            self.json_data["floatmaths"][fm], self.app)

    def create_scenegraph_structure(self, child_parent_pairs):
        for pair in child_parent_pairs:
            if pair[1] == "null":
                self.app.root.Children.value.append(self.nodes[pair[0]])
            else:
                self.nodes[pair[1]].Children.value.append(self.nodes[pair[0]])

    def load_window(self):
        print("load window")

        json_window = self.json_data["windows"]["Window"]

        title = str(json_window["title"])
        name = str(json_window["name"])
        size = avango.gua.Vec2ui(json_window["left_resolution"][0],
                                 json_window["left_resolution"][1])

        mode = 0
        if (json_window["mode"] == "MONO"):
            mode = 0
        # TODO more stereo modes

        display = str(json_window["display"])

        new_window = avango.gua.nodes.GlfwWindow(
            Name=name,
            Size=size,
            LeftResolution=size,
            StereoMode=mode,
            Title=title,
            Display=display,
        )

        avango.gua.register_window(name, new_window)

        return new_window

    def load_materials(self):
        json_materials = self.json_data["materials"]

        self.materials = {}

        self.materials["default_material"] = avango.gua.nodes.Material()

        for mat in json_materials:
            new_mat = avango.gua.nodes.Material()

            if json_materials[mat]['color_map'] == "":
                color = avango.gua.Vec4(
                    json_materials[mat]['color'][0],
                    json_materials[mat]['color'][1],
                    json_materials[mat]['color'][2],
                    json_materials[mat]['opacity'],
                )
                new_mat.set_uniform('Color', color)
                new_mat.set_uniform('Opacity', json_materials[mat]['opacity'])
            else:
                new_mat.set_uniform(
                    "ColorMap", json_materials[mat]['color_map'])

            if json_materials[mat]['roughness_map'] == "":
                new_mat.set_uniform(
                    'Roughness', json_materials[mat]['roughness'])
            else:
                new_mat.set_uniform(
                    "RoughnessMap", json_materials[mat]['roughness_map'])

            if json_materials[mat]['metalness_map'] == "":
                new_mat.set_uniform(
                    'Metalness', json_materials[mat]['metalness'])
            else:
                new_mat.set_uniform(
                    "MetalnessMap", json_materials[mat]['metalness_map'])

            if json_materials[mat]['emissivity_map'] == "":
                new_mat.set_uniform(
                    'Emissivity', json_materials[mat]['emissivity'])
            else:
                new_mat.set_uniform(
                    "EmissivityMap", json_materials[mat]['emissivity_map'])

            if not json_materials[mat]['normal_map'] == "":
                new_mat.set_uniform("NormalMap",
                                    json_materials[mat]['normal_map'])

            new_mat.EnableBackfaceCulling.value =\
                json_materials[mat]['backface_culling']
            self.materials[mat] = new_mat

    def load_mesh(self, mesh):
        print("load mesh", mesh)

        json_mesh = self.json_data["meshes"][mesh]

        name = str(json_mesh["name"])
        name = name.replace('.', '_')

        parent_name = str(json_mesh["parent"])
        parent_name = parent_name.replace('.', '_')

        transform = load_transform_matrix(json_mesh["transform"])
        path = self.file_path + str(json_mesh["file"])

        if json_mesh["has_armature"]:
            geometry = self.SkelMeshLoader.create_geometry_from_file(
                name, path, 0)
                 # avango.gua.LoaderFlags.LOAD_MATERIALS

            for i in range(len(json_mesh["material"])):
                material_name = json_mesh["material"][i]
                geometry.Materials.value[i] = self.materials[material_name]
        else:
            material = self.materials[json_mesh["material"]]
            geometry = self.TriMeshLoader.create_geometry_from_file(
                name, path, material, 0)
                 # avango.gua.LoaderFlags.LOAD_MATERIALS

        geometry.Transform.value = transform

        return geometry, parent_name

    def load_camera(self):
        print("load camera")

        json_camera = self.json_data["camera"]

        name = str(json_camera["name"])
        parent_name = str(json_camera["parent"])

        transform = load_transform_matrix(json_camera["transform"])

        resolution = avango.gua.Vec2ui(json_camera["resolution"][0],
                                       json_camera["resolution"][1])
        # output_window = str(json_camera["output_window_name"])

        # calculate a screen
        fov = json_camera["field_of_view"]
        width = math.tan(fov/2.0) * 2.0
        height = width * 9.0 / 16.0
        screen = avango.gua.nodes.ScreenNode(
            Name="generated_screen",
            Width=width,
            Height=height,
        )
        screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -1.0)

        cam = avango.gua.nodes.CameraNode(
            Name=name,
            # LeftScreenPath="",
            SceneGraph="SceneGraph",
            Resolution=resolution,
            OutputWindowName="window",
            Transform=transform,
        )

        # create pipeline decription and passes
        json_passes = json_camera["passes"]
        passes = list(map(load_pipeline_pass, json_passes))

        pipeline_description =\
            avango.gua.nodes.PipelineDescription(Passes=passes)
        pipeline_description.EnableABuffer.value =\
            json_camera["enable_abuffer"]
        cam.PipelineDescription.value = pipeline_description

        cam.Children.value.append(screen)

        return cam, screen, parent_name

    def load_transform(self, transform):
        print("load transform", transform)

        json_transform = self.json_data["transforms"][transform]

        name = str(json_transform["name"])
        parent_name = str(json_transform["parent"])

        transform = load_transform_matrix(json_transform["transform"])

        node = avango.gua.nodes.TransformNode(Name=name)
        node.Transform.value = transform

        return node, parent_name

    def load_light(self, light):
        print("load light", light)

        json_light = self.json_data["lights"][light]

        name = str(json_light["name"]).replace('.', '_')
        parent_name = str(json_light["parent"])

        transform = load_transform_matrix(json_light["transform"])

        transform = transform

        color = avango.gua.Color(
            json_light["color"][0],
            json_light["color"][1],
            json_light["color"][2],
        )

        brightness = json_light["brightness"]

        light_type = eval("avango.gua.LightType." + json_light["type"])

        light = avango.gua.nodes.LightNode(
            Name=name,
            Type=light_type,
            Transform=transform,
            Color=color,
            EnableShadows=True,
            Brightness=brightness,
        )

        return light, parent_name


def load_transform_matrix(matrix_list):
    transform = avango.gua.make_identity_mat()

    for element in range(len(matrix_list)):
        transform.set_element(int(element/4), element % 4,
                              matrix_list[element])

    return transform


def load_pipeline_pass(json_pass):
    if json_pass["type"] == "RESOLVE_PASS":
        res_pass = avango.gua.nodes.ResolvePassDescription()
        res_pass.BackgroundMode.value = \
            eval("avango.gua.BackgroundMode."+json_pass["background_mode"])
        res_pass.BackgroundColor.value = avango.gua.Color(
            json_pass["background_color"][0],
            json_pass["background_color"][1],
            json_pass["background_color"][2],
        )

        res_pass.EnvironmentLightingMode.value =\
            eval("avango.gua.EnvironmentLightingMode." +
                 json_pass["environment_lighting_mode"])
        res_pass.EnvironmentLightingColor.value = avango.gua.Color(
            json_pass["environment_lighting_color"][0],
            json_pass["environment_lighting_color"][1],
            json_pass["environment_lighting_color"][2],
        )
        # res_pass.EnvironmentLightingSphereMap.value =\
            # json_pass["environment_lighting_spheremap"]

        res_pass.HorizonFade.value = json_pass["horizon_fade"]

        res_pass.EnableSSAO.value = json_pass["ssao_enable"]
        res_pass.SSAORadius.value = json_pass["ssao_radius"]
        res_pass.SSAOIntensity.value = json_pass["ssao_intensity"]
        res_pass.SSAOFalloff.value = json_pass["ssao_falloff"]
        res_pass.SSAONoiseTexture.value = json_pass["noise_texture"]

        res_pass.EnableScreenSpaceShadow.value =\
            json_pass["screenspace_shadows_enable"]
        res_pass.ScreenSpaceShadowRadius.value =\
            json_pass["screenspace_shadows_radius"]
        res_pass.ScreenSpaceShadowMaxRadiusPX.value =\
            json_pass["screenspace_shadows_max_radius_px"]
        res_pass.ScreenSpaceShadowIntensity.value =\
            json_pass["screenspace_shadows_intensity"]

        res_pass.EnableFog.value = json_pass["fog_enable"]
        res_pass.FogStart.value = json_pass["fog_start"]
        res_pass.FogEnd.value = json_pass["fog_end"]

        res_pass.Exposure.value = json_pass["tone_mapping_exposure"]
        res_pass.ToneMappingMode.value =\
            eval("avango.gua.ToneMappingMode."+json_pass["tone_mapping_mode"])

        res_pass.DebugTiles.value = json_pass["debug_tiles_enable"]

        return res_pass

    elif json_pass["type"] == "LIGHT_VISIBILITY_PASS":
        light_pass = avango.gua.nodes.LightVisibilityPassDescription()

        light_pass.RasterizationMode.value =\
            eval("avango.gua.RasterizationMode." +
                 json_pass["rasterization_mode"])
        light_pass.TilePower.value = json_pass["tile_power"]

        return light_pass

    elif json_pass["type"] == "TRI_MESH_PASS":
        return avango.gua.nodes.TriMeshPassDescription()

    # elif json_pass["type"] == "SKEL_ANIM_PASS":
        # return avango.gua.skelanim.nodes.SkeletalAnimationPassDescription()

    elif json_pass["type"] == "TEXTURED_QUAD_PASS":
        return avango.gua.nodes.TexturedQuadPassDescription()

    elif json_pass["type"] == "TEXTURED_SCREENSPACE_QUAD_PASS":
        return avango.gua.nodes.TexturedScreenSpaceQuadPassDescription()

    elif json_pass["type"] == "BBOX_PASS":
        return avango.gua.nodes.BBoxPassDescription()

    elif json_pass["type"] == "DEBUG_VIEW_PASS":
        return avango.gua.nodes.DebugViewPassDescription()

    elif json_pass["type"] == "SSAA_PASS":
        return avango.gua.nodes.SSAAPassDescription()

 # Not Working
    elif json_pass["type"] == "FULLSCREEN_PASS":
        return avango.gua.nodes.FullscreenPassDescription()

    elif json_pass["type"] == "VOLUME_PASS":
        return avango.gua.nodes.VolumePassDescription()

    elif json_pass["type"] == "PLOD_PASS":
        return avango.gua.nodes.PLODPassDescription()

    elif json_pass["type"] == "NURBS_PASS":
        return avango.gua.nodes.NurbsMeshPassDescription()

    elif json_pass["type"] == "VIDEO_3D_PASS":
        return avango.gua.nodes.Video3DPassDescription()
