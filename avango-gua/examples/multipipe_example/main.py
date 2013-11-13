#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import time
import math

import examples_common.navigator
from examples_common.GuaVE import GuaVE
import weimar_scene
import oilrig_scene


STEREO = False
TRACKING = False
USER_COUNT = 1

class TimedMaterialUniformUpdate(avango.script.Script):

  TimeIn = avango.SFFloat()
  MaterialName = avango.SFString()
  UniformName = avango.SFString()

  @field_has_changed(TimeIn)
  def update(self):
    avango.gua.set_material_uniform(self.MaterialName.value,
                                    self.UniformName.value,
                                    self.TimeIn.value)

class UpdatePortalTransform(avango.script.Script):

  PortalTransformIn = avango.gua.SFMatrix4()
  ViewTransformIn = avango.gua.SFMatrix4()
  ScreenTransformIn = avango.gua.SFMatrix4()
  ViewTransformOut = avango.gua.SFMatrix4()

  def evaluate(self):
    self.ViewTransformOut.value = avango.gua.make_inverse_mat(self.PortalTransformIn.value) * \
                                  self.ScreenTransformIn.value * self.ViewTransformIn.value

def create_pipeline(user):

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  weimar_camera = avango.gua.nodes.Camera(LeftEye = "/screen/head" + str(user) + ("/mono_eye" if not STEREO else "/left_eye") + str(user),
                                          RightEye = "/screen/head" + str(user) + ("/mono_eye" if not STEREO else "/right_eye") + str(user),
                                          LeftScreen = "/screen",
                                          RightScreen = "/screen",
                                          SceneGraph = "WeimarScene")

  pre_pipe = avango.gua.nodes.Pipeline(Camera = weimar_camera,
                                       OutputTextureName =  "weimar" + str(user))


  if STEREO:
    pre_pipe.LeftResolution.value  = avango.gua.Vec2ui(1740/2, 1145/2)
    pre_pipe.RightResolution.value = avango.gua.Vec2ui(1755/2, 1150/2)
  else :
    pre_pipe.LeftResolution.value  = avango.gua.Vec2ui(width/2, height/2)

  pre_pipe.EnableStereo.value = STEREO
  pre_pipe.EnableSsao.value = False
  pre_pipe.SsaoRadius.value = 5.0
  pre_pipe.BackgroundTexture.value = "data/textures/sky.jpg"
  pre_pipe.EnableFog.value = True
  pre_pipe.FogStart.value = 10.0
  pre_pipe.FogEnd.value = 35.0
  pre_pipe.FogTexture.value = "data/textures/sky.jpg"
  pre_pipe.AmbientColor.value = avango.gua.Color(0.4, 0.4, 0.5)



  oilrig_camera = avango.gua.nodes.Camera(LeftEye = "/screen/head" + str(user) + ("/mono_eye" if not STEREO else "/left_eye") + str(user),
                                          RightEye = "/screen/head" + str(user) + ("/mono_eye" if not STEREO else "/right_eye") + str(user),
                                          LeftScreen = "/screen",
                                          RightScreen = "/screen",
                                          SceneGraph = "OilrigScene",
                                          RenderMask = "!multiuser | user" + str(user))

  pipe = avango.gua.nodes.Pipeline(Camera = oilrig_camera,
                                   OutputTextureName =  "oilrig" + str(user))

  if STEREO:
    pipe.LeftResolution.value  = avango.gua.Vec2ui(1740, 1145)
    pipe.RightResolution.value = avango.gua.Vec2ui(1755, 1150)
  else :
    pipe.LeftResolution.value  = size

  pipe.EnablePreviewDisplay.value = False
  pipe.EnableStereo.value = STEREO
  pipe.EnableFXAA.value = True
  pipe.EnableFog.value = True
  pipe.FogStart.value = 80.0
  pipe.FogEnd.value = 300.0
  pipe.FogTexture.value = "data/textures/sky.jpg"
  pipe.BackgroundTexture.value = "data/textures/sky.jpg"
  pipe.AmbientColor.value = avango.gua.Color(0.8, 0.8, 1.0)
  pipe.EnableSsao.value = True
  pipe.SsaoIntensity.value = 2.0


  window =  avango.gua.nodes.Window()

  if STEREO:
    window.Size.value = avango.gua.Vec2ui(1920 * 2, 1200)
    window.Title.value = "multipipe_example"
    window.LeftResolution.value = avango.gua.Vec2ui(1740, 1145)
    window.LeftPosition.value = avango.gua.Vec2ui(180, 10)
    window.RightResolution.value = avango.gua.Vec2ui(1755, 1150)
    window.RightPosition.value = avango.gua.Vec2ui(1930, 17)
    window.StereoMode.value = avango.gua.StereoMode.SIDE_BY_SIDE

    if user == 0:
      window.Display.value = ":0.0"
    elif user == 1:
      window.Display.value = ":0.1"

  else:
    window.Size.value = size
    window.LeftResolution.value = size
    window.EnableVsync.value = True

  pipe.Window.value = window

  pipe.PreRenderPipelines.value = [pre_pipe]


  return pipe



def start():

  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  avango.gua.create_texture("data/textures/sky.jpg")
  avango.gua.create_texture("data/textures/frame.jpg")

  weimar = weimar_scene.create(USER_COUNT)
  oilrig = oilrig_scene.create(USER_COUNT)

  pipes = []
  for i in range(0, USER_COUNT):
    portal = avango.gua.nodes.TexturedQuadNode(Name = "portal" + str(i),
                                               Texture = "weimar" + str(i),
                                               IsStereoTexture = STEREO,
                                               Width = 1.6,
                                               Height = 0.9)

    portal.Transform.value = avango.gua.make_trans_mat(6.0, 6.0, 2.0) * \
                             avango.gua.make_rot_mat(-90.0, 0.0, 1.0, 0.0) * \
                             avango.gua.make_scale_mat(3.0, 3.0, 3.0)

    portal.GroupNames.value = ["user" + str(i), "multiuser"]

    frame = avango.gua.nodes.TexturedQuadNode(Name = "frame",
                                              Texture = "data/textures/frame.jpg",
                                              Width = 1.6,
                                              Height = 0.9)
    frame.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -0.001) * \
                            avango.gua.make_scale_mat(1.05, 1.05, 1.05)

    frame.GroupNames.value = ["user" + str(i), "multiuser"]

    portal.Children.value = [frame]
    oilrig.Root.value.Children.value.append(portal)

    pipes.append(create_pipeline(i))

    portal_updater = UpdatePortalTransform()
    portal_updater.PortalTransformIn.connect_from(portal.Transform)
    portal_updater.ViewTransformIn.connect_from(oilrig["/screen/head" + str(i)].Transform)
    portal_updater.ScreenTransformIn.connect_from(oilrig["/screen"].Transform)
    weimar["/screen/head" + str(i)].Transform.connect_from(portal_updater.ViewTransformOut)


  water_updater = TimedMaterialUniformUpdate()
  water_updater.MaterialName.value = "Water"
  water_updater.UniformName.value = "time"
  timer = avango.nodes.TimeSensor()
  water_updater.TimeIn.connect_from(timer.Time)


  navigator = examples_common.navigator.Navigator()
  nav_node = oilrig["/screen"]
  navigator.StartLocation.value = nav_node.Transform.value.get_translate()
  navigator.OutTransform.connect_from(nav_node.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  nav_node.Transform.connect_from(navigator.OutTransform)

  pipe = pipes[0]

  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = pipes
  viewer.SceneGraphs.value = [weimar, oilrig]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

