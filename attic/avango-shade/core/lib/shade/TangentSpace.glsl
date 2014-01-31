// SHADE
// TangentSpace

vec4 TangentSpace_transform_impl(TangentSpace_SELF, vec4 v)
{
  vec4 v_object = gl_ModelViewMatrixInverse * v - gl_Vertex;
  vec3 result;
  result.x = dot(v_object.xyz, TangentSpace_get_tangent(self));
  result.y = dot(v_object.xyz, TangentSpace_get_bitangent(self));
  result.z = dot(v_object.xyz, TangentSpace_get_normal(self));
  return vec4(result, 1.);
}

void TangentSpace_init_vertex_impl(TangentSpace_SELF)
{
  TangentSpace_set_position(self, gl_Vertex.xyz);
  TangentSpace_set_tangent(self, gl_MultiTexCoord1.xyz);
  TangentSpace_set_bitangent(self, gl_MultiTexCoord2.xyz);
  TangentSpace_set_normal(self, gl_Normal);

  vec4 eyepoint = TangentSpace_transform_impl(self, vec4(0., 0., 0., 1.));
  TangentSpace_set_eyepoint(self, eyepoint.xyz);
}

vec3 TangentSpace_get_impl(TangentSpace_SELF)
{
  return vec3(0., 0., 0.);
}

vec3 TangentSpace_get_transformed_normal_impl(TangentSpace_SELF)
{
  return normalize(TangentSpace_get_normal_map(self).xyz - vec3(0.5, 0.5, 0.5));
}

vec3 TangentSpace_get_transformed_eyepoint_impl(TangentSpace_SELF)
{
  return TangentSpace_get_eyepoint(self);
}
