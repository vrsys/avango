#version 120
#define SHADE_FRAGMENT_SHADER 1
#define OBJREF int
#define FOREACH(index, list) for(OBJREF index = list.begin; index != list.end; ++index)
struct LIST
{
  int begin;
  int end;
};
const OBJREF obj_0xa038f28_material = 1;
const LIST obj_0xa0390f0_lights = LIST(3, 5);
uniform vec3 obj_0xa039f38_value;
uniform vec3 obj_0xa03b1e0_value;
varying vec3 obj_0xa03e100_value;
const OBJREF obj_0xa03e208_geometry = 17;
vec3 obj_0xa03bbb8_value;
vec4 obj_0xa03b908_interpolated_light_normal;
const OBJREF obj_0xa03b908_coordinate_system = 19;
varying vec3 obj_0xa03fab8_value;
const OBJREF obj_0xa03fbc0_geometry = 29;
varying vec3 obj_0xa0401d0_value;
const OBJREF obj_0xa0402a0_geometry = 32;
vec3 obj_0xa039cd0_tangent;
vec3 obj_0xa039cd0_bitangent;
vec3 obj_0xa039cd0_normal;
varying vec2 obj_0xa041d10_value;
const OBJREF obj_0xa041e10_geometry = 39;
uniform sampler2D obj_0xa03a330_texture_unit;
uniform sampler2D obj_0xa03b908_texture_unit;
uniform vec3 obj_0xa03b550_value;
uniform vec3 obj_0xa03b720_value;
varying vec3 obj_0xa042c50_value;
const OBJREF obj_0xa042d58_geometry = 47;
const OBJREF obj_0xa0390f0_coordinate_system = 5;
vec4 obj_0xa0390f0_acc_color;
uniform float obj_0xa039658_value;
uniform float obj_0xa039820_value;
uniform float obj_0xa0399e8_value;
uniform vec4 obj_0xa039bb0_value;
const OBJREF obj_0xa038f28_geometry = 2;
void Initializeable_init_fragment(OBJREF self)
{
}
void Initializeable_init_fragment1(OBJREF self)
{
}
void Initializeable_init_fragment2(OBJREF self)
{
}
void Initializeable_init_fragment3(OBJREF self)
{
}
void Initializeable_init_fragment4(OBJREF self)
{
}
void Initializeable_init_fragment5(OBJREF self)
{
}
void Initializeable_init_fragment6(OBJREF self)
{
}
void Initializeable_init_fragment7(OBJREF self)
{
}
void Initializeable_init_fragment8(OBJREF self)
{
}
void shade_initialize()
{
  Initializeable_init_fragment(22);
  Initializeable_init_fragment1(25);
  Initializeable_init_fragment2(36);
  Initializeable_init_fragment3(33);
  Initializeable_init_fragment4(19);
  Initializeable_init_fragment5(47);
  Initializeable_init_fragment6(14);
  Initializeable_init_fragment7(44);
  Initializeable_init_fragment8(1);
}
vec4 IlluminatedMaterial_set_acc_color(OBJREF self, vec4 arg)
{
  {
return ((obj_0xa0390f0_acc_color)=(arg));
  }
return vec4(0, 0, 0, 0);
}
void IlluminatedMaterial_init_impl(OBJREF self, LIST lights, OBJREF coordinate_system)
{
vec4 black = vec4(0.);
  IlluminatedMaterial_set_acc_color(self, black);}
void IlluminatedMaterial_init(OBJREF self)
{
  {
    IlluminatedMaterial_init_impl(1, obj_0xa0390f0_lights, obj_0xa0390f0_coordinate_system);
  }
}
vec3 TangentSpace_get_impl(OBJREF self)
{
return vec3(0., 0., 0.);}
vec3 Gettable_vec3_get(OBJREF self)
{
  {
return TangentSpace_get_impl(5);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_2_1_vec3_get_value(OBJREF self)
{
  {
return obj_0xa03e100_value;
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_2_1_vec3_get_impl(OBJREF self)
{
return Value_inout_2_1_vec3_get_value(self);
}
vec3 Gettable_vec3_get1(OBJREF self)
{
  {
return Value_inout_2_1_vec3_get_impl(13);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_transformed_position(OBJREF self)
{
  {
return Gettable_vec3_get1(13);
  }
return vec3(0, 0, 0);
}
vec2 Value_inout_2_1_vec2_get_value(OBJREF self)
{
  {
return obj_0xa041d10_value;
  }
return vec2(0, 0);
}
vec2 Value_inout_2_1_vec2_get_impl(OBJREF self)
{
return Value_inout_2_1_vec2_get_value(self);
}
vec2 Gettable_vec2_get1(OBJREF self)
{
  {
return Value_inout_2_1_vec2_get_impl(35);
  }
return vec2(0, 0);
}
vec2 UVCoord_get_uv_coord(OBJREF self)
{
  {
return Gettable_vec2_get1(35);
  }
return vec2(0, 0);
}
vec2 UVCoord_get_fragment_impl(OBJREF self)
{
return UVCoord_get_uv_coord(self);}
vec2 PerStageGettable_vec2_get_fragment(OBJREF self)
{
  {
return UVCoord_get_fragment_impl(33);
  }
return vec2(0, 0);
}
vec2 PerStageGettable_vec2_get_impl(OBJREF self)
{
return PerStageGettable_vec2_get_fragment(self);
return vec2(0, 0);
}
vec2 Gettable_vec2_get(OBJREF self)
{
  {
return PerStageGettable_vec2_get_impl(33);
  }
return vec2(0, 0);
}
vec2 Texture2D_get_uv(OBJREF self)
{
  {
return Gettable_vec2_get(33);
  }
return vec2(0, 0);
}
vec4 Texture2D_get_texture_impl(OBJREF self, sampler2D texture_unit)
{
return texture2D(texture_unit, Texture2D_get_uv(self));}
vec4 Texture2D_get_texture(OBJREF self)
{
  {
return Texture2D_get_texture_impl(26, obj_0xa03a330_texture_unit);
  }
return vec4(0, 0, 0, 0);
}
vec4 Texture2D_get_impl(OBJREF self, sampler2D texture_unit)
{
return Texture2D_get_texture(self);
return vec4(0, 0, 0, 0);
}
vec4 Gettable_vec4_get(OBJREF self)
{
  {
return Texture2D_get_impl(26, obj_0xa03a330_texture_unit);
  }
return vec4(0, 0, 0, 0);
}
vec4 TangentSpace_get_normal_map(OBJREF self)
{
  {
return Gettable_vec4_get(26);
  }
return vec4(0, 0, 0, 0);
}
vec3 TangentSpace_get_transformed_normal_impl(OBJREF self)
{
return normalize(TangentSpace_get_normal_map(self).xyz - vec3(0.5, 0.5, 0.5));}
vec3 ICoordinateSystem_get_transformed_normal(OBJREF self)
{
  {
return TangentSpace_get_transformed_normal_impl(5);
  }
return vec3(0, 0, 0);
}
vec3 IlluminatedMaterial_get_normal_impl(OBJREF self, LIST lights, OBJREF coordinate_system)
{
vec3 position = ICoordinateSystem_get_transformed_normal(coordinate_system);
  return position;}
vec3 IlluminatedMaterial_get_normal(OBJREF self)
{
  {
return IlluminatedMaterial_get_normal_impl(1, obj_0xa0390f0_lights, obj_0xa0390f0_coordinate_system);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_2_1_vec3_get_value1(OBJREF self)
{
  {
return obj_0xa0401d0_value;
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_2_1_vec3_get_impl1(OBJREF self)
{
return Value_inout_2_1_vec3_get_value1(self);
}
vec3 Gettable_vec3_get2(OBJREF self)
{
  {
return Value_inout_2_1_vec3_get_impl1(24);
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_eyepoint(OBJREF self)
{
  {
return Gettable_vec3_get2(24);
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_transformed_eyepoint_impl(OBJREF self)
{
return TangentSpace_get_eyepoint(self);}
vec3 ICoordinateSystem_get_transformed_eyepoint(OBJREF self)
{
  {
return TangentSpace_get_transformed_eyepoint_impl(5);
  }
return vec3(0, 0, 0);
}
vec3 IlluminatedMaterial_get_eyepoint_impl(OBJREF self, LIST lights, OBJREF coordinate_system)
{
vec3 position = ICoordinateSystem_get_transformed_eyepoint(coordinate_system);
  return position;}
vec3 IlluminatedMaterial_get_eyepoint(OBJREF self)
{
  {
return IlluminatedMaterial_get_eyepoint_impl(1, obj_0xa0390f0_lights, obj_0xa0390f0_coordinate_system);
  }
return vec3(0, 0, 0);
}
vec3 IlluminatedMaterial_get_position_impl(OBJREF self, LIST lights, OBJREF coordinate_system)
{
vec3 position = Gettable_vec3_get(coordinate_system);
  return position;}
vec3 IlluminatedMaterial_get_position(OBJREF self)
{
  {
return IlluminatedMaterial_get_position_impl(1, obj_0xa0390f0_lights, obj_0xa0390f0_coordinate_system);
  }
return vec3(0, 0, 0);
}
float Value_uniform_float_get_impl(OBJREF self, float value)
{
return value;
}
float Gettable_float_get(OBJREF self)
{
  {
return Value_uniform_float_get_impl(8, obj_0xa0399e8_value);
  }
return 0.000000e+00;
}
float Plastic_get_roughness(OBJREF self)
{
  {
return Gettable_float_get(8);
  }
return 0.000000e+00;
}
float Value_uniform_float_get_impl1(OBJREF self, float value)
{
return value;
}
float Gettable_float_get1(OBJREF self)
{
  {
return Value_uniform_float_get_impl1(6, obj_0xa039658_value);
  }
return 0.000000e+00;
}
float Plastic_get_diffuse(OBJREF self)
{
  {
return Gettable_float_get1(6);
  }
return 0.000000e+00;
}
vec4 Value_uniform_vec4_get_impl(OBJREF self, vec4 value)
{
return value;
}
vec4 Gettable_vec4_get1(OBJREF self)
{
  {
return Value_uniform_vec4_get_impl(9, obj_0xa039bb0_value);
  }
return vec4(0, 0, 0, 0);
}
vec4 Plastic_get_color(OBJREF self)
{
  {
return Gettable_vec4_get1(9);
  }
return vec4(0, 0, 0, 0);
}
float Value_uniform_float_get_impl2(OBJREF self, float value)
{
return value;
}
float Gettable_float_get2(OBJREF self)
{
  {
return Value_uniform_float_get_impl2(7, obj_0xa039820_value);
  }
return 0.000000e+00;
}
float Plastic_get_specular(OBJREF self)
{
  {
return Gettable_float_get2(7);
  }
return 0.000000e+00;
}
vec4 IlluminatedMaterial_get_acc_color(OBJREF self)
{
  {
return obj_0xa0390f0_acc_color;
  }
return vec4(0, 0, 0, 0);
}
void IlluminatedMaterial_accum_color_impl(OBJREF self, LIST lights, OBJREF coordinate_system, vec4 color)
{
vec4 accum_color = IlluminatedMaterial_get_acc_color(self);
  accum_color += color;
  IlluminatedMaterial_set_acc_color(self, accum_color);}
void IlluminatedMaterial_accum_color(OBJREF self, vec4 arg1)
{
  {
    IlluminatedMaterial_accum_color_impl(1, obj_0xa0390f0_lights, obj_0xa0390f0_coordinate_system, arg1);
  }
}
void Plastic_illuminance_impl(OBJREF self, vec3 color, vec3 direction)
{
vec3 normal = IlluminatedMaterial_get_normal(self);
  vec3 eye = normalize(IlluminatedMaterial_get_eyepoint(self)-IlluminatedMaterial_get_position(self));

  float diffuse_intensity = dot(direction, normal);
  if (diffuse_intensity <= 0.)
    return;
  float specular_intensity = pow(max(0., dot(normal, normalize(direction+eye))), Plastic_get_roughness(self));
  vec4 fragment_color = diffuse_intensity * Plastic_get_diffuse(self) * vec4(color, 1.) * Plastic_get_color(self);
  fragment_color += specular_intensity * Plastic_get_specular(self) * vec4(color, 1.) * vec4(1.);
  IlluminatedMaterial_accum_color(self, fragment_color);}
void IlluminatedMaterial_illuminance(OBJREF self, vec3 arg1, vec3 arg2)
{
  {
    Plastic_illuminance_impl(1, arg1, arg2);
  }
}
vec3 Value_uniform_vec3_get_impl(OBJREF self, vec3 value)
{
return value;
}
vec3 Gettable_vec3_get3(OBJREF self)
{
  {
return Value_uniform_vec3_get_impl(11, obj_0xa03b1e0_value);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_color(OBJREF self)
{
  {
return Gettable_vec3_get3(11);
  }
return vec3(0, 0, 0);
}
void PointLight_illuminate_impl(OBJREF self, OBJREF surface, vec3 point_position)
{
vec3 pos = PointLight_get_transformed_position(self);
  vec3 direction = normalize(pos - point_position);
  IlluminatedMaterial_illuminance(surface, PointLight_get_color(self), direction);}
void ILight_illuminate(OBJREF self, OBJREF arg1, vec3 arg2)
{
  {
    PointLight_illuminate_impl(3, arg1, arg2);
  }
}
vec3 Value_inout_2_1_vec3_get_value2(OBJREF self)
{
  {
return obj_0xa042c50_value;
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_2_1_vec3_get_impl2(OBJREF self)
{
return Value_inout_2_1_vec3_get_value2(self);
}
vec3 Gettable_vec3_get4(OBJREF self)
{
  {
return Value_inout_2_1_vec3_get_impl2(43);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_transformed_position1(OBJREF self)
{
  {
return Gettable_vec3_get4(43);
  }
return vec3(0, 0, 0);
}
vec3 Value_uniform_vec3_get_impl1(OBJREF self, vec3 value)
{
return value;
}
vec3 Gettable_vec3_get5(OBJREF self)
{
  {
return Value_uniform_vec3_get_impl1(41, obj_0xa03b720_value);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_color1(OBJREF self)
{
  {
return Gettable_vec3_get5(41);
  }
return vec3(0, 0, 0);
}
void PointLight_illuminate_impl1(OBJREF self, OBJREF surface, vec3 point_position)
{
vec3 pos = PointLight_get_transformed_position1(self);
  vec3 direction = normalize(pos - point_position);
  IlluminatedMaterial_illuminance(surface, PointLight_get_color1(self), direction);}
void ILight_illuminate1(OBJREF self, OBJREF arg1, vec3 arg2)
{
  {
    PointLight_illuminate_impl1(4, arg1, arg2);
  }
}
vec4 IlluminatedMaterial_gather_impl(OBJREF self, LIST lights, OBJREF coordinate_system)
{
return IlluminatedMaterial_get_acc_color(self);}
vec4 IlluminatedMaterial_gather(OBJREF self)
{
  {
return IlluminatedMaterial_gather_impl(1, obj_0xa0390f0_lights, obj_0xa0390f0_coordinate_system);
  }
return vec4(0, 0, 0, 0);
}
vec4 IlluminatedMaterial_shade_impl(OBJREF self, LIST lights, OBJREF coordinate_system)
{
IlluminatedMaterial_init(self);
  vec3 position = Gettable_vec3_get(coordinate_system);
    {
    OBJREF iter = 3;
ILight_illuminate(iter, self, position);  }
  {
    OBJREF iter = 4;
ILight_illuminate1(iter, self, position);  }

  return IlluminatedMaterial_gather(self);}
vec4 IMaterial_shade(OBJREF self)
{
  {
return IlluminatedMaterial_shade_impl(1, obj_0xa0390f0_lights, obj_0xa0390f0_coordinate_system);
  }
return vec4(0, 0, 0, 0);
}
void Surface_fragment_impl(OBJREF self, OBJREF material, OBJREF geometry)
{
gl_FragColor = IMaterial_shade(material);}
void Enterable_enter_fragment(OBJREF self)
{
  {
    Surface_fragment_impl(0, obj_0xa038f28_material, obj_0xa038f28_geometry);
  }
}
void main()
{
  shade_initialize();
  Enterable_enter_fragment(0);
}
