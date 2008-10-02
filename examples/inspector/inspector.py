import avango.osg
import avango.inspector
import avango.osg.simpleviewer

root = avango.osg.nodes.Group(Name="Root")
sphere = avango.osg.nodes.Sphere(Name="Sphere")
root.Children.value = [ sphere ]

inspector = avango.inspector.nodes.Inspector(Children=[root])
avango.osg.simpleviewer.run(root)
