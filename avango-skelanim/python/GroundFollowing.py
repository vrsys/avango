import avango
import avango.script
import math

class GroundFollowing(avango.script.Script):
  
  InTransform = avango.gua.SFMatrix4()
  OutTransform = avango.gua.SFMatrix4()
  OutTransform.value = avango.gua.make_identity_mat()

  OffsetToGround = avango.SFFloat()
  MaxDistanceToGround = avango.SFFloat()
  SceneGraph = avango.gua.nodes.SceneGraph()

  DistanceToGround = avango.SFFloat()

  FPS = 60.0


  def __init__(self):
    self.super(GroundFollowing).__init__()

    self.OffsetToGround.value = 0.0
    self.MaxDistanceToGround.value = 1000.0
    self.__ray = avango.gua.nodes.RayNode()

    self._velocity_y = 0.0
    self._gravity = -9.81

    self._timer = avango.nodes.TimeSensor()
    self._last_time = self._timer.Time.value

    self._init_matrix_out = avango.gua.make_identity_mat()

    self.always_evaluate(True)

  def my_constructor(self, gravity = -9.81, init_matrix_out = avango.gua.make_identity_mat()):
    
    self._gravity = gravity
    self.OutTransform.value = init_matrix_out
    self._init_matrix_out = init_matrix_out

  def reset_transform(self):
    self.OutTransform.value = self._init_matrix_out

  def evaluate(self):

    in_translation = self.InTransform.value.get_translate()
    in_translation.y += self.OffsetToGround.value

    self.__ray.Transform.value = avango.gua.make_trans_mat(in_translation) *\
                                 avango.gua.make_rot_mat(-90, 1, 0, 0) *\
                                 avango.gua.make_scale_mat(1.0, 1.0, self.MaxDistanceToGround.value)

    results = self.SceneGraph.value.ray_test(
                                     self.__ray,
                                     avango.gua.PickingOptions.PICK_ONLY_FIRST_OBJECT |
                                     avango.gua.PickingOptions.GET_WORLD_POSITIONS)

    delta_trans = avango.gua.Vec3(0.0,0.0,0.0)

    if len(results.value) > 0:

      first_hit = results.value[0]

      hit_world_trans = first_hit.WorldPosition.value

      new_pos = avango.gua.make_trans_mat(hit_world_trans)# * avango.gua.make_trans_mat(0.0,self.OffsetToGround.value, 0.0)

      delta_trans = (new_pos.get_translate() - self.InTransform.value.get_translate())

      self.DistanceToGround.value = delta_trans.y
    else:
      self.DistanceToGround.value = self.MaxDistanceToGround.value + 1

    delta_norm = avango.gua.Vec3(delta_trans.x,delta_trans.y,delta_trans.z)
    length = delta_norm.normalize()

    # time dependent:
    delta_time = self._timer.Time.value - self._last_time
    self._last_time = self._timer.Time.value

    self._velocity_y -= (self._gravity*delta_time*self.FPS)
    
    if length > math.fabs(self._velocity_y):

      self.OutTransform.value = self.OutTransform.value * avango.gua.make_trans_mat(delta_norm*self._velocity_y)

    else:

      self.OutTransform.value = self.OutTransform.value * avango.gua.make_trans_mat(delta_trans)

      self._velocity_y = 0.0