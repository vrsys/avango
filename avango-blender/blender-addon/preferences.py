import bpy
from bpy.types import Operator, AddonPreferences
from bpy.props import StringProperty


class AvangoPreferences(AddonPreferences):
    bl_idname = __package__

    # filepath = StringProperty(
    #     name="Avango File Path",
    #     subtype='FILE_PATH',
    #     )
    filepath_loader = StringProperty(
        name="Avango-Json-Loader Directory Path",
        subtype='FILE_PATH',
        )

    def draw(self, context):
        layout = self.layout
        layout.label(text="Set the necessary fielpaths")
        # layout.prop(self, "filepath")
        layout.prop(self, "filepath_loader")


class OBJECT_OT_addon_prefs_avango_blender(Operator):
    """Display avango-blender preferences"""
    bl_idname = "object.addon_prefs_avango_blender"
    bl_label = "Addon Preferences avango-blender"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        user_preferences = context.user_preferences
        addon_prefs = user_preferences.addons[__name__].preferences

        info = ("Path: %s" %
                (addon_prefs.filepath))

        self.report({'INFO'}, info)
        print(info)

        return {'FINISHED'}


# Registration
def register():
    bpy.utils.register_class(OBJECT_OT_addon_prefs_avango_blender)
    bpy.utils.register_class(AvangoPreferences)


def unregister():
    bpy.utils.unregister_class(OBJECT_OT_addon_prefs_avango_blender)
    bpy.utils.unregister_class(AvangoPreferences)
