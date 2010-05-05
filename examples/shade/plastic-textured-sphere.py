import avango.osg
import avango.osg.simpleviewer
import avango.shade

light = avango.shade.nodes.DirectionalLight()
light_source = avango.shade.nodes.LightSource(LightNum = 1)
light_source.Position.value = avango.osg.Vec4(0., 1., 1., 0.)
light.direction.connect_from(light_source.Shader)
light.color.value = avango.shade.nodes.Value_vec3_uniform(value = avango.osg.Vec3(1., 1., 1.))

coord_sys = avango.shade.nodes.CameraSpace()

image_loader = avango.osg.nodes.LoadImage(Filename="pattern.dds")
texture = avango.osg.nodes.Texture2D()
texture.Image.connect_from(image_loader.Image)
uvcoord = avango.shade.nodes.UVCoord()
sampler = avango.shade.nodes.Texture2D(texture_unit=texture, uv=uvcoord)

plastic = avango.shade.nodes.Plastic(lights = [light],
                                     coordinate_system = coord_sys,
                                     specular=avango.shade.nodes.Value_float_uniform(value=0.3),
                                     diffuse=avango.shade.nodes.Value_float_uniform(value=0.9),
                                     roughness=avango.shade.nodes.Value_float_uniform(value=512.),
                                     )
plastic.color.value = sampler

surface = avango.shade.nodes.Surface(material=plastic)
state = avango.shade.nodes.State(Shader=surface)

root = avango.shade.nodes.StateGroup(State=state)
sphere = avango.osg.nodes.Sphere(Radius=1.)

root.Children.value = [ sphere, light_source]

avango.osg.simpleviewer.run(root)
