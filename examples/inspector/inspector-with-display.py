import avango.osg
import avango.display
import sys

print "NOTE: Start application with '--inspector' option"

argv = avango.display.init(sys.argv)

root = avango.osg.nodes.Group(Name="Root")
sphere1 = avango.osg.nodes.Sphere(Name="RedSphere", Color=avango.osg.Vec4(1., 0., 0., 1))
sphere1.Matrix.value = avango.osg.make_trans_mat(1, 1.7, -5)
sphere2 = avango.osg.nodes.Sphere(Name="GreenSphere", Color=avango.osg.Vec4(0., 1., 0., 1))
sphere2.Matrix.value = avango.osg.make_trans_mat(-1, 1.7, -5)
root.Children.value = [ sphere1, sphere2 ]

view = avango.display.make_view()
view.Root.value = root

avango.display.run()
