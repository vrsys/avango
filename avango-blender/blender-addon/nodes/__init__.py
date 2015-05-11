from . import vec3
from . import rotation_matrix
from . import time_sensor
from . import keyboard
from . import float_node
from . import float_math
from . import from_object
from . import window
from . import screen
from . import light
from . import script_node
from . import translation_matrix


def register():
    vec3.register()
    rotation_matrix.register()
    time_sensor.register()
    keyboard.register()
    float_node.register()
    float_math.register()
    from_object.register()
    window.register()
    screen.register()
    light.register()
    script_node.register()
    translation_matrix.register()
    # transform.register()


def unregister():
    vec3.unregister()
    rotation_matrix.unregister()
    time_sensor.unregister()
    keyboard.unregister()
    float_node.unregister()
    float_math.unregister()
    from_object.unregister()
    window.unregister()
    screen.unregister()
    light.unregister()
    script_node.unregister()
    translation_matrix.unregister()
    # transform.unregister()
