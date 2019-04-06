import avango
import avango.script
import avango.gua
from avango.script import field_has_changed


class PhotoProjectionQuad(avango.script.Script):

	def __init__(self):
	    self.super(PhotoProjectionQuad).__init__()
	    self.always_evaluate(False)
	    self.is_initialized = False

	def my_constructor(self, parent_node, width, height):
		self.parent_node = parent_node
		self.width = width
		self.height = height

	

		dynamic_triangle_node = dt_loader.create_empty_geometry(
            "dynamic_triangle_node", 
            "empty_name_1.lob", 
            avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.MAKE_PICKABLE)

	    dynamic_triangle_node.Material.value = vt_mat
	    dynamic_triangle_node.Material.value.set_uniform("Metalness", 0.0)
	    dynamic_triangle_node.Material.value.set_uniform("Emissivity", 1.0)
	    dynamic_triangle_node.Material.value.set_uniform("Roughness", 1.0)
	    dynamic_triangle_node.Material.value.set_uniform("vt_images", atlas_path)
	    dynamic_triangle_node.Material.value.EnableVirtualTexturing.value = True
	    dynamic_triangle_node.Material.value.EnableBackfaceCulling.value = False

