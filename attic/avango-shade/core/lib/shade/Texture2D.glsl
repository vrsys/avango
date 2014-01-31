// SHADE
// Texture2D

vec4 Texture2D_get_texture_impl(Texture2D_SELF)
{
  return texture2D(texture_unit, Texture2D_get_uv(self));
}
