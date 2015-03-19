import avango
import avango.script
from avango.script import field_has_changed

class CameraControl(avango.script.Script):


  _mouse_pos = avango.gua.Vec2(0.0,0.0)
  _mouse_scroll = avango.gua.Vec2(0.0,14.0)

  def __init__(self):

    self.super(CameraControl).__init__()

  def my_constructor(self, screen_node, target_node, application_window):

    self._screen = screen_node

    self._target = target_node

    def handle_mouse(m):
      self._mouse_pos += m - self._mouse_pos
    application_window.on_move_cursor(handle_mouse)

    def handle_scroll(s):
      self._mouse_scroll+=s
    application_window.on_scroll(handle_scroll)

    self.always_evaluate(True)
		

  def evaluate(self):

    # camera steering:
    if self._mouse_pos != None and self._mouse_scroll != None:
      target_trans = self._target.WorldTransform.value.get_translate()
      target_scale = self._target.WorldTransform.value.get_scale()
      self._screen.Transform.value = avango.gua.make_trans_mat(target_trans+avango.gua.Vec3(0.0,0.1,0.0)) * \
        avango.gua.make_rot_mat(self._mouse_pos.x/2.0,0.0,1.0,0.0) * \
        avango.gua.make_rot_mat(self._mouse_pos.y/2.0,1.0,0.0,0.0) * \
        avango.gua.make_trans_mat(0,0.003*self._mouse_scroll.y,0.05*self._mouse_scroll.y) * \
        avango.gua.make_scale_mat(target_scale)# * avango.gua.make_rot_mat(180.0,0.0,1.0,0.0)
