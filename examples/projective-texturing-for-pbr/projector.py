import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import xml.etree.ElementTree as ET
from decimal import Decimal

class XML_Projector(avango.script.Script):

    Texture = avango.SFString()
    Material = avango.gua.SFMaterial()
    Image_View_Mat = avango.gua.SFMatrix4()

    def __init__(self):
        self.super(XML_Projector).__init__()
    
    def load_XML(self, XML_path, parent):
        loader = avango.gua.nodes.TriMeshLoader()
        xml = ET.parse(XML_path).getroot()

        self.group_node = avango.gua.nodes.TransformNode(Name = "projector_group")
        parent.Children.value.append(self.group_node)
        self.group_node.Transform.value = self.load_transform(xml)
        self.geometry = loader.create_geometry_from_file(
            "frame",
            "data/objects/frame.obj",
            avango.gua.LoaderFlags.DEFAULTS,
        )
        self.geometry.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 1.0, 1.0, 1.0))
        self.geometry.Material.value.set_uniform("Emissivity", 0.3)
        self.group_node.Children.value.append(self.geometry)

        self.Material.value = avango.gua.nodes.Material(
          ShaderName = "proj_mat"
        )
        proj_mat_desc = avango.gua.nodes.MaterialShaderDescription()
        proj_mat_desc.load_from_file("data/materials/Projective_Texture_pbr.gmd")
        avango.gua.register_material_shader(proj_mat_desc, "proj_mat")
        self.Image_View_Mat.value = avango.gua.make_inverse_mat(self.group_node.Transform.value)
        self.Material.value.set_uniform("Image_View_Mat", self.Image_View_Mat.value)

    def load_transform(self, xml):
        trans = xml.find("Transformation")
        position = parse_vec3(trans.find("Position").text, faktor=0.001)
        rot_x = parse_vec3(trans.find("Direction").find("X").text)
        rot_y = parse_vec3(trans.find("Direction").find("Y").text)
        rot_z = parse_vec3(trans.find("Direction").find("Z").text)
        
        pxl = xml.find("Image").find("Pixel")
        pxl_size = float(pxl.find("Size").text)

        pxl_x = float(pxl.find("Width").text)
        pxl_y = float(pxl.find("Height").text)
        
        scale_x = pxl_x * pxl_size
        scale_y = pxl_y * pxl_size

        depth = float(xml.find("Depth").find("Range").text)

        transform =\
            build_matrix(position, rot_x, rot_y, rot_z) *\
            avango.gua.make_scale_mat(scale_x, scale_y, depth)
            # avango.gua.make_scale_mat(0.1)

        return transform

    @field_has_changed(Texture)
    def update_texture(self):
        self.Material.value.set_uniform("projective_texture", self.Texture.value)


def parse_vec3(text, faktor=1.0):
    text = text[1:-1]
    texts = text.split('/')
    floats = list(map(float, texts))
    return avango.gua.Vec3(floats[0], floats[1], floats[2]) * faktor


def build_matrix(p, x, y, z):
    lst = [
        x[0], x[1], x[2], 0.0,
        y[0], y[1], y[2], 0.0,
        z[0], z[1], z[2], 0.0,
        p[0], p[1], p[2], 1.0,
    ]
    return avango.gua.from_list(lst)


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

    self.geometry = loader.create_geometry_from_file(
        "projector_geometry",
        "data/objects/projector.obj",
        avango.gua.LoaderFlags.NORMALIZE_SCALE |
            avango.gua.LoaderFlags.NORMALIZE_POSITION |
            avango.gua.LoaderFlags.LOAD_MATERIALS,
    )
    self.geometry.Transform.value = avango.gua.make_scale_mat(0.03)
    self.group_node.Children.value.append(self.geometry)

    self.screen = avango.gua.nodes.ScreenNode(
      Name = "screen",
      Width = 0.5,
      Height = 0.5,
      Transform = avango.gua.make_trans_mat(0.0, 0.0, -0.1)
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
    proj_mat_desc.load_from_file("data/materials/Projective_Texture_pbr.gmd")
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


  @field_has_changed(Texture)
  def update_texture(self):
    self.Material.value.set_uniform("projective_texture", self.Texture.value)



