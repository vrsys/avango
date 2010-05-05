#define OBJREF int
#define FOR_EACH(list, index) for(OBJREF index = list.begin; index != list.end; ++index)
struct LIST
{
  int begin;
  int end;
};
#define SHADE_FRAGMENT_SHADER 1
const OBJREF obj_0x7d2060_surface = OBJREF(1);
varying vec3 obj_0x7d2060_position;
varying vec3 obj_0x7d2060_normal;
varying vec2 obj_0x7d2060_uv;
uniform vec3 obj_0x7d2690_color_;
const LIST obj_0x7d2690_lights_ = LIST(2, 3);
vec3 obj_0x7d2690_acc_color_;
vec3 obj_0x7d2690_acc_normal_;
vec3 obj_0x7d2690_acc_position_;
vec2 obj_0x7d2690_acc_uv_;
uniform float obj_0x7d2690_diffuse_;
uniform float obj_0x7d2690_specular_;
uniform vec3 obj_0x7d34f0_color;
uniform vec3 obj_0x7d34f0_position;
#define Example_SELF OBJREF self, in OBJREF surface, in vec3 position, in vec3 normal, in vec2 uv
#define Light_SELF OBJREF self, in vec3 color
#define PointLight_SELF OBJREF self, in vec3 position
#define Surface_SELF OBJREF self, in vec3 color_, in LIST lights_
#define Plastic_SELF OBJREF self, in float diffuse_, in float specular_
void Example_fragment_main(OBJREF self);
void Light_illuminate(OBJREF self, OBJREF arg1, vec3 arg2);
vec3 Surface_shade(OBJREF self, vec3 arg1, vec3 arg2, vec2 arg3);
void Surface_illuminance(OBJREF self, vec3 arg1, vec3 arg2);
vec3 Surface_get_color(OBJREF self);
LIST Surface_get_lights(OBJREF self);
vec3 Surface_get_normal(OBJREF self);
vec2 Surface_get_uv(OBJREF self);
vec3 Surface_get_position(OBJREF self);
void Surface_init(OBJREF self, vec3 arg1, vec3 arg2, vec2 arg3);
void Surface_accum_color(OBJREF self, vec3 arg1);
vec3 Surface_gather(OBJREF self);
void Plastic_illuminance(OBJREF self, vec3 arg1, vec3 arg2);
vec3 Surface_get_acc_color_(OBJREF self);
vec3 Surface_set_acc_color_(OBJREF self, vec3 arg);
vec3 Surface_get_acc_normal_(OBJREF self);
vec3 Surface_set_acc_normal_(OBJREF self, vec3 arg);
vec3 Surface_get_acc_position_(OBJREF self);
vec3 Surface_set_acc_position_(OBJREF self, vec3 arg);
vec2 Surface_get_acc_uv_(OBJREF self);
vec2 Surface_set_acc_uv_(OBJREF self, vec2 arg);
// SHADE
#ifdef SHADE_FRAGMENT_SHADER
void Example_fragment_impl(Example_SELF)
 {
  vec3 shading = Surface_shade(surface, position, normalize(normal), uv);
  gl_FragColor = vec4(shading, 1.);
 }
#endif
// SHADE
#ifdef SHADE_FRAGMENT_SHADER
#ifdef PointLight_SELF
void PointLight_illuminate_impl(PointLight_SELF, OBJREF surface, vec3 point_position)
 {
  vec3 direction = normalize(position - point_position);
  Surface_illuminance(surface, vec3(1., 1., 1.), direction);
 }
#endif
#ifdef Surface_SELF
vec3 Surface_get_color_impl(Surface_SELF)
 {
  return color_;
 }
LIST Surface_get_lights_impl(Surface_SELF)
 {
  return lights_;
 }
vec3 Surface_get_position_impl(Surface_SELF)
 {
  return Surface_get_acc_position_(self);
 }
vec3 Surface_get_normal_impl(Surface_SELF)
 {
  return Surface_get_acc_normal_(self);
 }
vec2 Surface_get_uv_impl(Surface_SELF)
 {
  return Surface_get_acc_uv_(self);
 }
void Surface_init_impl(Surface_SELF, vec3 position, vec3 normal, vec2 uv)
 {
  Surface_set_acc_color_(self, vec3(0.));
  Surface_set_acc_position_(self, position);
  Surface_set_acc_normal_(self, normal);
  Surface_set_acc_uv_(self, uv);
 }
void Surface_accum_color_impl(Surface_SELF, vec3 color)
 {
  Surface_set_acc_color_(self, Surface_get_acc_color_(self) + color);
 }
vec3 Surface_gather_impl(Surface_SELF)
 {
  return Surface_get_acc_color_(self);
 }
vec3 Surface_shade_impl(Surface_SELF, vec3 position, vec3 normal, vec2 uv)
 {
  Surface_init(self, position, normal, uv);
  FOR_EACH(lights_, l)
   {
    Light_illuminate(l, self, position);
   }
  return Surface_gather(self);
 }
void Surface_illuminance_uv(Surface_SELF, vec3 color, vec3 direction)
 {
 }
#endif
#ifdef Plastic_SELF
void Plastic_illuminance_impl(Plastic_SELF, vec3 color, vec3 direction)
 {
  vec3 normal = Surface_get_normal(self);
  vec3 eye = normalize(-Surface_get_position(self));
  float diffuse_intensity = max(0., dot(direction, normal));
  float specular_intensity = pow(max(0., dot(normal, normalize(direction+eye))), 16.);
  vec3 fragment_color = diffuse_ * color * Surface_get_color(self) * diffuse_intensity;
  fragment_color += specular_ * color * vec3(1.) * specular_intensity;
  Surface_accum_color(self, fragment_color);
 }
#endif
#endif /* SHADE_FRAGMENT_SHADER */
void Example_fragment_main(OBJREF self)
{
  if (self == 0)
  {
    Example_fragment_impl(0, obj_0x7d2060_surface, obj_0x7d2060_position, obj_0x7d2060_normal, obj_0x7d2060_uv);
    return;
  }
}
void Light_illuminate(OBJREF self, OBJREF arg1, vec3 arg2)
{
  if (self == 2)
  {
    PointLight_illuminate_impl(2, obj_0x7d34f0_position, arg1, arg2);
    return;
  }
}
vec3 Surface_shade(OBJREF self, vec3 arg1, vec3 arg2, vec2 arg3)
{
  if (self == 1)
  {
    return Surface_shade_impl(1, obj_0x7d2690_color_, obj_0x7d2690_lights_, arg1, arg2, arg3);
  }
  return vec3(0, 0, 0);
}
void Surface_illuminance(OBJREF self, vec3 arg1, vec3 arg2)
{
  if (self == 1)
  {
    Plastic_illuminance_impl(1, obj_0x7d2690_diffuse_, obj_0x7d2690_specular_, arg1, arg2);
    return;
  }
}
vec3 Surface_get_color(OBJREF self)
{
  if (self == 1)
  {
    return Surface_get_color_impl(1, obj_0x7d2690_color_, obj_0x7d2690_lights_);
  }
  return vec3(0, 0, 0);
}
LIST Surface_get_lights(OBJREF self)
{
  if (self == 1)
  {
    return Surface_get_lights_impl(1, obj_0x7d2690_color_, obj_0x7d2690_lights_);
  }
  return LIST(0, 0);
}
vec3 Surface_get_normal(OBJREF self)
{
  if (self == 1)
  {
    return Surface_get_normal_impl(1, obj_0x7d2690_color_, obj_0x7d2690_lights_);
  }
  return vec3(0, 0, 0);
}
vec2 Surface_get_uv(OBJREF self)
{
  if (self == 1)
  {
    return Surface_get_uv_impl(1, obj_0x7d2690_color_, obj_0x7d2690_lights_);
  }
  return vec2(0, 0);
}
vec3 Surface_get_position(OBJREF self)
{
  if (self == 1)
  {
    return Surface_get_position_impl(1, obj_0x7d2690_color_, obj_0x7d2690_lights_);
  }
  return vec3(0, 0, 0);
}
void Surface_init(OBJREF self, vec3 arg1, vec3 arg2, vec2 arg3)
{
  if (self == 1)
  {
    Surface_init_impl(1, obj_0x7d2690_color_, obj_0x7d2690_lights_, arg1, arg2, arg3);
    return;
  }
}
void Surface_accum_color(OBJREF self, vec3 arg1)
{
  if (self == 1)
  {
    Surface_accum_color_impl(1, obj_0x7d2690_color_, obj_0x7d2690_lights_, arg1);
    return;
  }
}
vec3 Surface_gather(OBJREF self)
{
  if (self == 1)
  {
    return Surface_gather_impl(1, obj_0x7d2690_color_, obj_0x7d2690_lights_);
  }
  return vec3(0, 0, 0);
}
void Plastic_illuminance(OBJREF self, vec3 arg1, vec3 arg2)
{
  if (self == 1)
  {
    Plastic_illuminance_impl(1, obj_0x7d2690_diffuse_, obj_0x7d2690_specular_, arg1, arg2);
    return;
  }
}
vec3 Surface_get_acc_color_(OBJREF self)
{
  if (self == 1) return obj_0x7d2690_acc_color_;
  return vec3(0, 0, 0);
}
vec3 Surface_set_acc_color_(OBJREF self, vec3 arg)
{
  if (self == 1) return (obj_0x7d2690_acc_color_ = arg);
  return vec3(0, 0, 0);
}
vec3 Surface_get_acc_normal_(OBJREF self)
{
  if (self == 1) return obj_0x7d2690_acc_normal_;
  return vec3(0, 0, 0);
}
vec3 Surface_set_acc_normal_(OBJREF self, vec3 arg)
{
  if (self == 1) return (obj_0x7d2690_acc_normal_ = arg);
  return vec3(0, 0, 0);
}
vec3 Surface_get_acc_position_(OBJREF self)
{
  if (self == 1) return obj_0x7d2690_acc_position_;
  return vec3(0, 0, 0);
}
vec3 Surface_set_acc_position_(OBJREF self, vec3 arg)
{
  if (self == 1) return (obj_0x7d2690_acc_position_ = arg);
  return vec3(0, 0, 0);
}
vec2 Surface_get_acc_uv_(OBJREF self)
{
  if (self == 1) return obj_0x7d2690_acc_uv_;
  return vec2(0, 0);
}
vec2 Surface_set_acc_uv_(OBJREF self, vec2 arg)
{
  if (self == 1) return (obj_0x7d2690_acc_uv_ = arg);
  return vec2(0, 0);
}
void main(void)
{
  Example_fragment_main(0);
}
