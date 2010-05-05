import avango.osg
import avango.osg.simpleviewer
import avango.shade

constant = avango.shade.nodes.Constant()
constant.color.value = avango.shade.nodes.Value_vec4_uniform(value = avango.osg.Vec4(1., 0., 0., 1.))
surface = avango.shade.nodes.Surface(material=constant)
state = avango.shade.nodes.State(Shader=surface)

root = avango.shade.nodes.StateGroup(State=state)
sphere = avango.osg.nodes.Sphere(Radius=1.)
root.Children.value.append(sphere)

avango.osg.simpleviewer.run(root)
