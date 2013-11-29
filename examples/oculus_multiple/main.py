#!/usr/bin/python

import avango
import avango.gua
import avango.script
from avango.script import field_has_changed
import avango.oculus

from OculusDualViewer import OculusDualViewer

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()
  RotationSpeed = avango.SFFloat()

  def __init__(self):
    self.super(TimedRotate).__init__()
    self.RotationSpeed.value = 2.0

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*self.RotationSpeed.value,
                                                   0.0, 1.0, 0.0)

def start():
  # initialize oculus rift
  avango.oculus.initOVR()  

  # initialize materials
  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  # create loader for geometries
  loader = avango.gua.nodes.GeometryLoader()

  # create scenegraph
  graph = avango.gua.nodes.SceneGraph(
    Name = 'scenegraph'
  )

  # create lamp
  lamp_node = avango.gua.nodes.SpotLightNode(
    Name = 'lamp',
    EnableShadows = True,
    EnableGodrays = True,
    Transform = avango.gua.make_trans_mat(0, 3, 0) * \
                avango.gua.make_rot_mat(-90, 1, 0, 0) * \
                avango.gua.make_scale_mat(55, 55, 30)
  )
  graph.Root.value.Children.value.append(lamp_node)

  # create floor
  plane_node = loader.create_geometry_from_file('floor', 'data/objects/plane.obj', 'Tiles', avango.gua.LoaderFlags.DEFAULTS)
  plane_node.Transform.value = avango.gua.make_trans_mat(0, -2, 0) * avango.gua.make_scale_mat(10, 1, 10)
  graph.Root.value.Children.value.append(plane_node)

  # create teapot group
  tea_move_node = avango.gua.nodes.TransformNode(
    Name = 'tea_move',
    Transform = avango.gua.make_trans_mat(0, -1.85, 0)
  )
  tea_rot_node = avango.gua.nodes.TransformNode(
    Name = 'tea_rot'
  )
  graph.Root.value.Children.value.append(tea_move_node)
  tea_move_node.Children.value.append(tea_rot_node)

  pot1 = loader.create_geometry_from_file('pot1', 'data/objects/teapot.obj', 'Shiny', avango.gua.LoaderFlags.DEFAULTS)
  pot2 = loader.create_geometry_from_file('pot2', 'data/objects/teapot.obj', 'Shiny', avango.gua.LoaderFlags.DEFAULTS)
  pot3 = loader.create_geometry_from_file('pot3', 'data/objects/teapot.obj', 'Shiny', avango.gua.LoaderFlags.DEFAULTS)

  tea_rot_node.Children.value = [pot1, pot2, pot3]

  pot1.Transform.value = avango.gua.make_trans_mat(2,  0, 0) * avango.gua.make_scale_mat(0.2, 0.2, 0.2)
  pot2.Transform.value = avango.gua.make_trans_mat(0,  0, 2) * avango.gua.make_scale_mat(0.2, 0.2, 0.2)
  pot3.Transform.value = avango.gua.make_trans_mat(-2,  0, 0) * avango.gua.make_scale_mat(0.2, 0.2, 0.2)

  timed_rotate = TimedRotate(
    RotationSpeed = 50.0
  )
  timer = avango.nodes.TimeSensor()
  timed_rotate.TimeIn.connect_from(timer.Time)

  tea_rot_node.Transform.connect_from(timed_rotate.MatrixOut)

  # Create player avatars
  avatar_transform = avango.gua.nodes.TransformNode()
  avatar_transform.Transform.value = avango.gua.make_identity_mat()

  p1_transform = avango.gua.nodes.TransformNode()
  p2_transform = avango.gua.nodes.TransformNode()

  avatar1 = loader.create_geometry_from_file("monkey", "data/objects/monkey.obj", "Stones", avango.gua.LoaderFlags.DEFAULTS)
  avatar1.Transform.value = avango.gua.make_trans_mat(0, 0, 0.7) * avango.gua.make_rot_mat(180, 0, 1, 0)

  avatar2 = loader.create_geometry_from_file("monkey", "data/objects/monkey.obj", "Stones", avango.gua.LoaderFlags.DEFAULTS)
  avatar2.Transform.value = avango.gua.make_trans_mat(0, 0, 0.7) * avango.gua.make_rot_mat(180, 0, 1, 0)
  
  avatar_transform.Children.value = [p1_transform, p2_transform]
  p1_transform.Children.value = [avatar1]
  p2_transform.Children.value = [avatar2]
  graph.Root.value.Children.value.append(avatar_transform)

  # create and launch viewer
  viewer = OculusDualViewer()
  viewer.SceneGraph.value = graph
  p1_transform.Transform.connect_from(viewer.Player1Position)
  p2_transform.Transform.connect_from(viewer.Player2Position)
  viewer.run(locals(), globals())

if __name__ == '__main__':
  start()

