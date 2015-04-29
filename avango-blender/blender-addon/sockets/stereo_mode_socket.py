import bpy
from bpy.types import NodeSocket


class StereoModeSocket(NodeSocket):
    '''Custom node socket type'''
    bl_idname = 'StereoModeSocketType'
    bl_label = 'Stereo Mode Socket'

    # Enum items list
    modes = [
        ("MONO", "Mono", "mono"),
        ("SIDE_BY_SIDE", "SideBySide", "side by side stereo"),
        ("ANAGLYPH_RED_GREEN", "Anaglyph", "anaglyph stereo"),
        ("ANAGLYPH_RED_CYAN", "Anaglyph", "anaglyph stereo"),
        ("CHECKERBOARD", "Checkerboard", "checkerboard for 3D-TVs")
    ]

    stereo_mode = bpy.props.EnumProperty(
        name="StereoMode",
        description="stereo modes",
        items=modes,
        default='MONO',
        )

    # Optional function for drawing the socket input value
    def draw(self, context, layout, node, text):
        if self.is_output or self.is_linked:
            layout.label(text)
        else:
            layout.prop(self, "stereo_mode", text=text)

    # Socket color
    def draw_color(self, context, node):
        return (1.0, 0.4, 0.216, 0.5)


def register():
    bpy.utils.register_class(StereoModeSocket)


def unregister():
    bpy.utils.unregister_class(StereoModeSocket)
