import bpy
from bpy.props import (StringProperty,
                       IntProperty,
                       FloatProperty,
                       IntVectorProperty,
                       FloatVectorProperty,
                       BoolProperty,
                       PointerProperty,
                       EnumProperty,
                       CollectionProperty)

enum_monomodes = [
    ("CENTER", "Center", "mono camera"),
    ("LEFT",   "Left",   "render from left eye"),
    ("RIGHT",  "Right",  "render from right eye")]

enum_pipeline_passes = [
    ("TRI_MESH_PASS", "TriMesh", "Render triangle meshes", 1),
    ("TEXTURED_QUAD_PASS", "TexturedQuad", "Render textured quads", 2),
    ("TEXTURED_SCREENSPACE_QUAD_PASS", "TexturedScreenspaceQuad",
     "Render screenspace quads", 3),
    ("PLOD_PASS", "Plod", "Render point based geometries", 4),
    ("NURBS_PASS", "Nurbs", "Render NURBS", 5),
    ("VIDEO_3D_PASS", "Video3d", "Render 3D Video avatars", 6),
    ("BBOX_PASS", "BBox", "Render bounding boxes", 7),

    ("LIGHT_VISIBILITY_PASS", "LightVisibility", "Render visible lights", 8),
    ("RESOLVE_PASS", "Resolve", "Shading", 9),

    ("DEBUG_VIEW_PASS", "DebugView", "Render debug tiles", 10),
    ("SSAA_PASS", "SSAA", "Apply screenspace antialiasing", 11),
    ("FULLSCREEN_PASS",
     "Fullscreen", "Render fullscreen quad with custom shader", 12),
    ("VOLUME_PASS", "Volume", "Render volumes", 13),
    ("SKEL_ANIM_PASS", "SkeletalAnimationPass", "Render skinned meshes", 14),
    # ("TONE_MAPPING_PASS", "ToneMapping", "Apply tone mapping operator", 14),
    # ("EMISSIVE_PASS", "Emissive", "Render emissive objects", 11),
    ("SKY_MAP_PASS", "SkyMapPass", "Render sky map", 15),
    ]

enum_resolve_background_mode = [
    ("COLOR", "Color", "solid color", 0),
    ("SKYMAP_TEXTURE", "Skymap Texture", "skymap texture", 1),
    ("QUAD_TEXTURE", "Texture", "texture", 2),
    ("CUBEMAP_TEXTURE", "Cubemap", "texture", 3),
    ]

enum_resolve_tonemapping_mode = [
    ("LINEAR", "Linear", "no tonemapping", 0),
    ("HEJL", "Hejl", "hejl", 1),
    ("REINHARD", "Reinhard", "reinhard", 2),
    ("UNCHARTED", "Uncharted", "uncharted", 3)
    ]

enum_resolve_environment_lighting_mode = [
    ("SPHEREMAP", "Spheremap", "spheremap", 0),
    ("CUBEMAP", "Cubemap", "cubemap", 1),
    ("AMBIENT_COLOR", "Ambient Color", "ambient color", 2)
    ]

enum_light_visibility_rasterization_mode = [
    ("AUTO", "AUTO", "", 0),
    ("SIMPLE", "SIMPLE", "", 1),
    ("CONSERVATIVE", "CONSERVATIVE", "conservative", 2),
    ("MULTISAMPLED_2", "MULTISAMPLED_2", "multisampled 2", 3),
    ("MULTISAMPLED_4", "MULTISAMPLED_4", "multisampled 4", 4),
    ("MULTISAMPLED_8", "MULTISAMPLED_8", "multisampled 8", 5),
    ("MULTISAMPLED_16", "MULTISAMPLED_8", "multisampled 16", 6),
    ("FULLSCREEN_FALLBACK", "FULLSCREEN_FALLBACK", "fullscreen fallback", 7),
    ]


class PipelinePassDescription(bpy.types.PropertyGroup):
    name = bpy.props.StringProperty(
        name="Name",
        description="PipelinePassDescription Name"
        )

    uuid = bpy.props.StringProperty(name="UUID")

    pass_type = bpy.props.EnumProperty(
        name="PassType",
        items=enum_pipeline_passes,
        description="PipelinePassDescription type"
        )
    show_expanded = bpy.props.BoolProperty(
        name="Set pipelinepass expanded in the user interface",
        default=True
        )
    light_visibility_rasterization_mode = EnumProperty(
        name="RasterizationMode",
        description="rasterization mode",
        items=enum_light_visibility_rasterization_mode,
        default='AUTO',
        )
    light_visibility_tile_power = IntProperty(
        name="TilePower",
        description="tile power",
        min=0, max=7,
        default=2,
        )
    resolve_background_mode = EnumProperty(
        name="Background Mode",
        description="background mode",
        items=enum_resolve_background_mode,
        default='COLOR'
        )
    resolve_background_color = FloatVectorProperty(
        name="Background Color",
        description="background color",
        min=0,
        max=1,
        default=(0.2, 0.2, 0.2),
        subtype='COLOR'
        )
    resolve_background_texture = StringProperty(
        name="Background Texture",
        description="background texture",
        default="gua_default_texture"
        )
    resolve_alternative_background_texture = StringProperty(
        name="Alternative Background Texture",
        description="alternative background texture",
        default="gua_default_texture"
        )

    resolve_vignette_softness = FloatProperty(
        name="Vignette Softness",
        description="vignette softness",
        min=0.0,
        default=1.3
        )
    resolve_vignette_coverage = FloatProperty(
        name="Vignette Coverage",
        description="vignette coverage",
        min=0.0,
        default=1.3
        )
    resolve_vignette_color = FloatVectorProperty(
        name="Vignette Color",
        description="vignette color",
        min=0,
        max=1,
        #default=(0.05, 0.05, 0.05),
        #subtype='COLOR'
        size=4
        )

    resolve_environment_lighting_mode = EnumProperty(
        name="Environment Lighting Mode",
        description="environment lighting mode",
        items=enum_resolve_environment_lighting_mode,
        default='AMBIENT_COLOR'
        )
    resolve_environment_lighting_color = FloatVectorProperty(
        name="Environment Lighting Color",
        description="environment lighting color",
        min=0,
        max=1,
        default=(0.05, 0.05, 0.05),
        subtype='COLOR'
        )
    #resolve_environment_lighting_spheremap = StringProperty(
    #    name="Environment Lighting Spheremap",
    #    description="environment lighting spheremap",
    #    default="gua_default_texture"
    #    )
    #resolve_environment_lighting_cubemap = StringProperty(
    #    name="Environment Lighting Cubemap",
    #    description="environment lighting cubemap",
    #    default="gua_default_texture"
    #    )
    resolve_environment_lighting_texture = StringProperty(
        name="Environment Lighting Texture",
        description="environment lighting texture",
        default="gua_default_texture"
        )
    resolve_alternative_environment_lighting_texture = StringProperty(
        name="Alternative Environment Lighting Texture",
        description="alternative environment lighting texture",
        default="gua_default_texture"
        )
    resolve_horizon_fade = FloatProperty(
        name="Horizon Fade",
        description="horizon fade",
        min=0.0,
        default=1.3
        )
    resolve_ssao_enable = BoolProperty(
        name="SSAO enable",
        description="enable ssao",
        default=False
        )
    resolve_ssao_radius = FloatProperty(
        name="SSAO Radius",
        description="ssao radius",
        min=0.0,
        default=1.0
        )
    resolve_ssao_intensity = FloatProperty(
        name="SSAO Intensity",
        description="ssao intensity",
        min=0.0,
        default=1.0
        )
    resolve_ssao_falloff = FloatProperty(
        name="SSAO Falloff",
        description="ssao falloff",
        min=0.0,
        default=0.1
        )
    resolve_noise_tex = StringProperty(
        name="Noise Texture",
        description="noise texture",
        default="gua_default_texture"
        )
    resolve_screen_space_shadows_enable = BoolProperty(
        name="Enable Screenspace Shadows",
        description="enable screenspace shadows",
        default=False
        )
    resolve_screen_space_shadows_radius = FloatProperty(
        name="Screen Space Shadows Radius",
        description="screen space shadows radius",
        min=0.0,
        default=1.0
        )
    resolve_screen_space_shadows_max_radius_px = FloatProperty(
        name="Screen Space Shadows Max Radius in Pixels",
        description="maximum radius in pixels",
        min=0.0,
        default=200.0
        )
    resolve_screen_space_shadows_intensity = FloatProperty(
        name="Screen Space Shadows Intensity",
        description="screen space shadows intensity",
        min=0.0,
        default=0.8
        )
    resolve_enable_fog = BoolProperty(
        name="Enable Fog",
        description="enable fog",
        default=False
        )
    resolve_fog_start = FloatProperty(
        name="Fog Start",
        description="fog start",
        min=0.0,
        default=10.0
        )
    resolve_fog_end = FloatProperty(
        name="Fog End",
        description="fog end",
        min=0.0,
        default=1000.0
        )
    resolve_tone_mapping_exposure = FloatProperty(
        name="Exposure",
        description="exposure",
        min=0.0,
        default=1.0
        )
    resolve_tone_mapping_mode = EnumProperty(
        name="Tone Mapping Operator",
        description="used tone mapping operator",
        items=enum_resolve_tonemapping_mode,
        default='UNCHARTED'
        )
    resolve_enable_debug_tiles = BoolProperty(
        name="Enable Debug Tiles",
        description="show debug tiles",
        default=False
        )
    skymap_output_texture_name = StringProperty(
        name="Output Texture Name",
        description="output texture name",
        default="day_skymap"
        )
    #skymap_light_color # LightColor
    skymap_light_direction = FloatVectorProperty(
        name="Light Direction",
        description="light direction",
        #min=-1,
        #max=1,
        default=(0.00, 1.00, 0.00),
        subtype='DIRECTION'
        )
    # LightBrightness
    skymap_ground_color = FloatVectorProperty(
        name="Ground Color",
        description="ground color",
        min=0,
        max=1,
        default=(0.05, 0.05, 0.05),
        subtype='COLOR'
        )
    # RayleighFactor
    # MieFactor

    def to_dict(self):
        new_dict = {
            'type': self.pass_type,
            'name': self.name,
        }
        if self.pass_type == "RESOLVE_PASS":
            new_dict['background_mode'] = self.resolve_background_mode
            new_dict['background_color'] = [
                self.resolve_background_color.r,
                self.resolve_background_color.g,
                self.resolve_background_color.b]
            new_dict['background_texture'] = self.resolve_background_texture
            new_dict['vignette_softness'] = self.resolve_vignette_softness
            new_dict['vignette_coverage'] = self.resolve_vignette_coverage
            new_dict['vignette_color'] = [
                self.resolve_vignette_color[0],
                self.resolve_vignette_color[1],
                self.resolve_vignette_color[2],
                self.resolve_vignette_color[3]
                ]

            new_dict['environment_lighting_mode'] = (
                self.resolve_environment_lighting_mode)
            new_dict['environment_lighting_color'] = [
                self.resolve_environment_lighting_color.r,
                self.resolve_environment_lighting_color.g,
                self.resolve_environment_lighting_color.b]
            #new_dict['environment_lighting_cubemap'] = (
            #    self.resolve_environment_lighting_cubemap)
            #new_dict['environment_lighting_spheremap'] = (
            #    self.resolve_environment_lighting_spheremap)
            new_dict['environment_lighting_texture'] = (
                self.resolve_environment_lighting_texture)

            new_dict['horizon_fade'] = self.resolve_horizon_fade

            new_dict['ssao_enable'] = self.resolve_ssao_enable
            new_dict['ssao_radius'] = self.resolve_ssao_radius
            new_dict['ssao_intensity'] = self.resolve_ssao_intensity
            new_dict['ssao_falloff'] = self.resolve_ssao_falloff

            new_dict['noise_texture'] = self.resolve_noise_tex

            new_dict['screenspace_shadows_enable'] = (
                self.resolve_screen_space_shadows_enable)
            new_dict['screenspace_shadows_radius'] = (
                self.resolve_screen_space_shadows_radius)
            new_dict['screenspace_shadows_max_radius_px'] = (
                self.resolve_screen_space_shadows_max_radius_px)
            new_dict['screenspace_shadows_intensity'] = (
                self.resolve_screen_space_shadows_intensity)

            new_dict['fog_enable'] = self.resolve_enable_fog
            new_dict['fog_start'] = self.resolve_fog_start
            new_dict['fog_end'] = self.resolve_fog_end

            new_dict['tone_mapping_exposure'] = (
                self.resolve_tone_mapping_exposure)
            new_dict['tone_mapping_mode'] = self.resolve_tone_mapping_mode

            new_dict['debug_tiles_enable'] = self.resolve_enable_debug_tiles

        if self.pass_type == "LIGHT_VISIBILITY_PASS":
            new_dict['rasterization_mode'] = (
                self.light_visibility_rasterization_mode)
            new_dict['tile_power'] = self.light_visibility_tile_power

        if self.pass_type == "SKY_MAP_PASS":
            new_dict['output_texture_name'] = self.skymap_output_texture_name
            new_dict['light_direction'] = [
                self.skymap_light_direction[0],
                self.skymap_light_direction[1],
                self.skymap_light_direction[2],
                ]
            new_dict['ground_color'] = [
                self.skymap_ground_color[0],
                self.skymap_ground_color[1],
                self.skymap_ground_color[2],
                ]

        return new_dict


class AvangoRenderSettings(bpy.types.PropertyGroup):
    @classmethod
    def register(cls):
        bpy.types.Scene.avango = PointerProperty(
            name="Avango Render Settings",
            description="Avango render settings",
            type=cls,
            )

        cls.film_exposure = FloatProperty(
            name="Exposure",
            description="Image brightness scale",
            min=0.0, max=10.0,
            default=1.0,
            )
        cls.film_transparent = BoolProperty(
            name="Transparent",
            description="World background is transparent with premultiplied "
                        "alpha",
            default=False,
            )

        cls.filter_width = FloatProperty(
            name="Filter Width",
            description="Pixel filter width",
            min=0.01, max=10.0,
            default=1.5,
            )

        # pipeline description
        cls.pipeline_passes = CollectionProperty(
            name="Pipeline Description Passes",
            description="List of all pipeline passes",
            type=PipelinePassDescription
            )

        cls.enable_abuffer = BoolProperty(
            name="enable_abuffer",
            description="Enable Transparencies",
            default=True,
            )

    @classmethod
    def unregister(cls):
        del bpy.types.Scene.avango


class AvangoObjectSettings(bpy.types.PropertyGroup):
    @classmethod
    def register(cls):
        bpy.types.Object.avango = PointerProperty(
            name="Avango Object Settings",
            description="Avango object settings",
            type=cls,
            )
        cls.use_a_boolean = BoolProperty(
            name="Avango Fancy Bool Property",
            description="Fancy bool property in avango-blender",
            )
        cls.samples = IntProperty(
            name="Samples",
            description="Number of samples to bla",
            min=1, max=2147483647,
            default=10,
            )
        cls.display_bounding_box = BoolProperty(
            name="DisplayBoundingBox",
            description='display bounding box',
            # TODO(set in Display show_bounds = True)
            default=False
            )

    @classmethod
    def unregister(cls):
        del bpy.types.Object.avango


class AvangoCameraSettings(bpy.types.PropertyGroup):
    @classmethod
    def register(cls):
        bpy.types.Camera.avango = PointerProperty(
            name="Avango Camera Settings",
            description="Avango object settings",
            type=cls,
            )

        cls.enabled = BoolProperty(
            name="enabled",
            description="enabled",
            default=True,
            )

        cls.enable_abuffer = BoolProperty(
            name="enable_abuffer",
            description="Enable Transparencies",
            default=True,
            )
        # pipeline description
        cls.pipeline_passes = CollectionProperty(
            name="Pipeline Description Passes",
            description="List of all pipeline passes",
            type=PipelinePassDescription
            )

        # mask
        cls.view_id = IntProperty(
            name="view_id",
            description="can be used to customize material parameters for "
                        "objects rendered by this camera",
            default=0,
            min=0
            )

        cls.enable_stereo = BoolProperty(
            name="enable_stereo",
            description="enable stereo",
            default=False,
            )

        cls.output_window_name = StringProperty(
            default='Window',
            description=''
            )

        cls.EyeDistance = FloatProperty(
            name="EyeDistance",
            description="set eye distance in meter",
            subtype='DISTANCE',
            unit='LENGTH',
            default=0.07
            )

        cls.EyeOffset = FloatProperty(
            name="EyeOffset",
            description="set eye offset in meter",
            subtype='DISTANCE',
            unit='LENGTH',
            default=0.0
            )

        cls.left_screen_path = StringProperty(
            name="LeftScreenPath",
            description='path to screen node used for the left eye',
            default="unknown_screen"
            )

        cls.right_screen_path = StringProperty(
            name="RightScreenPath",
            description='path to screen node used for the right eye',
            default="unknown_screen"
            )

        cls.mono_mode = EnumProperty(
            name="MonoMode",
            items=enum_monomodes,
            default='CENTER',
            )

        cls.resolution = IntVectorProperty(
            name="Resolution",
            description="resolution",
            default=(800, 600),
            min=1,
            size=2
            )

        cls.output_texture_name = StringProperty(
            name="OutputTextureName",
            description="output texture name",
            default=""
            )

        cls.output_window_name = StringProperty(
            name="OutputWindowName",
            description="output window name",
            default=""
            )

        cls.enable_frustum_culling = BoolProperty(
            name="EnableFrustumCulling",
            description="enable frustum culling",
            default=True
            )

        # PrerenderCameras
        # cls.pre_render_cameras = CollectionProperty(type=StringProperty)
# Enabled

    @classmethod
    def unregister(cls):
        del bpy.types.Camera.avango

'''
class AvangoWorldSettings(bpy.types.PropertyGroup):
class AvangoMeshSettings(bpy.types.PropertyGroup):
'''


class AvangoLampSettings(bpy.types.PropertyGroup):
    @classmethod
    def register(cls):
        bpy.types.Lamp.avango = PointerProperty(
            name="Avango Lamp Settings",
            description="Avango lamp settings",
            type=cls,
            )
        cls.cast_shadow = BoolProperty(
            name="Cast Shadow",
            description="Lamp casts shadows",
            default=True,
            )

    @classmethod
    def unregister(cls):
        del bpy.types.Lamp.avango


class AvangoMaterialSettings(bpy.types.PropertyGroup):
    @classmethod
    def register(cls):
        bpy.types.Material.avango = PointerProperty(
            name="Avango Material Settings",
            description="Avango material settings",
            type=cls,
            )
        cls.color_texture = StringProperty(
            name="Base Color Texture",
            description="If texture is set, base color will be ignored",
            #subtype='FILE_PATH',
            )
        cls.use_color_texture = BoolProperty(
            name="Use Color Texture",
            description="Base Color",
            default=False,
            )
        cls.metalness = BoolProperty(
            name="Metalness",
            description="Metalness",
            default=False,
            )
        cls.metalness_texture = StringProperty(
            name="Metalness Texture",
            description="If path is set, metalness will be ignored",
            subtype='FILE_PATH',
            )
        cls.use_metalness_texture = BoolProperty(
            name="Use Metalness Texture",
            description="Metalness Texture",
            default=False,
            )
        cls.roughness = FloatProperty(
            name="Roughness",
            description="zero means smooth and one means rough",
            min=0, max=1,
            default=0.3,
            )
        cls.roughness_texture = StringProperty(
            name="Roughness Texture",
            description="If path is set, roughness will be ignored",
            subtype='FILE_PATH',
            )
        cls.use_roughness_texture = BoolProperty(
            name="Use Roughness Texture",
            description="Roughness Texture",
            default=False,
            )
        cls.emissivity = FloatProperty(
            name="Emissivity",
            description="Emissivity",
            min=0, max=1,
            default=0.0,
            )
        cls.emissivity_texture = StringProperty(
            name="Emissivity Texture",
            description="If path is set, emissivity will be ignored",
            subtype='FILE_PATH',
            )
        cls.use_emissivity_texture = BoolProperty(
            name="Use Emissivity Texture",
            description="Emissivity Texture",
            default=False,
            )
        cls.opacity = FloatProperty(
            name="Opacity",
            description="Opacity",
            min=0, max=1,
            default=1.0,
            )
        cls.normal_texture = StringProperty(
            name="Normal Texture",
            subtype='FILE_PATH',
            )
        cls.backface_culling = BoolProperty(
            name="Backface Culling",
            description="Enable Backface Culling",
            default=True,
            )

    @classmethod
    def unregister(cls):
        del bpy.types.Material.avango

'''
class AvangoSceneSettings(bpy.types.PropertyGroup):

class Avango Physics Settings(bpy.types.PropertyGroup):
class Avango Armature Settings(bpy.types.PropertyGroup):
class Avango Sound Settings(bpy.types.PropertyGroup):

class Avango Curve Settings(bpy.types.PropertyGroup):
class Avango Image Settings(bpy.types.PropertyGroup):
class Avango Group Settings(bpy.types.PropertyGroup):
class Avango Particle Settings(bpy.types.PropertyGroup):
class Avango Action Settings(bpy.types.PropertyGroup):
'''

# context.space_data.fx_settings


def register():
    bpy.utils.register_class(PipelinePassDescription)
    bpy.utils.register_class(AvangoRenderSettings)
    bpy.utils.register_class(AvangoObjectSettings)
    bpy.utils.register_class(AvangoCameraSettings)
    bpy.utils.register_class(AvangoMaterialSettings)
    bpy.utils.register_class(AvangoLampSettings)


def unregister():
    bpy.utils.unregister_class(PipelinePassDescription)
    bpy.utils.unregister_class(AvangoRenderSettings)
    bpy.utils.unregister_class(AvangoObjectSettings)
    bpy.utils.unregister_class(AvangoCameraSettings)
    bpy.utils.unregister_class(AvangoMaterialSettings)
    bpy.utils.unregister_class(AvangoLampSettings)
