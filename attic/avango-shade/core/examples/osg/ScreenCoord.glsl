// SHADE
// ScreenCoord

void ScreenCoord_init_vertex_impl(ScreenCoord_SELF)
{
  vec4 transformed_vertex = gl_ModelViewProjectionMatrix * gl_Vertex;
  ScreenCoord_set_uv_coord(self, transformed_vertex.xyw);
}

vec2 ScreenCoord_get_fragment_impl(ScreenCoord_SELF)
{
  vec3 transformed_vertex = ScreenCoord_get_uv_coord(self);
  vec2 projected_vertex = transformed_vertex.xy / transformed_vertex.z;
  return (projected_vertex + 1.) * 0.5;
}
