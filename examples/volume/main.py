#!/usr/bin/python3

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.volume
from examples_common.GuaVE import GuaVE
import sys

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  def evaluate(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(
      self.TimeIn.value * 10.0, 0.0, 1.0, 0.0)


def start():
  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
  loader = avango.gua.nodes.TriMeshLoader()

  volumeloader = avango.gua.nodes.VolumeLoader()
  volume_geode = volumeloader.load("volume", "/opt/gua_vrgeo_2013/data/objects/head_w256_h256_d225_c1_b8.raw", 0)

  transform = avango.gua.nodes.TransformNode(
    Children=[volume_geode]
  )

  size = avango.gua.Vec2ui(1024, 768)

  window = avango.gua.nodes.GlfwWindow(
    Size=size,
    LeftResolution=size
  )

  avango.gua.register_window("window", window)

  cam = avango.gua.nodes.CameraNode(
    LeftScreenPath="/screen",
    SceneGraph="scenegraph",
    Resolution=size,
    OutputWindowName="window",
    Transform=avango.gua.make_trans_mat(0.0, 0.0, 1.5)
  )

  cam.PipelineDescription.value.Passes.value.append(avango.gua.nodes.VolumePassDescription())

  screen = avango.gua.nodes.ScreenNode(
    Name="screen",
    Width=2,
    Height=1.5,
    Children=[cam],
    Transform=avango.gua.make_trans_mat(0.0, 0.0, 1.5)
  )

  graph.Root.value.Children.value = [transform, screen]

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]

  updater = TimedRotate()

  timer = avango.nodes.TimeSensor()
  updater.TimeIn.connect_from(timer.Time)

  transform.Transform.connect_from(updater.MatrixOut)

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

