import avango.osg
import avango.inspector
import avango.osg.simpleviewer

root = avango.osg.nodes.Group(Name="Root")
sphere1 = avango.osg.nodes.Sphere(Name="Red Sphere", Color=avango.osg.Vec4(1., 0., 0., 1))
sphere1.Matrix.value = avango.osg.make_trans_mat(1, 0, 0)
sphere2 = avango.osg.nodes.Sphere(Name="Green Sphere", Color=avango.osg.Vec4(0., 1., 0., 1))
sphere2.Matrix.value = avango.osg.make_trans_mat(-1, 0, 0)
root.Children.value = [ sphere1, sphere2 ]

inspector = avango.inspector.nodes.Inspector(Children=[root])
avango.osg.simpleviewer.run(root)
