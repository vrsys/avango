bl_info = {
    "name": "Avango",
    "author": "Avango Development Team",
    "version": (14, 11, 0),
    "blender": (2, 72, 0),
    "location": "File > Import-Export",
    "description": "Avango is a 3D framework",
    "warning": "",
    "wiki_url": "github.com/vrsys/avango-blender",
    "category": "Import-Export"
}

if "bpy" in locals():
    import importlib
    if "preferences" in locals():
        importlib.reload(preferences)
    if "node_tree" in locals():
        importlib.reload(node_tree)
    if "export_avango" in locals():
        importlib.reload(export_avango)
    if "sockets" in locals():
        importlib.reload(sockets)
    if "engine" in locals():
        importlib.reload(engine)

import bpy
import nodeitems_utils as nu
from . import (
    preferences,
    node_tree,
    export_avango,
    starter, nodes,
    sockets,
    engine,
    )

node_categories = [
    node_tree.AvangoNodeCategory("SOMENODES", "scene",
                                 items=[
                                     nu.NodeItem("Window"),
                                     nu.NodeItem("Camera"),
                                     nu.NodeItem("Light"),
                                     nu.NodeItem("Screen"),
                                     # nu.NodeItem("Transform"),
                                     nu.NodeItem("Mesh"),
                                     nu.NodeItem("FromObject"),
                                 ]),
    node_tree.AvangoNodeCategory(
        "UTILNODES", "utils",
        items=[
            nu.NodeItem("FloatInputNode"), nu.NodeItem("FloatMath"),
            nu.NodeItem("TranslationMatrix"), nu.NodeItem(
                "RotationMatrix"), nu.NodeItem("Vec3"), nu.NodeItem("Script")
        ]),
    node_tree.AvangoNodeCategory("CORENODES", "core",
                                 items=[
                                     nu.NodeItem("TimeSensor"),
                                     nu.NodeItem("Keyboard"),
                                 ])
]


def register():
    from . import ui
    from . import properties
    preferences.register()
    properties.register()
    ui.register()
    node_tree.register()
    sockets.register()
    export_avango.register()
    starter.register()
    nodes.register()

    bpy.utils.register_module(__name__)

    # Idee: root am SceneGraph
    # 1.)
    # alle objects haben CollectionProperty,
    # sollte beim exportieren des scene graphs ein die CollectionProperty eines
    # Hierarchieknotens leer sein, wird ein default FieldContainer des
    # passenden typs erstellt
    # 2.) wenn ein Node gelöscht wird, soll auf referenzierten blender objecten
    #   vorher die Referenz auf diesen Node entfernt werden

    #bpy.types.Object.avango_nodes = bpy.props.CollectionProperty(
    #  type=bpy.types.PropertyGroup)

    if 'AVANGO_NODES' in nu._node_categories:
        nu.unregister_node_categories('AVANGO_NODES')
    nu.register_node_categories('AVANGO_NODES', node_categories)


def unregister():
    from . import ui
    from . import properties
    node_tree.unregister()
    export_avango.unregister()
    starter.unregister()
    if 'AVANGO_NODES' in nu._node_categories:
        nu.unregister_node_categories('AVANGO_NODES')
    preferences.unregister()
    properties.unregister()
    ui.unregister()
    bpy.utils.unregister_module(__name__)


if __name__ == "__main__":
    register()
