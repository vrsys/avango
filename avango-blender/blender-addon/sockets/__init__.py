from . import vec3_socket
from . import matrix_socket
from . import stereo_mode_socket
from . import camera_socket
from . import screen_socket
from . import avango_node_socket


def register():
    # register sockets
    vec3_socket.register()
    matrix_socket.register()
    stereo_mode_socket.register()
    camera_socket.register()
    screen_socket.register()
    avango_node_socket.register()


def unregister():
    vec3_socket.unregister()
    matrix_socket.unregister()
    stereo_mode_socket.unregister()
    camera_socket.unregister()
    screen_socket.unregister()
    avango_node_socket.unregister()
