import avango
import avango.gua
from avango.script import field_has_changed
import time

class Animation(avango.script.Script):
  Duration = avango.SFFloat()

  EndPos   = avango.gua.SFVec3()
  EndRot   = avango.gua.SFQuat()

  StartPos = avango.gua.SFVec3()
  StartRot = avango.gua.SFQuat()

  CurrentMatrix = avango.gua.SFMatrix4()

  def __init__(self):
    self.super(Animation).__init__()
    self.always_evaluate(True)
    self._start = int(round(time.time() * 1000))

  @field_has_changed(EndPos)
  def restart(self):
    self._start = int(round(time.time() * 1000))

  def evaluate(self):
    _now = int(round(time.time() * 1000))
    if (_now - self._start < self.Duration.value):

      if (_now - self._start < 0.5 * self.Duration.value):
        tmp = (_now - self._start) / (0.5*self.Duration.value)
        tmp = tmp*tmp
        _now = tmp*0.5*self.Duration.value + self._start
      else:
        tmp = (_now - self._start) / (0.5*self.Duration.value) - 2.0
        tmp = -tmp*tmp
        _now = (tmp + 2.0)*0.5*self.Duration.value + self._start

      _pos = self.StartPos.value.lerp_to(self.EndPos.value, (_now-self._start)/self.Duration.value)
      _rot = self.StartRot.value.slerp_to(self.EndRot.value, (_now-self._start)/self.Duration.value)
      self.CurrentMatrix.value = avango.gua.make_trans_mat(_pos) * avango.gua.make_rot_mat(_rot.get_angle(), _rot.get_axis())
