#define SHADE_FRAGMENT_SHADER 1
#define OBJREF int
#define FOREACH(index, list) for(OBJREF index = list.begin; index != list.end; ++index)
struct LIST
{
  int begin;
  int end;
};
const OBJREF obj_0x986ccb8_material = 1;
const LIST obj_0x986d218_lights = LIST(2, 4);
uniform vec3 obj_0x986e170_value;
uniform vec3 obj_0x986e340_value;
varying vec3 obj_0x98708d8_value;
uniform vec3 obj_0x986e6b0_value;
uniform vec3 obj_0x986e880_value;
varying vec3 obj_0x9871210_value;
const OBJREF obj_0x986d218_coordinate_system = 4;
varying vec3 obj_0x98718b8_value;
varying vec3 obj_0x9871b48_value;
varying vec3 obj_0x9871e50_value;
vec4 obj_0x986d218_acc_color;
uniform float obj_0x986d6b8_value;
uniform float obj_0x986d850_value;
uniform float obj_0x986da18_value;
uniform vec4 obj_0x986dbe0_value;
const OBJREF obj_0x986ccb8_geometry = 0;
void Initializeable_init_fragment(OBJREF self)
{
}
void Initializeable_init_fragment1(OBJREF self)
{
}
void shade_initialize()
{
  Initializeable_init_fragment(4);
  Initializeable_init_fragment1(1);
}
vec4 IlluminatedMaterial_set_acc_color(OBJREF self, vec4 arg)
{
  {
return ((obj_0x986d218_acc_color)=(arg));
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
    IlluminatedMaterial_init_impl(1, obj_0x986d218_lights, obj_0x986d218_coordinate_system);
  }
}
vec3 Value_inout_0_1_vec3_get_value(OBJREF self)
{
  {
return obj_0x98718b8_value;
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
return Value_inout_0_1_vec3_get_impl(15);
  }
return vec3(0, 0, 0);
}
vec3 ObjectSpace_get_position(OBJREF self)
{
  {
return Gettable_vec3_get1(15);
  }
return vec3(0, 0, 0);
}
vec3 ObjectSpace_get_impl(OBJREF self)
{
return ObjectSpace_get_position(self);
return vec3(0, 0, 0);
}
vec3 Gettable_vec3_get(OBJREF self)
{
  {
return ObjectSpace_get_impl(4);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_get_value1(OBJREF self)
{
  {
return obj_0x98708d8_value;
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
return Value_inout_0_1_vec3_get_impl1(11);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_transformed_position(OBJREF self)
{
  {
return Gettable_vec3_get2(11);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_get_value2(OBJREF self)
{
  {
return obj_0x9871b48_value;
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_get_impl2(OBJREF self)
{
return Value_inout_0_1_vec3_get_value2(self);
}
vec3 Gettable_vec3_get3(OBJREF self)
{
  {
return Value_inout_0_1_vec3_get_impl2(16);
  }
return vec3(0, 0, 0);
}
vec3 ObjectSpace_get_normal(OBJREF self)
{
  {
return Gettable_vec3_get3(16);
  }
return vec3(0, 0, 0);
}
vec3 ObjectSpace_get_transformed_normal_impl(OBJREF self)
{
return normalize(ObjectSpace_get_normal(self));}
vec3 ICoordinateSystem_get_transformed_normal(OBJREF self)
{
  {
return ObjectSpace_get_transformed_normal_impl(4);
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
return IlluminatedMaterial_get_normal_impl(1, obj_0x986d218_lights, obj_0x986d218_coordinate_system);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_get_value3(OBJREF self)
{
  {
return obj_0x9871e50_value;
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_get_impl3(OBJREF self)
{
return Value_inout_0_1_vec3_get_value3(self);
}
vec3 Gettable_vec3_get4(OBJREF self)
{
  {
return Value_inout_0_1_vec3_get_impl3(17);
  }
return vec3(0, 0, 0);
}
vec3 ObjectSpace_get_eyepoint(OBJREF self)
{
  {
return Gettable_vec3_get4(17);
  }
return vec3(0, 0, 0);
}
vec3 ObjectSpace_get_transformed_eyepoint_impl(OBJREF self)
{
return ObjectSpace_get_eyepoint(self);}
vec3 ICoordinateSystem_get_transformed_eyepoint(OBJREF self)
{
  {
return ObjectSpace_get_transformed_eyepoint_impl(4);
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
return IlluminatedMaterial_get_eyepoint_impl(1, obj_0x986d218_lights, obj_0x986d218_coordinate_system);
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
return IlluminatedMaterial_get_position_impl(1, obj_0x986d218_lights, obj_0x986d218_coordinate_system);
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
return Value_uniform_float_get_impl(7, obj_0x986da18_value);
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
return Value_uniform_float_get_impl1(5, obj_0x986d6b8_value);
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
vec4 Gettable_vec4_get(OBJREF self)
{
  {
return Value_uniform_vec4_get_impl(8, obj_0x986dbe0_value);
  }
return vec4(0, 0, 0, 0);
}
vec4 Plastic_get_color(OBJREF self)
{
  {
return Gettable_vec4_get(8);
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
return Value_uniform_float_get_impl2(6, obj_0x986d850_value);
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
return obj_0x986d218_acc_color;
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
    IlluminatedMaterial_accum_color_impl(1, obj_0x986d218_lights, obj_0x986d218_coordinate_system, arg1);
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
vec3 Gettable_vec3_get5(OBJREF self)
{
  {
return Value_uniform_vec3_get_impl(10, obj_0x986e340_value);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_color(OBJREF self)
{
  {
return Gettable_vec3_get5(10);
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
vec3 Value_inout_0_1_vec3_get_value4(OBJREF self)
{
  {
return obj_0x9871210_value;
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_get_impl4(OBJREF self)
{
return Value_inout_0_1_vec3_get_value4(self);
}
vec3 Gettable_vec3_get6(OBJREF self)
{
  {
return Value_inout_0_1_vec3_get_impl4(14);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_transformed_position1(OBJREF self)
{
  {
return Gettable_vec3_get6(14);
  }
return vec3(0, 0, 0);
}
vec3 Value_uniform_vec3_get_impl1(OBJREF self, vec3 value)
{
return value;
}
vec3 Gettable_vec3_get7(OBJREF self)
{
  {
return Value_uniform_vec3_get_impl1(13, obj_0x986e880_value);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_color1(OBJREF self)
{
  {
return Gettable_vec3_get7(13);
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
return IlluminatedMaterial_gather_impl(1, obj_0x986d218_lights, obj_0x986d218_coordinate_system);
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
return IlluminatedMaterial_shade_impl(1, obj_0x986d218_lights, obj_0x986d218_coordinate_system);
  }
return vec4(0, 0, 0, 0);
}
void Surface_fragment_impl(OBJREF self, OBJREF material, OBJREF geometry)
{
gl_FragColor = IMaterial_shade(material);}
void Enterable_enter_fragment(OBJREF self)
{
  {
    Surface_fragment_impl(0, obj_0x986ccb8_material, obj_0x986ccb8_geometry);
  }
}
void main()
{
  shade_initialize();
  Enterable_enter_fragment(0);
}
