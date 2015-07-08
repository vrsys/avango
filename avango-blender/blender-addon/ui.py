import bpy
from bpy.types import Panel, Operator
from .properties import enum_pipeline_passes


class AvangoButtonsPanel():
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"
    COMPAT_ENGINES = {'AVANGO'}

    @classmethod
    def poll(cls, context):
        rd = context.scene.render
        return rd.engine in cls.COMPAT_ENGINES
        #return True


# scene references node_tree
#   when an object is added to bpy.context.scene, it will inherit the same
#   node_tree reference
# id_data in Lamp, Mesh, Camera, Empty, Lattice, Speaker, Armature, Text,
#   Metaball, Surface, Curve
def panel_node_draw(layout, id_data, output_type, input_name):
    # ntree = id_data.node_tree
    pass


def template_pipeline_pass(layout, acam, p):
    column = layout.column(align=True)
    box = column.box()
    row = box.row(align=False)

    sub = row.row(align=True)
    sub.alignment = 'LEFT'
    sub.prop(p, "show_expanded", icon_only=True, text="", emboss=False,
             icon='TRIA_DOWN' if p.show_expanded else 'TRIA_RIGHT')

    row.label(text="", icon='MOD_ARRAY')
    row.prop(p, "pass_type", text="", icon='NONE')
    # up and down arrows aligned
    sub = row.row(align=True)
    op = sub.operator("AVANGO_OT_pipeline_pass_move_up", text="",
                      icon='TRIA_UP')
    op.uuid = p.uuid
    op = sub.operator("AVANGO_OT_pipeline_pass_move_down", text="",
                      icon='TRIA_DOWN')
    op.uuid = p.uuid

    op = row.operator("AVANGO_OT_pipeline_pass_remove", text="", icon='X')
    op.uuid = p.uuid

    if p.show_expanded:
        box = column.box()
        #row = box.row(align=False)
        #row.operator("render.render", text="Apply")
        #row.operator("render.render", text="Copy")
        return box
    else:
        return None


class AvangoLamp_PT_nodes(AvangoButtonsPanel, Panel):
    bl_label = "Nodes"
    bl_context = "data"

    @classmethod
    def poll(cls, context):
        return context.lamp and AvangoButtonsPanel.poll(context)

    def draw(self, context):
        pass

        #lamp = context.lamp
        #if not panel_node_draw(layout, lamp, 'OUTPUT_LAMP', 'Surface'):
        #    #layout.prop(lamp, "color")
        #    layout.label("well well, nothing to see here.")


class AvangoObject_PT_nodes(AvangoButtonsPanel, Panel):
    bl_label = "Avango Object Thingy"
    bl_context = "object"
    #bl_idname = "OBJECT_PT_so"

    @classmethod
    def poll(cls, context):
        return context.object and AvangoButtonsPanel.poll(context)

    def draw(self, context):
        layout = self.layout
        obj = context.object
        aobj = obj.avango

        layout.prop(aobj, "display_bounding_box", text="BoundingBox")

        split = layout.split()

        col = split.column()
        col.label("Focus:")
        col.label("on stuff")

        sub = col.row()
        #sub.active =
        col = split.column()

        col.label("Aperture thingy:")
        sub = col.column(align=True)
        sub.prop(aobj, "samples", text="Samples")
        sub.prop(aobj, "samples", text="Samples")


class AvangoCamera_PT_nodes(AvangoButtonsPanel, Panel):
    bl_label = "Avango Camera Settings"
    bl_context = "data"

    @classmethod
    def poll(cls, context):
        return context.camera and AvangoButtonsPanel.poll(context)

    def draw(self, context):
        layout = self.layout

        cam = context.camera
        acam = cam.avango

        layout.prop(acam, "enabled", text="Enabled")
        layout.prop(acam, "view_id", text="ViewID")
        layout.label("Mask: BlackList, WhiteList - TODO")
        layout.prop(acam, "resolution", text="Resolution")
        layout.prop(acam, "output_texture_name", text="To texture")
        layout.prop(acam, "output_window_name", text="Window")
        layout.label("Lens: ")
        layout.prop(cam, "type", text="Camera type", expand=True)

        layout.label("Clipping:")
        #split = layout.split()

        sub = layout.column(align=True)
        sub.prop(cam, "clip_start", text="Near Clip")
        sub.prop(cam, "clip_end",   text="Far Clip")

        layout.prop(acam, "enable_frustum_culling", text="Frustum Culling")
        layout.prop(acam, "mono_mode", text="Mono Mode", expand=True)
        layout.prop(acam, "enable_stereo", text="Stereo Mode")
        if acam.enable_stereo:
            col = layout.column(align=True)
            col.prop(acam, "left_screen_path", text="Left Screen")
            col.prop(acam, "right_screen_path", text="Right Screen")

            col = layout.column(align=True)
            col.prop(acam, "eye_distance", text="Eye Distance")
            col.prop(acam, "eye_offset", text="Eye Offset")
        else:
            layout.prop(acam, "left_screen_path", text="Screen")
        layout.label("PrerenderCameras: TODO")


class AVANGO_OT_pipeline_pass_add(Operator):
    """Add pipeline pass to pipeline"""
    bl_idname = "avango.pipeline_pass_add"
    bl_label = "Add Pass"
    bl_description = "Add a pass to the pipeline description"
    bl_options = {'INTERNAL', 'UNDO', 'REGISTER'}

    pass_type = bpy.props.EnumProperty(name="", items=enum_pipeline_passes,
                                       default="TRI_MESH_PASS")

    @classmethod
    def poll(cls, context):
        return getattr(context, "camera", False)

    def execute(self, context):
        cam = context.camera
        acam = cam.avango

        p = acam.pipeline_passes.add()
        p.pass_type = self.pass_type
        import uuid
        p.uuid = str(uuid.uuid4())

        return {'FINISHED'}


class AVANGO_OT_pipeline_pass_move_up(Operator):
    """Move Up Pipeline Pass"""
    bl_idname = "avango.pipeline_pass_move_up"
    bl_label = "Move Up Pipeline Pass"
    bl_description = "Move pipeline pass up in the stack"
    bl_options = {'INTERNAL', 'UNDO', 'REGISTER'}

    uuid = bpy.props.StringProperty(name="UUID")

    @classmethod
    def poll(cls, context):
        return getattr(context, "camera", False)

    def execute(self, context):
        cam = context.camera
        acam = cam.avango
        i = next((i for (i, p) in enumerate(acam.pipeline_passes)
                 if self.uuid == p.uuid), -1)
        if i > 0:
            acam.pipeline_passes.move(i, i-1)
        return {'FINISHED'}


class AVANGO_OT_pipeline_pass_move_down(Operator):
    """Move Up Pipeline Pass"""
    bl_idname = "avango.pipeline_pass_move_down"
    bl_label = "Move Down Pipeline Pass"
    bl_description = "Move pipeline pass down in the stack"
    bl_options = {'INTERNAL', 'UNDO', 'REGISTER'}

    uuid = bpy.props.StringProperty(name="UUID")

    @classmethod
    def poll(cls, context):
        return getattr(context, "camera", False)

    def execute(self, context):
        cam = context.camera
        acam = cam.avango
        i = next((i for (i, p) in enumerate(acam.pipeline_passes)
                 if self.uuid == p.uuid), -1)
        if i > -1:
            acam.pipeline_passes.move(i, i+1)
        return {'FINISHED'}


class AVANGO_OT_pipeline_pass_remove(Operator):
    """Remove a pipeline pass from the active camera"""
    bl_idname = "avango.pipeline_pass_remove"
    bl_label = "Remove Pass"
    bl_description = "Remove a pipeline pass from the active camera"
    bl_options = {'INTERNAL', 'UNDO', 'REGISTER'}

    uuid = bpy.props.StringProperty(name="UUID")

    @classmethod
    def poll(cls, context):
        return getattr(context, "camera", False)

    def execute(self, context):
        cam = context.camera
        acam = cam.avango

        i = next((i for (i, p) in enumerate(acam.pipeline_passes)
                 if self.uuid == p.uuid), -1)
        if i > -1:
            acam.pipeline_passes.remove(i)
        return {'FINISHED'}

    def invoke(self, context, event):
        #self.pipeline_pass = event
        #print(dir(self))
        #print(dir(context))

        return self.execute(context)


class AvangoCamera_PT_pipeline(AvangoButtonsPanel, Panel):
    bl_label = "Pipeline"
    bl_context = "data"
    #bl_options = {'DEFAULT_CLOSED'}

    @classmethod
    def poll(cls, context):
        return context.camera and AvangoButtonsPanel.poll(context)

    def draw(self, context):
        layout = self.layout

        cam = context.camera
        acam = cam.avango

        layout.prop(acam, "enable_abuffer")
        layout.operator_menu_enum("avango.pipeline_pass_add", "pass_type")

        for p in acam.pipeline_passes:
            box = template_pipeline_pass(layout, acam, p)
            if box:
                # match enum type to our functions, avoids a lookup table.
                getattr(self, p.pass_type)(box, acam, p)

        #
        #layout.template_node_link(ntree, node, socket)

        # pipeline stuff
        #layout.operator_menu_enum("camera.pipeline_pass_add", "type")
        # name = "Add Pass"
        # description = "Add a pipeline pass to the active camera"
        # idname = "CAMERA_OT_pipeline_pass_add"
        # invoke = WM_menu_invoke
        # exec = pipeline_pass_add_exec
        # poll =
        # flag = OPTYPE_REGISTER | OPTYPE_UNDO

    def TRI_MESH_PASS(self, layout, cam, p):
        pass
        #row = layout.row(align=False)
        #row.label("TRI_MESH_PASS")

    def LIGHT_VISIBILITY_PASS(self, layout, cam, p):
        #row = layout.row(align=False)
        #row.label("LIGHT_VISIBILITY_PASS")
        row = layout.row()
        row.prop(p, "light_visibility_rasterization_mode")
        row = layout.row()
        row.prop(p, "light_visibility_tile_power")

    def RESOLVE_PASS(self, layout, cam, p):
        row = layout.row(align=False)

        #row.label("RESOLVE_PASS")

        #row.prop(p, "name")
        #row.prop(p, "pass_type")

        row = layout.row()
        row.prop(p, "resolve_background_mode")
        if p.resolve_background_mode == 'COLOR':
            row = layout.row()
            row.prop(p, "resolve_background_color")
        elif p.resolve_background_mode == 'QUAD_TEXTURE':
            row = layout.row()
            row.prop(p, "resolve_background_texture")
        elif p.resolve_background_mode == 'CUBEMAP_TEXTURE':
            row = layout.row()
            row.prop(p, "resolve_background_texture")

        row = layout.row()
        row.prop(p, "resolve_environment_lighting_mode")
        if p.resolve_environment_lighting_mode == 'AMBIENT_COLOR':
            row = layout.row()
            row.prop(p, "resolve_environment_lighting_color")
        elif p.resolve_environment_lighting_mode == 'SPHEREMAP':
            row = layout.row()
            row.prop(p, "resolve_environment_lighting_spheremap")
        elif p.resolve_environment_lighting_mode == 'CUBEMAP':
            row = layout.row()
            row.prop(p, "resolve_environment_lighting_cubemap")

        row = layout.row()
        row.prop(p, "resolve_horizon_fade")

        row = layout.row()
        row.prop(p, "resolve_ssao_enable")
        if p.resolve_ssao_enable:
            row = layout.row()
            row.prop(p, "resolve_ssao_radius")
            row = layout.row()
            row.prop(p, "resolve_ssao_intensity")
            row = layout.row()
            row.prop(p, "resolve_ssao_falloff")
            row = layout.row()
            row.prop(p, "resolve_noise_tex")

        row = layout.row()
        row.prop(p, "resolve_screen_space_shadows_enable")
        if p.resolve_screen_space_shadows_enable:
            row = layout.row()
            row.prop(p, "resolve_screen_space_shadows_radius")
            row = layout.row()
            row.prop(p, "resolve_screen_space_shadows_max_radius_px")
            row = layout.row()
            row.prop(p, "resolve_screen_space_shadows_intensity")
        row = layout.row()
        row.prop(p, "resolve_enable_fog")
        if p.resolve_enable_fog:
            row = layout.row()
            row.prop(p, "resolve_fog_start")
            row = layout.row()
            row.prop(p, "resolve_fog_end")
        row = layout.row()
        row.prop(p, "resolve_tone_mapping_exposure")
        row = layout.row()
        row.prop(p, "resolve_tone_mapping_mode")
        row = layout.row()
        row.prop(p, "resolve_enable_debug_tiles")

    def SKY_MAP_PASS(self, layout, cam, p):
        row = layout.row()
        row.prop(p, "skymap_output_texture_name")
        row.prop(p, "skymap_light_direction")
        row.prop(p, "skymap_ground_color")

    def TEXTURED_QUAD_PASS(self, layout, cam, p):
        pass

    def TEXTURED_SCREENSPACE_QUAD_PASS(self, layout, cam, p):
        pass

    def PLOD_PASS(self, layout, cam, p):
        pass

    def NURBS_PASS(self, layout, cam, p):
        pass

    def BBOX_PASS(self, layout, cam, p):
        pass

    def VIDEO_3D_PASS(self, layout, cam, p):
        pass

    def DEBUG_VIEW_PASS(self, layout, cam, p):
        pass

    def FULLSCREEN_PASS(self, layout, cam, p):
        pass

    def VOLUME_PASS(self, layout, cam, p):
        pass

    def SSAA_PASS(self, layout, cam, p):
        pass

    def SKEL_ANIM_PASS(self, layout, cam, p):
        pass


#    def TONE_MAPPING_PASS(self, layout, cam, p):
#        pass


#    def EMISSIVE_PASS(self, layout, cam, p):
#        pass


class AvangoMaterial_PT_material(AvangoButtonsPanel, Panel):
    bl_label = "Avango PBR Material"
    bl_context = "material"

    @classmethod
    def poll(cls, context):
        return context.material and AvangoButtonsPanel.poll(context)

    def draw(self, context):
        layout = self.layout
        material = context.material
        amaterial = material.avango
        row = layout.row()
        row.prop(material, "diffuse_color", text="Base Color")
        row.prop(amaterial, "use_color_texture", text="", icon='TEXTURE',
                 toggle=1)
        if amaterial.use_color_texture:
            layout.prop_search(amaterial, "color_texture", bpy.data, 'textures',
                               text='', icon='TEXTURE_DATA')

        layout.prop(amaterial, "opacity")

        row = layout.row()
        if amaterial.use_roughness_texture:
            row.prop_search(amaterial, "roughness_texture", bpy.data, 'textures',
                               text='', icon='TEXTURE_DATA')
        else:
            row.prop(amaterial, "roughness")
        row.prop(amaterial, "use_roughness_texture", text="", icon='TEXTURE',
                 toggle=1)

        row = layout.row()
        if amaterial.use_metalness_texture:
            row.prop_search(amaterial, "metalness_texture", bpy.data, 'textures',
                               text='', icon='TEXTURE_DATA')
        else:
            row.prop(amaterial, "metalness", text="Metall")
        row.prop(amaterial, "use_metalness_texture", text="", icon='TEXTURE',
                 toggle=1)

        # actually mix between shaded fragment and base color

        row = layout.row()
        if amaterial.use_emissivity_texture:
            row.prop_search(amaterial, "emissivity_texture", bpy.data, 'textures',
                               text='', icon='TEXTURE_DATA')
        else:
            row.prop(amaterial, "emissivity", text="Emissive Color")
        row.prop(amaterial, "use_emissivity_texture", text="", icon='TEXTURE',
                 toggle=1)

        row = layout.row()
        row.label("Normal")
        row.prop_search(amaterial, "normal_texture", bpy.data, 'textures',
                           text='', icon='TEXTURE_DATA')

        layout.prop(amaterial, "backface_culling")
        # layout.label("Ambient Occlusion")

        '''
        split = layout.split(percentage=0.35, align=False)
        row = split.row()
        row.label("Roughness")
        split.prop(amaterial, "roughness", text="")

        split = layout.split(percentage=0.35, align=False)
        row = split.row()
        row.label("Metalness")
        split.prop(amaterial, "metalness", text="")

        split = layout.split(percentage=0.35, align=False)
        row = split.row()
        row.label("Emissivity")
        split.prop(amaterial, "emissivity", text="")
        '''


class AvangoLamp_PT_lamp(AvangoButtonsPanel, Panel):
    bl_label = "Lamp"
    bl_context = "data"

    @classmethod
    def poll(cls, context):
        return context.lamp and AvangoButtonsPanel.poll(context)

    def draw(self, context):
        layout = self.layout

        lamp = context.lamp
        #alamp = lamp.avango
        #ascene = context.scene.avango

        layout.prop(lamp, "type", expand=True)

        if lamp.type in {'HEMI', 'AREA'}:
            layout.label(text="Not supported, interpreted as sun lamp")

        split = layout.split()

        col = split.column()
        sub = col.column()
        sub.prop(lamp, "color", text="")
        sub.prop(lamp, "energy", text="Brightness")

        if lamp.type in {'POINT', 'SPOT'}:
            sub.label(text="Falloff:")
            sub.prop(lamp, "falloff_type", text="")
            sub.prop(lamp, "distance")

            if lamp.falloff_type == 'LINEAR_QUADRATIC_WEIGHTED':
                col.label(text="Attenuation Factors:")
                sub = col.column(align=True)
                sub.prop(lamp, "linear_attenuation", slider=True,
                         text="Linear")
                sub.prop(lamp, "quadratic_attenuation", slider=True,
                         text="Quadratic")

            col.prop(lamp, "use_sphere")

        #if lamp.type == 'AREA':
        #    col.prop(lamp, "distance")
        #    col.prop(lamp, "gamma")

        col = split.column()
        col.prop(lamp, "use_specular")
        col.prop(lamp, "use_diffuse")
        #col.prop(alamp, "cast_shadow")
        col.prop(lamp, "use_shadow")


class DATA_PT_shadow(AvangoButtonsPanel, Panel):
    bl_label = "Shadow"
    bl_context = "data"
    COMPAT_ENGINES = {'AVANGO'}

    @classmethod
    def poll(cls, context):
        COMPAT_LIGHTS = {'SPOT', 'SUN'}
        lamp = context.lamp
        engine = context.scene.render.engine
        return (lamp and lamp.type in COMPAT_LIGHTS) and\
            (engine in cls.COMPAT_ENGINES)

    def draw_header(self, context):
        lamp = context.lamp

        self.layout.prop(lamp, "use_shadow", text="")

    def draw(self, context):
        layout = self.layout

        lamp = context.lamp
        # alamp = lamp.avango
        # ascene = context.scene.avango

        layout.active = lamp.use_shadow

        split = layout.split()

        col = split.column()
        #col.prop(lamp, "shadow_color", text="")

        #col = split.column()
        #col.prop(lamp, "use_shadow_layer", text="This Layer Only")
        #col.prop(lamp, "use_only_shadow")

        #col = layout.column()
        #col.label("Buffer Type:")
        #col.prop(lamp, "ge_shadow_buffer_type", text="", toggle=True)
        col.label("Quality:")
        col = layout.column(align=True)
        col.prop(lamp, "shadow_buffer_size", text="ShadowMapSize")
        # ,text="Size")
        col.prop(lamp, "shadow_buffer_bias", text="Bias")
        #col.prop(lamp, "shadow_buffer_bleed_bias", text="Bleed Bias")

        row = layout.row()
        row.label("Clipping:")
        row = layout.row(align=True)
        row.prop(lamp, "shadow_buffer_clip_start", text="Clip Start")
        row.prop(lamp, "shadow_buffer_clip_end", text="Clip End")

        #if lamp.type == 'SUN':
        #    row = layout.row()
        #    row.prop(lamp, "shadow_frustum_size", text="Frustum Size")


def draw_avango_starter(self, context):
    layout = self.layout
    scene = context.scene

    if scene.render.engine == 'AVANGO':
        layout.operator("start.avango", icon='PLAY')


def get_panels():
    types = bpy.types
    panels = [
        "OBJECT_PT_so",
        "DATA_PT_mesh",
        "DATA_PT_context_mesh",

        "DATA_PT_camera",
        "DATA_PT_context_camera",
        "DATA_PT_camera_display",

        "DATA_PT_context_lamp",

        "MATERIAL_PT_context_material",

        "TEXTURE_PT_preview",
        "TEXTURE_PT_context_texture",
        "TEXTURE_PT_image",
        #"TEXTURE_PT_envmap",
    ]
    return [getattr(types, p) for p in panels if hasattr(types, p)]


def register():
    bpy.types.INFO_MT_render.append(draw_avango_starter)
    bpy.types.INFO_HT_header.append(draw_avango_starter)
    for panel in get_panels():
        panel.COMPAT_ENGINES.add('AVANGO')


def unregister():
    bpy.types.INFO_MT_render.remove(draw_avango_starter)
    bpy.types.INFO_HT_header.remove(draw_avango_starter)
    for panel in get_panels():
        panel.COMPAT_ENGINES.remove('AVANGO')
