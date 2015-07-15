import avango
import avango.gua as av
import os

import bpy
from bpy.app.handlers import persistent
from mathutils import Matrix

WIN_SIZE = av.Vec2ui(1024, 768)

# it is currently impossible to close windows in guacamole. Therefore we create
# one global window, which we will use for all previews.

def texture_filepath(texname):
    return bpy.data.textures[texname].image.filepath

g_window = av.nodes.HeadlessSurface(
    Size=WIN_SIZE,
    LeftResolution=WIN_SIZE
)
av.register_window("blender_window", g_window)

g_graph = av.nodes.SceneGraph(Name="scenegraph")

g_viewer = av.nodes.BlenderViewer()
g_viewer.Window.value = g_window
g_viewer.SceneGraphs.value = [g_graph]

g_loader = av.nodes.TriMeshLoader()

g_cached_objects = {}


def matrix_to_list(matrix):
    """Convert a blender matrix to a list."""
    return sum(list(list(x) for x in matrix), [])


def from_blender_matrix4(m):
    """Convert a blender matrix to an avango Matrix4."""
    return av.transpose(av.from_list(matrix_to_list(m)))


def from_blender_color(c):
    """Convert a blender color to an avango Color."""
    return av.Color(c.r, c.g, c.b)


blender_lamp_type = {
    'POINT': av.LightType.POINT,
    'SPOT': av.LightType.SPOT,
    'SUN': av.LightType.SUN,
    'HEMI': av.LightType.SUN,
    'AREA': av.LightType.SUN
    }


def from_blender_lamp_type(c):
    """Convert a blender color to an avango Color."""
    return blender_lamp_type[c]


# context.space_data.region_3d.
def camera_from_view(camera, region, b_space_view_3d):
    rv3d = b_space_view_3d.region_3d
    camera.NearClip.value = b_space_view_3d.clip_start
    camera.FarClip.value = b_space_view_3d.clip_end
    camera.Resolution.value = av.Vec2ui(int(region.width),
                                        int(region.height))
    camera.Transform.value = av.make_inverse_mat(
        from_blender_matrix4(rv3d.view_matrix))
    camera.Mode.value = 0 if rv3d.is_perspective else 1

# perspective
    # zoom = 1
    # pixelaspect 1 : 1
    # sensor_width = 32
    # sensor_height = 18
    # sensor_fit = AUTO
    # shuttertime = 1
    # border right = 1
    # border top = 1
    # pano_viewplane.right = 1
    # pano_viewplane.top = 1
    # viewport_camera_border.right = 1
    # viewport_camera_border.top = 1
    # full_width = render_resolution_x(b_render_settings)
    # full_height = render_resolution_y(b_render_settings)
    """
    lens = b_v3d.lens
    shuttertime = b_scene.render.motion_blur_shutter
    if b_v3d.view_perspective == PERSPECTIVE_CAMERA:
        ob = b_v3d.camera
    """


def default_pipeline_passes(pipeline_passes):
    import uuid
    p = pipeline_passes.add()
    p.pass_type = 'TRI_MESH_PASS'
    p.uuid = str(uuid.uuid4())

    p = pipeline_passes.add()
    p.pass_type = 'TEXTURED_QUAD_PASS'
    p.uuid = str(uuid.uuid4())

#    p = pipeline_passes.add()
#    p.pass_type = 'TEXTURED_SCREENSPACE_QUAD_PASS'
#    p.uuid = str(uuid.uuid4())

    p = pipeline_passes.add()
    p.pass_type = 'LIGHT_VISIBILITY_PASS'
    p.uuid = str(uuid.uuid4())

    p = pipeline_passes.add()
    p.pass_type = 'RESOLVE_PASS'
    p.uuid = str(uuid.uuid4())

    p.resolve_ssao_enable = True
    p.resolve_ssao_intensity = 4.0
    p.resolve_ssao_falloff = 10.0
    p.resolve_ssao_radius = 7.0
    p.resolve_environment_lighting_color = (0.1, 0.1, 0.1)
    p.resolve_tone_mapping_mode = 'UNCHARTED'
    p.resolve_tone_mapping_exposure = 1.0
    p.resolve_background_color = (0.45, 0.5, 0.6)
    p.resolve_enable_debug_tiles = False

    p = pipeline_passes.add()
    p.pass_type = 'SSAA_PASS'
    p.uuid = str(uuid.uuid4())

    # p = pipeline_passes.add()
    # p.pass_type = 'DEBUG_VIEW_PASS'
    # p.uuid = str(uuid.uuid4())

def get_resolve_pass(pipeline_passes):
    for p in pipeline_passes:
        if p.pass_type is 'RESOLVE_PASS':
            return p
    return None

class PassCreator:
    def create(self, pipeline_passes):
        print("PassCreator.create")

        passes = []
        for p in pipeline_passes:
            # match enum type to our functions, avoids a lookup table.
            if hasattr(self, p.pass_type):
                passes.append(getattr(self, p.pass_type)(p))
        return passes

    def LIGHT_VISIBILITY_PASS(self, p):
        ap = av.nodes.LightVisibilityPassDescription()
        ap.RasterizationMode.value = eval(
            "avango.gua.RasterizationMode." +
            p.light_visibility_rasterization_mode)
        ap.TilePower.value = p.light_visibility_tile_power
        return ap

    def RESOLVE_PASS(self, p):
        res_pass = av.nodes.ResolvePassDescription()
        res_pass.BackgroundMode.value = eval(
            "avango.gua.BackgroundMode."+p.resolve_background_mode)
        res_pass.BackgroundColor.value = from_blender_color(
            p.resolve_background_color)
        res_pass.BackgroundTexture.value = p.resolve_background_texture

        res_pass.VignetteCoverage.value = p.resolve_vignette_coverage
        res_pass.VignetteSoftness.value = p.resolve_vignette_softness
        res_pass.VignetteColor.value = av.Vec4(
                p.resolve_vignette_color[0],
                p.resolve_vignette_color[1],
                p.resolve_vignette_color[2],
                p.resolve_vignette_color[3]
                )

        res_pass.EnvironmentLightingTexture.value = (
                p.resolve_environment_lighting_texture)
        res_pass.EnvironmentLightingMode.value = eval(
            "avango.gua.EnvironmentLightingMode." +
            p.resolve_environment_lighting_mode)
        res_pass.EnvironmentLightingColor.value = from_blender_color(
            p.resolve_environment_lighting_color)
        res_pass.HorizonFade.value = p.resolve_horizon_fade
        res_pass.EnableSSAO.value = p.resolve_ssao_enable
        res_pass.SSAORadius.value = p.resolve_ssao_radius
        res_pass.SSAOIntensity.value = p.resolve_ssao_intensity
        res_pass.SSAOFalloff.value = p.resolve_ssao_falloff
        res_pass.SSAONoiseTexture.value = p.resolve_noise_tex
        # jo
        res_pass.EnableScreenSpaceShadow.value = (
            p.resolve_screen_space_shadows_enable)
        res_pass.ScreenSpaceShadowRadius.value = (
            p.resolve_screen_space_shadows_radius)
        res_pass.ScreenSpaceShadowMaxRadiusPX.value = (
            p.resolve_screen_space_shadows_max_radius_px)
        res_pass.ScreenSpaceShadowIntensity.value = (
            p.resolve_screen_space_shadows_intensity)
        res_pass.EnableFog.value = p.resolve_enable_fog
        res_pass.FogStart.value = p.resolve_fog_start
        res_pass.FogEnd.value = p.resolve_fog_end
        res_pass.Exposure.value = p.resolve_tone_mapping_exposure
        res_pass.ToneMappingMode.value = eval(
            "avango.gua.ToneMappingMode." + p.resolve_tone_mapping_mode)
        res_pass.DebugTiles.value = p.resolve_enable_debug_tiles
        return res_pass

    def SKY_MAP_PASS(self, p):
        sky_pass = av.nodes.SkyMapPassDescription()
        sky_pass.OutputTextureName.value = p.skymap_output_texture_name
        sky_pass.LightDirection.value = av.Vec3(
                p.skymap_light_direction[0],
                p.skymap_light_direction[1],
                p.skymap_light_direction[2])
        #sky_pass.GroundColor.value = p.skymap_ground_color
        return sky_pass

    def TRI_MESH_PASS(self, p):
        return av.nodes.TriMeshPassDescription()

    def TEXTURED_QUAD_PASS(self, p):
        return av.nodes.TexturedQuadPassDescription()

    def TEXTURED_SCREENSPACE_QUAD_PASS(self, p):
        return av.nodes.TexturedScreenspaceQuadDescription()

    def PLOD_PASS(self, p):
        return av.nodes.PLODPassDescription()

    def NURBS_PASS(self, p):
        return av.nodes.NURBSPassDescription()

    def BBOX_PASS(self, p):
        return av.nodes.BBoxPassDescription()

    def VIDEO_3D_PASS(self, p):
        return av.nodes.Video3DPassDescription()

    def DEBUG_VIEW_PASS(self, p):
        return av.nodes.DebugViewPassDescription()

    def FULLSCREEN_PASS(self, p):
        return av.nodes.FullscreenPassDescription()

    def VOLUME_PASS(self, p):
        return av.nodes.VolumePassDescription()

    def SSAA_PASS(self, p):
        return av.nodes.SSAAPassDescription()


def pipeline_uuids(pipeline_passes):
    (p.uuid for p in pipeline_passes)


def engine_init():
    pass


def engine_create(engine, context):
    ascene = context.scene.avango
    if len(ascene.pipeline_passes) == 0:
        default_pipeline_passes(ascene.pipeline_passes)
        # engine.pipeline_uuids = list(p.uuid for p in ascene.pipeline_passes)
        ascene.enable_abuffer = True

    pc = PassCreator()
    passes = pc.create(ascene.pipeline_passes)

    engine.viewport_screen = av.nodes.ScreenNode(
        Name="viewport_screen",
        Width=2,
        Height=1.5
    )

    pipeline_description = av.nodes.PipelineDescription(
        Passes=passes,
        EnableABuffer=True,
        )

    engine.viewport_camera = av.nodes.CameraNode(
        Name=str(engine.uuid),
        LeftScreenPath="/"+str(engine.uuid)+"/viewport_screen",
        SceneGraph="scenegraph",
        Resolution=WIN_SIZE,
        OutputWindowName="blender_window",
        Children=[engine.viewport_screen],
        PipelineDescription=pipeline_description,
    )

    engine.session = {'uuid': str(engine.uuid)}

    g_graph.Root.value.Children.value.append(engine.viewport_camera)

    print("create engine uuid: ", engine.uuid)


def engine_free(engine):
    if hasattr(engine, "session"):
        print("avango-blender.engine.free " + engine.session['uuid'])
        children = g_graph.Root.value.Children.value
        g_graph.Root.value.Children.value = list(
            c for c in children if (
                c.Name.value != engine.viewport_camera.Name.value))
        engine.viewport_screen = None
        engine.viewport_camera = None
        g_viewer.unregister_engine(engine.session['uuid'])
        engine.session = None


def engine_render(engine, scene):
    print("render")
    if hasattr(engine, "session"):
        print("render " + engine.session['uuid'])


def engine_update(engine, data, scene):
    # if hasattr(engine, "session"):
    #     print("update " + engine.session['uuid'])
    # b_space_view_3d = context.space_data
    # rv3d = b_space_view_3d.region_3d
    # self.camera.NearClip.value = b_space_view_3d.clip_start
    # self.camera.FarClip.value = b_space_view_3d.clip_end
    # xs = matrix_to_list(rv3d.view_matrix)
    # print(xs)
    # #self.camera.Transform.value = av.transpose(
    #     av.from_list(xs))
    # self.camera.Transform.value = av.transpose(
    #     av.inverse(av.from_list(xs)))

    for o in scene.objects:
        if o.name not in g_cached_objects:
            if o.type == 'LAMP':
                bl_lamp = bpy.data.lamps[o.data.name]
                light = av.nodes.LightNode(
                    Type=from_blender_lamp_type(o.data.type),
                    Name=o.name,
                    Color=from_blender_color(bl_lamp.color),
                    Brightness=bl_lamp.energy,
                    Transform=from_blender_matrix4(o.matrix_world)
                )
                g_cached_objects[o.name] = light
                g_graph.Root.value.Children.value.append(light)
            elif o.type == 'MESH':
                # parent = 'null'
                # if o.parent:
                #    parent = o.parent.name
                # matrix = o.matrix_world
                filename = o.name + '.obj'

                path = '/tmp/'

                if not os.path.exists(path):
                    os.makedirs(path)

                bpy.ops.object.select_all(action='DESELECT')
                # scene.objects.active = o
                o.select = True
                world = o.matrix_world.copy()
                Matrix.identity(o.matrix_world)
                bpy.ops.export_scene.obj(
                    filepath=path+filename,
                    check_existing=False,
                    use_selection=True,
                    use_normals=True,
                    use_triangles=True,
                    use_uvs=True,
                    use_materials=True,
                    axis_forward='Y',
                    axis_up='Z',
                    path_mode='AUTO'
                    )
                o.matrix_world = world
                o.select = False

                mesh = g_loader.create_geometry_from_file(
                    o.name,
                    path+filename,
                    av.LoaderFlags.NORMALIZE_SCALE)
                for slot in o.material_slots:
                    # material = o.material_slots['Material'].material
                    material = o.material_slots[0].material
                    amaterial = material.avango
                    col = slot.material.diffuse_color

                    mesh.Material.value.set_uniform("Color",
                                                    av.Vec4(col.r,
                                                            col.g,
                                                            col.b, 1.0))
                    if slot.material.avango.use_color_texture:
                        mesh.Material.value.set_uniform("ColorMap",
                                texture_filepath(slot.material.avango.color_texture))
                    mesh.Material.value.set_uniform("Roughness",
                                                    amaterial.roughness)
                    if slot.material.avango.use_roughness_texture:
                        mesh.Material.value.set_uniform("RoughnessMap",
                                texture_filepath(slot.material.avango.roughness_texture))
                    mesh.Material.value.set_uniform("Metalness",
                                                    float(amaterial.metalness))
                    if slot.material.avango.use_metalness_texture:
                        mesh.Material.value.set_uniform("MetalnessMap",
                                texture_filepath(slot.material.avango.metalness_texture))
                    mesh.Material.value.set_uniform("Emissivity",
                                                    amaterial.emissivity)
                    mesh.Material.value.set_uniform("Opacity",
                                                    amaterial.opacity)

                g_cached_objects[o.name] = mesh
                g_graph.Root.value.Children.value.append(mesh)
            elif o.type == 'CAMERA':
                bl_camera = bpy.data.cameras[o.data.name]
                acamera = bl_camera.avango
                screen = av.nodes.ScreenNode(
                    Name="screen",
                    Width=2,
                    Height=1.5,
                    Transform=av.make_trans_mat(0.0, 0.0, -2.5),
                )

                res_pass = av.nodes.ResolvePassDescription()
                res_pass.EnableSSAO.value = True
                res_pass.SSAOIntensity.value = 4.0
                res_pass.SSAOFalloff.value = 10.0
                res_pass.SSAORadius.value = 7.0
                res_pass.EnvironmentLightingColor.value = (
                    av.Color(0.1, 0.1, 0.1))
                res_pass.ToneMappingMode.value = av.ToneMappingMode.UNCHARTED
                res_pass.Exposure.value = 1.0
                res_pass.BackgroundColor.value = av.Color(1.0, 0.0, 0.0)

                pipeline_description = av.nodes.PipelineDescription(
                    Passes=[
                        av.nodes.TriMeshPassDescription(),
                        av.nodes.LightVisibilityPassDescription(),
                        res_pass,
                        av.nodes.SSAAPassDescription(),
                        ],
                    EnableABuffer=acamera.enable_abuffer,
                    )

                camera = av.nodes.CameraNode(
                    Name=o.name,
                    LeftScreenPath="/"+o.name+"/screen",
                    SceneGraph="scenegraph",
                    Resolution=av.Vec2ui(
                        acamera.resolution[0],
                        acamera.resolution[1]),
                    OutputWindowName="blender_window",
                    Children=[screen],
                    Transform=from_blender_matrix4(o.matrix_world),
                    NearClip=bl_camera.clip_start,
                    FarClip=bl_camera.clip_end,
                    Mode=0 if o.data.type == 'PERSP' else 1,
                    PipelineDescription=pipeline_description,
                )
                g_cached_objects[o.name] = camera
                g_graph.Root.value.Children.value.append(camera)


# res_pass.EnvironmentLightingColor.value = from_blender_color(
#                                    context.scene.world.ambient_color)
# res_pass.Exposure.value = context.scene.world.exposure


def engine_draw(engine, region, v3d, rv3d):
    active_camera_name = engine.viewport_camera.Name.value
    projection = rv3d.perspective_matrix * rv3d.view_matrix.inverted()
    width = 2 * v3d.clip_start / projection[0][0]
    height = 2 * v3d.clip_start / projection[1][1]
    if v3d.region_3d.view_perspective == 'CAMERA':
        active_camera_name = v3d.camera.name
        camera = g_cached_objects[active_camera_name]
        camera.Resolution.value = av.Vec2ui(int(region.width),
                                            int(region.height))

        screen = camera.Children.value[0]
        screen.Width.value = width
        screen.Height.value = height
        screen.Transform.value = av.make_trans_mat(
            0.0, 0.0, -camera.NearClip.value)
    else:
        camera_from_view(engine.viewport_camera, region, v3d)
        engine.viewport_screen.Width.value = width
        engine.viewport_screen.Height.value = height
        engine.viewport_screen.Transform.value = av.make_trans_mat(
            0.0, 0.0, -v3d.clip_start)

    # old_res = engine.viewport_camera.Resolution.value
    # engine.viewport_camera.Resolution.value = av.Vec2ui(int(region.width),
    #                                                     int(region.height))
    g_viewer.frame(engine.session['uuid'], active_camera_name)
    # engine.viewport_camera.Resolution.value = old_res


@persistent
def scene_update(scene):
    objects = bpy.data.objects
    if objects.is_updated:

        for o in objects:
            if o.name in g_cached_objects:
                if o.is_updated or o.is_updated_data:
                    if o.type == 'LAMP':

                        bl_lamp = bpy.data.lamps[o.data.name]
                        lamp = g_cached_objects[o.name]
                        lamp.Type.value = from_blender_lamp_type(
                            o.data.type)
                        lamp.Name.value = o.name
                        lamp.Color.value = from_blender_color(bl_lamp.color)
                        lamp.Brightness.value = bl_lamp.energy
                        lamp.Transform.value = from_blender_matrix4(
                            o.matrix_world)
                    elif o.type == 'MESH':
                        mesh = g_cached_objects[o.name]
                        mesh.Transform.value = from_blender_matrix4(
                            o.matrix_world)
                        for slot in o.material_slots:
                            # material = o.material_slots['Material'].material
                            material = o.material_slots[0].material
                            if material.is_updated:
                                amaterial = material.avango
                                col = slot.material.diffuse_color

                                mesh.Material.value.set_uniform(
                                    "Color",
                                    av.Vec4(col.r,
                                            col.g,
                                            col.b, 1.0))
                                mesh.Material.value.set_uniform(
                                    "Roughness",
                                    amaterial.roughness)
                                mesh.Material.value.set_uniform(
                                    "Metalness",
                                    amaterial.metalness)
                                mesh.Material.value.set_uniform(
                                    "Emissivity",
                                    amaterial.emissivity)
                                mesh.Material.value.set_uniform(
                                    "Opacity",
                                    amaterial.opacity)
                    elif o.type == 'CAMERA':
                        camera = g_cached_objects[o.name]
                        camera.Transform.value = from_blender_matrix4(
                            o.matrix_world)
                        if o.is_updated_data:
                            pc = PassCreator()
                            ps = pc.create(o.data.avango.pipeline_passes)
                            camera.PipelineDescription.value.Passes.value = ps
                    elif o.type == 'MATERIAL':
                        print("MATERIAL ???", o.type)
                    else:
                        print("what???", o.type)


class AvangoRender(bpy.types.RenderEngine):
    bl_idname = 'AVANGO'
    bl_label = "Avango Render"
    bl_use_shading_nodes = True
    bl_use_preview = False
    bl_use_exclude_layers = True
    bl_use_save_buffers = True
    # is_animation = True

    def __init__(self):
        import uuid
        self.uuid = uuid.uuid4()
        self.session = None
        self.viewport_camera = None
        self.viewport_screen = None
        self.v3d_pipeline_uuids = None

    def __del__(self):
        engine_free(self)

    # final render
    def update(self, data, scene):
        print("update")

    # Render scene into an image
    def render(self, scene):
        engine_render(self, scene)

    # Update on data changes for viewport render
    def view_update(self, context):
        if not self.session:
            engine_create(self, context)
        engine_update(self, context.blend_data, context.scene)

    # Draw viewport render
    def view_draw(self, context):
        engine_draw(self, context.region, context.space_data,
                    context.region_data)


@persistent
def clear_cache(context):
    g_cached_objects.clear()
    g_graph.Root.value.Children.value = []


bpy.app.handlers.scene_update_post.append(scene_update)
bpy.app.handlers.load_pre.append(clear_cache)
