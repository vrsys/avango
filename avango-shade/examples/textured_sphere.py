import avango.osg
import avango.osg.simpleviewer
import avango.shade

image_loader = avango.osg.nodes.LoadImage(Filename="pattern.dds")
texture = avango.osg.nodes.Texture2D()
texture.Image.connect_from(image_loader.Image)
uvcoord = avango.shade.nodes.UVCoord()
sampler = avango.shade.nodes.Texture2D(texture_unit=texture, uv=uvcoord)
constant = avango.shade.nodes.Constant(color=sampler)
surface = avango.shade.nodes.Surface(material=constant)
state = avango.shade.nodes.State(Shader=surface)

root = avango.shade.nodes.StateGroup(State=state)
sphere = avango.osg.nodes.Sphere(Radius=1.)
root.Children.value.append(sphere)

avango.osg.simpleviewer.run(root)
