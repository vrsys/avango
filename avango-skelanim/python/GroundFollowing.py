import avango
import avango.script

class GroundFollowing(avango.script.Script):
  
  InTransform = avango.gua.SFMatrix4()
  OutTransform = avango.gua.SFMatrix4()
  OutTransform.value = avango.gua.make_identity_mat()

  OffsetToGround = avango.SFFloat()
  MaxDistanceToGround = avango.SFFloat()
  SceneGraph = avango.gua.nodes.SceneGraph()

  DistanceToGround = avango.SFFloat()

  _velocity_y = 0.0
  _gravity = -0.00005

  def __init__(self):
    self.super(GroundFollowing).__init__()

    self.OffsetToGround.value = 0.0
    self.MaxDistanceToGround.value = 1000.0
    self.__ray = avango.gua.nodes.RayNode()

    self.always_evaluate(True)

  def evaluate(self):

    in_translation = self.InTransform.value.get_translate()
    in_translation.y += self.OffsetToGround.value

    self.__ray.Transform.value = avango.gua.make_trans_mat(in_translation) *\
                                 avango.gua.make_rot_mat(-90, 1, 0, 0) *\
                                 avango.gua.make_scale_mat(1.0, 1.0, self.MaxDistanceToGround.value)

    results = self.SceneGraph.value.ray_test(
                                     self.__ray,
                                     avango.gua.PickingOptions.PICK_ONLY_FIRST_OBJECT |
                                     avango.gua.PickingOptions.GET_POSITIONS)


    delta_trans = avango.gua.Vec3(0.0,0.0,0.0)

    if len(results.value) > 0:

      first_hit = results.value[0]

      hit_world = first_hit.Object.value.Transform.value * avango.gua.make_trans_mat(first_hit.Position.value)
      
      hit_world_trans = hit_world.get_translate()
      #hit_world_trans = first_hit.WorldPosition.value

      new_pos = avango.gua.make_trans_mat(hit_world_trans)# * avango.gua.make_trans_mat(0.0,self.OffsetToGround.value, 0.0)

      delta_trans = (new_pos.get_translate() - self.InTransform.value.get_translate())

      self.DistanceToGround.value = delta_trans.y
    else:
      self.DistanceToGround.value = self.MaxDistanceToGround.value + 1


    delta_norm = avango.gua.Vec3(delta_trans.x,delta_trans.y,delta_trans.z)
    length = delta_norm.normalize()

    if length > self._velocity_y and delta_trans.y < 0.0:

      self._velocity_y -= self._gravity

      #self.OutTransform.value = self.OutTransform.value * avango.gua.make_trans_mat(delta_norm*0.01)
      self.OutTransform.value = self.OutTransform.value * avango.gua.make_trans_mat(delta_norm*self._velocity_y)
    else:
      if delta_trans.y < 0.0:
        self.OutTransform.value = self.OutTransform.value * avango.gua.make_trans_mat(delta_trans)
      else:
        self.OutTransform.value = self.OutTransform.value * avango.gua.make_trans_mat(delta_trans/4.0)#smooth walking stairs

      self._velocity_y = 0.0