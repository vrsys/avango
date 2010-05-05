// SHADE
// Checkboard

vec4 Checkboard_get_pattern_impl(Checkboard_SELF)
{
  vec3 pos = Checkboard_get_position(self);
  vec3 steps = 2. * step(0.5, fract(scale*pos)) - 1.;
  return mix(Checkboard_get_color1(self), Checkboard_get_color2(self), max(0., steps.x*steps.y*steps.z));
}
