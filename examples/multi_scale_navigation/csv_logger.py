import avango.script
from avango.script import field_has_changed

# import 

class CSV_Logger(avango.script.Script):

  Input_Time = avango.SFFloat()
  Input_MinDistance = avango.SFFloat()
  Input_MotionSpeed = avango.SFFloat()
  Input_NearClip = avango.SFFloat()
  Input_FarClip = avango.SFFloat()
  #Input_RenderingFPS
  Input_Window = avango.gua.SFGlfwWindow()

  def __init__(self):
    self.super(CSV_Logger).__init__()

    my_file = open("test.csv", 'w')
    my_file.write("Time,MinDistance,MotionSpeed,NearClip,FarClip,RenderingFPS\n")
    my_file.close()

    self.count = 0;

    self.always_evaluate(True)

  def evaluate(self):
    fps = self.Input_Window.value.RenderingFPS.value
    if not fps == 0.0:
      if (self.count == 10):
        self.count = 0;
        line = (
          str(self.Input_Time.value) + "," + 
          str(self.Input_MinDistance.value) + "," + 
          str(self.Input_MotionSpeed.value) + "," + 
          str(self.Input_NearClip.value) + "," + 
          str(self.Input_FarClip.value) + "," + 
          str(fps) + "\n"
        )
        my_file = open("test.csv", 'a')
        my_file.write(line)
        my_file.close()
      else:
        self.count += 1;