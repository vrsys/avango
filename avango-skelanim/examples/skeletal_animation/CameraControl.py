import avango
import avango.script
from avango.script import field_has_changed

def clamp(minimum, x, maximum):
  return max(minimum, min(x, maximum))

class CameraControl(avango.script.Script):
  _mouse_delta = avango.gua.Vec2(0.0,.0)     # how much mouse was moved since last frame
  _camera_offset = 0.01
  _camera_rotation = avango.gua.Vec2(0.0,180.0)
  _mouse_last_pos = None
  _rotation_speed = 0.5
  _scroll_speed = 1

  def __init__(self):

    self.super(CameraControl).__init__()

  def my_constructor(self, screen_node, target_node, application_window):

    self._screen = screen_node
    self._target = target_node

    def handle_mouse(m): 
      if self._mouse_last_pos != None:
        self._mouse_delta = self._mouse_last_pos - m
      self._mouse_last_pos = m

    application_window.on_move_cursor(handle_mouse)

    def handle_scroll(s):
      self._camera_offset += s.y * self._scroll_speed
    application_window.on_scroll(handle_scroll)

    self.always_evaluate(True)
    

  def evaluate(self):
    # this cant be done in the constructor because the bbox scale is onlz correct after first rendered frame
    target_height = self._target.BoundingBox.value.Max.value.y - self._target.BoundingBox.value.Min.value.y  

    # camera steering:
    target_trans = self._target.WorldTransform.value.get_translate()

    self._camera_rotation.x -= self._rotation_speed * self._mouse_delta.y
    self._camera_rotation.x = clamp(-90, self._camera_rotation.x, 10)
    self._camera_rotation.y -= self._rotation_speed * self._mouse_delta.x

    self._screen.Transform.value = avango.gua.make_trans_mat(target_trans + avango.gua.Vec3(0.0,target_height,0.0)) * \
      avango.gua.make_rot_mat(self._camera_rotation.y, 0.0,1.0,0.0) * \
      avango.gua.make_rot_mat(self._camera_rotation.x, 1.0,0.0,0.0) * \
      avango.gua.make_trans_mat(0,0, self._camera_offset * target_height)

    self._mouse_delta = avango.gua.Vec2(0.0,.0)
