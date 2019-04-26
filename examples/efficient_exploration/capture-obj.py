import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.lod
from examples_common.GuaVE import GuaVE
import examples_common.navigator
from examples_common.GuaVE import GuaVE

import random
import os
import time
import subprocess


class CaptureScript(avango.script.Script):
    def __init__(self):
        self.super(CaptureScript).__init__()
        self.StartCounting = False
        self.frame_count = 0
        self.image_count = 0
        self.angle = 0
        self.path = os.path.dirname(os.path.realpath(__file__))

        self.camera = None
        self.cam_trans = None
        self.center = avango.gua.Vec3(0.0, 0.0, 0.0)
       
    def set_camera(self, camera, cam_trans, center):
        self.always_evaluate(True)
        self.StartCounting = True

        self.camera = camera
        self.cam_trans = cam_trans
        self.center = center


    def capture(self, name):
        
        file_name = self.path + '/' + name + self.frame_count
        self.image_count += 1  

        bash_command = "env DISPLAY=:0.0 import -window ROOT " + file_name + ".png"
        process = subprocess.Popen(bash_command.split(), stdout=subprocess.PIPE)
        output, error = process.communicate()
        print('Captured ', self.image_count)

    def evaluate(self):
        if self.StartCounting == True:
            self.frame_count += 1
            if self.frame_count == 59:

                print('Move')
                distance = 1.5
                height = 1.0
                levels = 3
                height_steps = height / levels
                angle_steps = 30
                steps = 360//angle_steps

                self.camera.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, distance)

                height_level = self.center.y - (height / 2) + height_steps * ((self.image_count//steps) % levels)
                self.cam_trans.Transform.value = avango.gua.make_trans_mat(self.center.x, height_level, self.center.y)
                
                self.angle = (self.angle + angle_steps) % 360
                print(self.angle)
                self.cam_trans.Transform.value *= avango.gua.make_rot_mat(self.angle, 0.0, 1.0, 0.0)

            if self.frame_count == 60:
                print('Take Image')
                self.capture
                self.frame_count = 0


            


def start():

  graph  = avango.gua.nodes.SceneGraph(Name = "scene")
  mesh_loader = avango.gua.nodes.TriMeshLoader()
 
  floor = mesh_loader.create_geometry_from_file(
    "floor",
    "data/objects/plane.obj",
    avango.gua.LoaderFlags.DEFAULTS
  )
  
  floor.Transform.value = avango.gua.make_trans_mat(0, -0.1 , 0) * avango.gua.make_scale_mat(4, 1, 4)
  floor.ShadowMode.value = 1
  graph.Root.value.Children.value.append(floor)

  # load model to take images of
  photo_model = mesh_loader.create_geometry_from_file(
    "photo_model",
    "data/objects/monkey.obj",
    avango.gua.LoaderFlags.DEFAULTS,
  )

  photo_model_center = avango.gua.Vec3(0.0, 0.5, 0.0)
  photo_model.Transform.value = avango.gua.make_trans_mat(photo_model_center) * avango.gua.make_scale_mat(0.5)
  # photo_model.Material.value.set_uniform('Color', avango.gua.Color(0.4, 0.4, 0.2))
  photo_model.Material.value.set_uniform('Emissivity', 1.0)
  graph.Root.value.Children.value.append(photo_model)


  # t = str(quad.transform).replace('\n', '')
  # cam_location_list.append(t)

  # with open('cam_transforms.txt', 'w') as outfile:  
  #     for line in cam_location_list:
  #         outfile.write(line)
  #         outfile.write('\n')

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  camera_transform = avango.gua.nodes.TransformNode(Name='cam_trans')

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       # Width = 4.8,
                                       # Height = 2.7)
                                       Width = 0.48,
                                       Height = 0.27) 

  screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -0.1)

  camera = avango.gua.nodes.CameraNode(
    Name = "cam",
    LeftScreenPath = "/cam_trans/cam/screen",
    RightScreenPath = "/cam_trans/cam/screen",
    SceneGraph = "scene",
    Resolution = size,
    OutputWindowName = "window",
    EyeDistance = 0.2,
    EnableStereo = False,
    Children = [screen],
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 0.0)
  )

  res_pass = avango.gua.nodes.ResolvePassDescription()
  res_pass.EnableSSAO.value = False
  res_pass.SSAOIntensity.value = 3.0
  res_pass.SSAOFalloff.value = 20.0
  res_pass.SSAORadius.value = 10.0
  res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1,0.1,0.1)
  res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
  res_pass.Exposure.value = 1.0
  res_pass.BackgroundMode.value = avango.gua.BackgroundMode.CUBEMAP_TEXTURE
  res_pass.BackgroundTexture.value = "awesome_skymap"
  # res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)
  res_pass.VignetteColor.value = avango.gua.Vec4(0, 0, 0, 1)

  pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes = [
      avango.gua.nodes.TriMeshPassDescription(),
      avango.gua.nodes.SkyMapPassDescription(
        OutputTextureName="awesome_skymap"
      ),
      avango.gua.nodes.LightVisibilityPassDescription(),
      res_pass,
    ],
    EnableABuffer = False
  )

  camera.PipelineDescription.value = pipeline_description

  graph.Root.value.Children.value.append(camera_transform)
  camera_transform.Children.value.append(camera)

  window = avango.gua.nodes.Window(
    Size = size,
    Title = "shadows",
    LeftResolution = size,
    RightResolution = size,
    EnableVsync = False,
    StereoMode = avango.gua.StereoMode.MONO
  )

  avango.gua.register_window("window", window)

  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = camera.Transform.value.get_translate()
  navigator.OutTransform.connect_from(camera.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  # camera_transform.Transform.connect_from(navigator.OutTransform)

  capture_script = CaptureScript()
  capture_script.set_camera(camera, camera_transform, photo_model_center)

  viewer = avango.gua.nodes.Viewer()
  # viewer.DesiredFPS.value = 200
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]

  timer = avango.nodes.TimeSensor()

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

'''
def main():

path = os.path.dirname(os.path.realpath(file))
file_name = path + '/test_screen_shot'

for i in range(5):
bash_command = "env DISPLAY=:0.0 import -window ROOT " + file_name + str(i) + ".png"
process = subprocess.Popen(bash_command.split(), stdout=subprocess.PIPE)
output, error = process.communicate()
print(i, error)
time.sleep(1)


if name == 'main':
main()

'''