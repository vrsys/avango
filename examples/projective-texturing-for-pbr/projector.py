import avango
import avango.script
import avango.gua
from avango.script import field_has_changed

import xml.etree.ElementTree as ET


class XML_Projector(avango.script.Script):
    Texture = avango.SFString()
    Material = avango.gua.SFMaterial()
    Image_View_Mat = avango.gua.SFMatrix4()
    Enable = avango.SFBool()

    def __init__(self):
        self.super(XML_Projector).__init__()
        loader = avango.gua.nodes.TriMeshLoader()

        # setup node and frame geometry
        self.group_node = avango.gua.nodes.TransformNode(
            Name="projector_group"
        )
        self.geometry = loader.create_geometry_from_file(
            "frame",
            "data/objects/frame.obj",
            avango.gua.LoaderFlags.DEFAULTS,
        )
        self.geometry.Material.value.set_uniform(
            "Color",
            avango.gua.Vec4(0.7, 0.7, 0.7, 1.0),
        )
        self.geometry.Material.value.set_uniform("Emissivity", 1.0)
        self.group_node.Children.value.append(self.geometry)

        # setup Material
        self.Material.value = avango.gua.nodes.Material(
            ShaderName="proj_mat"
        )
        proj_mat_desc = avango.gua.nodes.MaterialShaderDescription()
        proj_mat_desc.load_from_file(
            "data/materials/Projective_Texture_pbr.gmd"
        )
        avango.gua.register_material_shader(proj_mat_desc, "proj_mat")
        self.Material.value.set_uniform("Emissivity", 1.0)

    def load_XML(self, XML_path, parent):
        xml = ET.parse(XML_path).getroot()

        # load Transformation from XML
        self.group_node.Transform.value = self.load_transform(xml)
        parent.Children.value.append(self.group_node)

        # set Matrix
        self.Image_View_Mat.value =\
            avango.gua.make_inverse_mat(self.group_node.Transform.value)
        self.Material.value.set_uniform(
            "Image_View_Mat",
            self.Image_View_Mat.value,
        )

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

    @field_has_changed(Enable)
    def update_enable_uniform(self):
        self.Material.value.set_uniform("EnableProjection", self.Enable.value)

    @field_has_changed(Texture)
    def update_texture(self):
        self.Material.value.set_uniform(
            "projective_texture",
            self.Texture.value,
        )


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
