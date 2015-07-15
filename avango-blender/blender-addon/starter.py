import bpy
import bpy.types
from . import exporter
import os
import tempfile


class StartAvango(bpy.types.Operator):
    bl_idname = "start.avango"
    bl_label = "Start Avango"
    bl_description =\
        "exports an .avango file, loads it in avango and starts it"

    def invoke(self, context, event):
        print("invoke avango application")

        tmpdir = tempfile.mkdtemp()
        tmpfilepath = tmpdir+'/automatic_export.avango'

        exporter.save(tmpfilepath, context)

        relativ_path =\
            context.user_preferences.\
            addons['avango-blender'].\
            preferences['filepath_loader']

        absolute_path = bpy.path.abspath(relativ_path)

        command = "./start.sh "+tmpfilepath

        os.system(
            "gnome-terminal --hide-menubar --working-directory=" +
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
