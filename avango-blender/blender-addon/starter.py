import bpy
import bpy.types
from . import exporter
import os


class StartAvango(bpy.types.Operator):
    bl_idname = "start.avango"
    bl_label = "Start Avango"

    def invoke(self, context, event):
        print("invoke avango application")
        exporter.save('/tmp/automatic_export.avango', context)

        relativ_path =\
            context.user_preferences.\
            addons['avango-blender'].\
            preferences['filepath_loader']

        absolute_path = bpy.path.abspath(relativ_path)

        command = "./start.sh /tmp/automatic_export.avango"

        os.system(
            "gnome-terminal --working-directory=" +
            absolute_path +
            " --command=\"" +
            command +
            "\" "
        )

        return {'FINISHED'}

    def draw(self, context):
        pass


def register():
    bpy.utils.register_class(StartAvango)


def unregister():
    bpy.utils.unregister_class(StartAvango)
