// SHADE
// ObjectSpace

vec4 ObjectSpace_transform_impl(ObjectSpace_SELF, vec4 v)
{
  return gl_ModelViewMatrixInverse * v;
}

void ObjectSpace_init_vertex_impl(ObjectSpace_SELF)
{
  ObjectSpace_set_position(self, gl_Vertex.xyz);
  ObjectSpace_set_normal(self, gl_Normal);

  vec4 eyepoint = ObjectSpace_transform_impl(self, vec4(0., 0., 0., 1.));
  ObjectSpace_set_eyepoint(self, eyepoint.xyz);
}

vec3 ObjectSpace_get_transformed_normal_impl(ObjectSpace_SELF)
{
  return normalize(ObjectSpace_get_normal(self));
}

vec3 ObjectSpace_get_transformed_eyepoint_impl(ObjectSpace_SELF)
{
  return ObjectSpace_get_eyepoint(self);
}
