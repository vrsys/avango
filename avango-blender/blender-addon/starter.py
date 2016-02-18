import bpy
import bpy.types
from . import export_avango
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

        bpy.ops.export_scene.avango(
            filepath=tmpfilepath)

        relativ_path =\
            context.user_preferences.\
            addons['avango-blender'].\
            preferences['filepath_loader']

        absolute_path = bpy.path.abspath(relativ_path)

        command = "./start.sh "+tmpfilepath

        #cmd = "gnome-terminal --hide-menubar --working-directory=" + absolute_path + " --command=\"" + command + "\" "
        #cmd = "cd "+tmpdir+"; " + absolute_path + "start.sh " + tmpfilepath
        cmd = "python3.4 " + absolute_path + "main.py " + tmpfilepath
        print(cmd)
        try:
            os.system(cmd)
        except:
            self.report({'ERROR'},
                "avango starter not found, "
                "please fix starter.py")

        return {'FINISHED'}

    def draw(self, context):
        pass


def register():
    bpy.utils.register_class(StartAvango)


def unregister():
    bpy.utils.unregister_class(StartAvango)
