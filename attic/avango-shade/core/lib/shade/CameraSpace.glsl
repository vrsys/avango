// SHADE
// CameraSpace

vec4 CameraSpace_transform_impl(CameraSpace_SELF, vec4 v)
{
  return v;
}

void CameraSpace_init_vertex_impl(CameraSpace_SELF)
{
  CameraSpace_set_position(self, (gl_ModelViewMatrix * gl_Vertex).xyz);
  CameraSpace_set_normal(self, gl_NormalMatrix * gl_Normal);
}

vec3 CameraSpace_get_transformed_normal_impl(CameraSpace_SELF)
{
  return normalize(CameraSpace_get_normal(self));
}

vec3 CameraSpace_get_transformed_eyepoint_impl(CameraSpace_SELF)
{
  return vec3(0., 0., 0.);
}
