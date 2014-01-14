#define SHADE_FRAGMENT_SHADER 1
#define OBJREF int
#define FOREACH(index, list) for(OBJREF index = list.begin; index != list.end; ++index)
struct LIST
{
  int begin;
  int end;
};
const OBJREF obj_0x999cd00_material = 1;
const LIST obj_0x999d260_lights = LIST(2, 4);
uniform vec3 obj_0x999dfb0_value;
uniform vec3 obj_0x999c148_value;
varying vec3 obj_0x99a04c8_value;
uniform vec3 obj_0x999c4b0_value;
uniform vec3 obj_0x99aa168_value;
varying vec3 obj_0x99a0de8_value;
const OBJREF obj_0x999d260_coordinate_system = 4;
varying vec3 obj_0x99a1490_value;
varying vec3 obj_0x99a1720_value;
vec3 obj_0x999dd48_tangent;
vec3 obj_0x999dd48_bitangent;
vec3 obj_0x999dd48_normal;
varying vec2 obj_0x99a23c8_value;
uniform sampler2D obj_0x999e3a8_texture_unit;
vec4 obj_0x999d260_acc_color;
uniform float obj_0x999d700_value;
uniform float obj_0x999d898_value;
uniform float obj_0x999da60_value;
uniform vec4 obj_0x999dc28_value;
const OBJREF obj_0x999cd00_geometry = 0;
void Initializeable_init_fragment(OBJREF self)
{
}
void Initializeable_init_fragment1(OBJREF self)
{
}
void Initializeable_init_fragment2(OBJREF self)
{
}
void shade_initialize()
{
  Initializeable_init_fragment(18);
  Initializeable_init_fragment1(4);
  Initializeable_init_fragment2(1);
}
vec4 IlluminatedMaterial_set_acc_color(OBJREF self, vec4 arg)
{
  {
return ((obj_0x999d260_acc_color)=(arg));
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
    IlluminatedMaterial_init_impl(1, obj_0x999d260_lights, obj_0x999d260_coordinate_system);
  }
}
vec3 TangentSpace_get_impl(OBJREF self)
{
return vec3(0., 0., 0.);}
vec3 Gettable_vec3_get(OBJREF self)
{
  {
return TangentSpace_get_impl(4);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_get_value(OBJREF self)
{
  {
return obj_0x99a04c8_value;
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_get_impl(OBJREF self)
{
return Value_inout_0_1_vec3_get_value(self);
}
vec3 Gettable_vec3_get1(OBJREF self)
{
  {
return Value_inout_0_1_vec3_get_impl(11);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_transformed_position(OBJREF self)
{
  {
return Gettable_vec3_get1(11);
  }
return vec3(0, 0, 0);
}
vec2 Value_inout_0_1_vec2_get_value(OBJREF self)
{
  {
return obj_0x99a23c8_value;
  }
return vec2(0, 0);
}
vec2 Value_inout_0_1_vec2_get_impl(OBJREF self)
{
return Value_inout_0_1_vec2_get_value(self);
}
vec2 Gettable_vec2_get1(OBJREF self)
{
  {
return Value_inout_0_1_vec2_get_impl(19);
  }
return vec2(0, 0);
}
vec2 UVCoord_get_uv_coord(OBJREF self)
{
  {
return Gettable_vec2_get1(19);
  }
return vec2(0, 0);
}
vec2 UVCoord_get_fragment_impl(OBJREF self)
{
return UVCoord_get_uv_coord(self);}
vec2 PerStageGettable_vec2_get_fragment(OBJREF self)
{
  {
return UVCoord_get_fragment_impl(18);
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
return PerStageGettable_vec2_get_impl(18);
  }
return vec2(0, 0);
}
vec2 Texture2D_get_uv(OBJREF self)
{
  {
return Gettable_vec2_get(18);
  }
return vec2(0, 0);
}
vec4 Texture2D_get_texture_impl(OBJREF self, sampler2D texture_unit)
{
return texture2D(texture_unit, Texture2D_get_uv(self));}
vec4 Texture2D_get_texture(OBJREF self)
{
  {
return Texture2D_get_texture_impl(17, obj_0x999e3a8_texture_unit);
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
return Texture2D_get_impl(17, obj_0x999e3a8_texture_unit);
  }
return vec4(0, 0, 0, 0);
}
vec4 TangentSpace_get_normal_map(OBJREF self)
{
  {
return Gettable_vec4_get(17);
  }
return vec4(0, 0, 0, 0);
}
vec3 TangentSpace_get_transformed_normal_impl(OBJREF self)
{
return normalize(TangentSpace_get_normal_map(self).xyz - vec3(0.5, 0.5, 0.5));}
vec3 ICoordinateSystem_get_transformed_normal(OBJREF self)
{
  {
return TangentSpace_get_transformed_normal_impl(4);
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
return IlluminatedMaterial_get_normal_impl(1, obj_0x999d260_lights, obj_0x999d260_coordinate_system);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_get_value1(OBJREF self)
{
  {
return obj_0x99a1720_value;
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_get_impl1(OBJREF self)
{
return Value_inout_0_1_vec3_get_value1(self);
}
vec3 Gettable_vec3_get2(OBJREF self)
{
  {
return Value_inout_0_1_vec3_get_impl1(16);
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_eyepoint(OBJREF self)
{
  {
return Gettable_vec3_get2(16);
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_transformed_eyepoint_impl(OBJREF self)
{
return TangentSpace_get_eyepoint(self);}
vec3 ICoordinateSystem_get_transformed_eyepoint(OBJREF self)
{
  {
return TangentSpace_get_transformed_eyepoint_impl(4);
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
return IlluminatedMaterial_get_eyepoint_impl(1, obj_0x999d260_lights, obj_0x999d260_coordinate_system);
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
return IlluminatedMaterial_get_position_impl(1, obj_0x999d260_lights, obj_0x999d260_coordinate_system);
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
return Value_uniform_float_get_impl(7, obj_0x999da60_value);
  }
return 0.000000e+00;
}
float Plastic_get_roughness(OBJREF self)
{
  {
return Gettable_float_get(7);
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
return Value_uniform_float_get_impl1(5, obj_0x999d700_value);
  }
return 0.000000e+00;
}
float Plastic_get_diffuse(OBJREF self)
{
  {
return Gettable_float_get1(5);
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
return Value_uniform_vec4_get_impl(8, obj_0x999dc28_value);
  }
return vec4(0, 0, 0, 0);
}
vec4 Plastic_get_color(OBJREF self)
{
  {
return Gettable_vec4_get1(8);
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
return Value_uniform_float_get_impl2(6, obj_0x999d898_value);
  }
return 0.000000e+00;
}
float Plastic_get_specular(OBJREF self)
{
  {
return Gettable_float_get2(6);
  }
return 0.000000e+00;
}
vec4 IlluminatedMaterial_get_acc_color(OBJREF self)
{
  {
return obj_0x999d260_acc_color;
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
    IlluminatedMaterial_accum_color_impl(1, obj_0x999d260_lights, obj_0x999d260_coordinate_system, arg1);
  }
}
void Plastic_illuminance_impl(OBJREF self, vec3 color, vec3 direction)
{
vec3 normal = IlluminatedMaterial_get_normal(self);
  vec3 eye = normalize(IlluminatedMaterial_get_eyepoint(self)-IlluminatedMaterial_get_position(self));

  float diffuse_intensity = dot(direction, normal);
  //if (diffuse_intensity <= 0.)
  //  return;
  if (diffuse_intensity > 0.)
   {
    float specular_intensity = pow(max(0., dot(normal, normalize(direction+eye))), Plastic_get_roughness(self));
    vec4 fragment_color = diffuse_intensity * Plastic_get_diffuse(self) * vec4(color, 1.) * Plastic_get_color(self);
    fragment_color += specular_intensity * Plastic_get_specular(self) * vec4(color, 1.) * vec4(1.);
    IlluminatedMaterial_accum_color(self, fragment_color);
   }}
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
return Value_uniform_vec3_get_impl(10, obj_0x999c148_value);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_color(OBJREF self)
{
  {
return Gettable_vec3_get3(10);
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
    PointLight_illuminate_impl(2, arg1, arg2);
  }
}
vec3 Value_inout_0_1_vec3_get_value2(OBJREF self)
{
  {
return obj_0x99a0de8_value;
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_get_impl2(OBJREF self)
{
return Value_inout_0_1_vec3_get_value2(self);
}
vec3 Gettable_vec3_get4(OBJREF self)
{
  {
return Value_inout_0_1_vec3_get_impl2(14);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_transformed_position1(OBJREF self)
{
  {
return Gettable_vec3_get4(14);
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
return Value_uniform_vec3_get_impl1(13, obj_0x99aa168_value);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_color1(OBJREF self)
{
  {
return Gettable_vec3_get5(13);
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
    PointLight_illuminate_impl1(3, arg1, arg2);
  }
}
vec4 IlluminatedMaterial_gather_impl(OBJREF self, LIST lights, OBJREF coordinate_system)
{
return IlluminatedMaterial_get_acc_color(self);}
vec4 IlluminatedMaterial_gather(OBJREF self)
{
  {
return IlluminatedMaterial_gather_impl(1, obj_0x999d260_lights, obj_0x999d260_coordinate_system);
  }
return vec4(0, 0, 0, 0);
}
vec4 IlluminatedMaterial_shade_impl(OBJREF self, LIST lights, OBJREF coordinate_system)
{
IlluminatedMaterial_init(self);
  vec3 position = Gettable_vec3_get(coordinate_system);
    {
    OBJREF iter = 2;
ILight_illuminate(iter, self, position);  }
  {
    OBJREF iter = 3;
ILight_illuminate1(iter, self, position);  }

  return IlluminatedMaterial_gather(self);}
vec4 IMaterial_shade(OBJREF self)
{
  {
return IlluminatedMaterial_shade_impl(1, obj_0x999d260_lights, obj_0x999d260_coordinate_system);
  }
return vec4(0, 0, 0, 0);
}
void Surface_fragment_impl(OBJREF self, OBJREF material, OBJREF geometry)
{
gl_FragColor = IMaterial_shade(material);}
void Enterable_enter_fragment(OBJREF self)
{
  {
    Surface_fragment_impl(0, obj_0x999cd00_material, obj_0x999cd00_geometry);
  }
}
void main()
{
  shade_initialize();
  Enterable_enter_fragment(0);
}
