import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

class MultiViewExplorer(avango.script.Script):
	def __init__(self):
	    self.super(MultiViewExplorer).__init__()
	    self.always_evaluate(False)
	    self.is_initialized = False

	def my_constructor(self):
	    self.indicator = None
	    loader = avango.gua.nodes.TriMeshLoader()

	    self.localized_image_list = []
	    self.position_list = []
	    self.projection_lense = None
	    self.lense_parent_node = None
	    self.show_lense = False
	    self.min_tex_coords = avango.gua.Vec2(0.0, 0.0)
	    self.max_tex_coords = avango.gua.Vec2(1.0, 1.0)
	    self.last_lense_pos = None
	    self.scene_graph = None
	    self.old_closest_id = 0
	    self.button0_pressed = False
	    # self.offset = avango.gua.Vec3(0.0,0.0,0.0)
	    self.offset = avango.gua.make_identity_mat()
	    
	    self.group_node = avango.gua.nodes.TransformNode(Name = "projector_group")
	    self.group_node.Transform.connect_from(self.Transform)

	    self.geometry = loader.create_geometry_from_file("projector_geometry", "data/objects/projector.obj", 
	      avango.gua.LoaderFlags.NORMALIZE_SCALE |
	      avango.gua.LoaderFlags.NORMALIZE_POSITION | 
	      avango.gua.LoaderFlags.LOAD_MATERIALS)
	    self.geometry.Transform.value = avango.gua.make_scale_mat(0.1)
	    # self.group_node.Children.value.append(self.geometry)

	    self.screen = avango.gua.nodes.ScreenNode(
	      Name = "screen1",
	      Width = 0.5,
	      Height = 0.5,
	      Transform = avango.gua.make_trans_mat(0.0, 0.0, -0.1)
	    )
	    self.group_node.Children.value.append(self.screen)

	    self.cam = avango.gua.nodes.CameraNode(
	      LeftScreenPath = "projector_group/screen1",
	      # LeftScreenPath = "scene_trans/projector_group/screen1",
	      SceneGraph = "scenegraph",
	    )
	    self.group_node.Children.value.append(self.cam)

	    self.Material.value = avango.gua.nodes.Material(
	      ShaderName = "proj_mat"
	    )

	    self.Graph.value = avango.gua.nodes.SceneGraph(Name = "dummy")

	    proj_mat_desc = avango.gua.nodes.MaterialShaderDescription()
	    proj_mat_desc.load_from_file("data/materials/Projective_VT_Material.gmd")

	    # proj_mat_desc.EnableVirtualTexturing.value = True
	    avango.gua.register_material_shader(proj_mat_desc, "proj_mat")
