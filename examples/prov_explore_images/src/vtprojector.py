
import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

class Projector(avango.script.Script):

  Material = avango.gua.SFMaterial()
  Transform = avango.gua.SFMatrix4()
  Graph = avango.gua.SFSceneGraph()

  Texture = avango.SFString()


  def __init__(self):
    self.super(Projector).__init__()
    loader = avango.gua.nodes.TriMeshLoader()

    self.group_node = avango.gua.nodes.TransformNode(Name = "projector_group")
    self.group_node.Transform.connect_from(self.Transform)

    self.geometry = loader.create_geometry_from_file("projector_geometry", "data/objects/projector.obj", avango.gua.LoaderFlags.NORMALIZE_SCALE |
                                                                                                         avango.gua.LoaderFlags.NORMALIZE_POSITION | 
                                                                                                         avango.gua.LoaderFlags.LOAD_MATERIALS  )
    self.geometry.Transform.value = avango.gua.make_scale_mat(0.1)
    self.group_node.Children.value.append(self.geometry)

    self.screen = avango.gua.nodes.ScreenNode(
      Name = "screen",
      Width = 0.4,
      Height = 0.4,
      Transform = avango.gua.make_trans_mat(0.0, 0.0, -1.0)
    )
    self.group_node.Children.value.append(self.screen)

    self.cam = avango.gua.nodes.CameraNode(
      LeftScreenPath = "projector_group/screen",
      SceneGraph = "scenegraph",
    )
    self.group_node.Children.value.append(self.cam)

    self.Material.value = avango.gua.nodes.Material(
      ShaderName = "proj_mat"
    )

    self.Graph.value = avango.gua.nodes.SceneGraph(Name = "dummy")

    proj_mat_desc = avango.gua.nodes.MaterialShaderDescription()
    proj_mat_desc.load_from_file("data/materials/Projective_Texture_Material.gmd")
    # proj_mat_desc.EnableVirtualTexturing.value = True
    avango.gua.register_material_shader(proj_mat_desc, "proj_mat")


  def set_scenegraph(self, graph):
    self.graph = graph

  @field_has_changed(Transform)
  def update_matrices(self):
    frustum = self.cam.get_frustum(self.Graph.value, avango.gua.CameraMode.CENTER)

    projection_matrix = frustum.ProjectionMatrix.value
    view_matrix = frustum.ViewMatrix.value
    
    self.Material.value.set_uniform("projective_texture_matrix", projection_matrix)
    self.Material.value.set_uniform("view_texture_matrix", view_matrix ) 
    self.Material.value.set_uniform("view_port_min", avango.gua.Vec2(0.25, 0.25))
    self.Material.value.set_uniform("view_port_max", avango.gua.Vec2(0.3, 0.3))
    

  @field_has_changed(Texture)
  def update_texture(self):
    self.Material.value.set_uniform("projective_texture", self.Texture.value)
    self.Material.value.EnableVirtualTexturing.value = True

