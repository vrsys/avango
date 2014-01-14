// SHADE
// UVCoord

vec2 UVCoord_get_fragment_impl(UVCoord_SELF)
{
  return UVCoord_get_uv_coord(self);
}

void UVCoord_init_vertex_impl(UVCoord_SELF)
{
  UVCoord_set_uv_coord(self, gl_MultiTexCoord0.xy);
}
