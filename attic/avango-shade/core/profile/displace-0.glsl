#version 120
#define SHADE_VERTEX_SHADER 1
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
varying vec3 obj_0xa03e000_value;
const OBJREF obj_0xa03e208_geometry = 17;
vec3 obj_0xa03bbb8_value;
varying vec4 obj_0xa03b908_normal;
varying vec4 obj_0xa03b908_light_normal;
vec4 obj_0xa03b908_interpolated_light_normal;
const OBJREF obj_0xa03b908_coordinate_system = 19;
varying vec3 obj_0xa03f9b8_value;
const OBJREF obj_0xa03fbc0_geometry = 29;
varying vec3 obj_0xa040178_value;
const OBJREF obj_0xa0402a0_geometry = 32;
vec3 obj_0xa039cd0_tangent;
vec3 obj_0xa039cd0_bitangent;
vec3 obj_0xa039cd0_normal;
varying vec2 obj_0xa041c50_value;
const OBJREF obj_0xa041e10_geometry = 39;
uniform sampler2D obj_0xa03a330_texture_unit;
uniform sampler2D obj_0xa03b908_texture_unit;
varying vec2 obj_0xa03b908_uvcoord;
uniform vec3 obj_0xa03b550_value;
uniform vec3 obj_0xa03b720_value;
varying vec3 obj_0xa042b50_value;
const OBJREF obj_0xa042d58_geometry = 47;
const OBJREF obj_0xa0390f0_coordinate_system = 5;
vec4 obj_0xa0390f0_acc_color;
uniform float obj_0xa039658_value;
uniform float obj_0xa039820_value;
uniform float obj_0xa0399e8_value;
uniform vec4 obj_0xa039bb0_value;
const OBJREF obj_0xa038f28_geometry = 2;
void Initializeable_init_vertex(OBJREF self)
{
}
void Initializeable_init_vertex1(OBJREF self)
{
}
void Initializeable_init_vertex2(OBJREF self)
{
}
vec2 Value_inout_0_2_vec2_set_value(OBJREF self, vec2 arg)
{
  {
return ((obj_0xa041c50_value)=(arg));
  }
return vec2(0, 0);
}
vec2 Value_inout_0_2_vec2_set_impl(OBJREF self, vec2 arg1)
{
return Value_inout_0_2_vec2_set_value(self, arg1);
}
vec2 Settable_vec2_set(OBJREF self, vec2 arg1)
{
  {
return Value_inout_0_2_vec2_set_impl(34, arg1);
  }
return vec2(0, 0);
}
vec2 UVCoord_set_uv_coord(OBJREF self, vec2 arg)
{
  {
return Settable_vec2_set(34, arg);
  }
return vec2(0, 0);
}
void UVCoord_init_vertex_impl(OBJREF self)
{
UVCoord_set_uv_coord(self, gl_MultiTexCoord0.xy);}
void Initializeable_init_vertex3(OBJREF self)
{
  {
    UVCoord_init_vertex_impl(33);
  }
}
vec3 Value_inout_0_2_vec3_set_value(OBJREF self, vec3 arg)
{
  {
return ((obj_0xa03f9b8_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_set_impl(OBJREF self, vec3 arg1)
{
return Value_inout_0_2_vec3_set_value(self, arg1);
}
vec3 Settable_vec3_set(OBJREF self, vec3 arg1)
{
  {
return Value_inout_0_2_vec3_set_impl(20, arg1);
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_set_position(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set(20, arg);
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_set_tangent(OBJREF self, vec3 arg)
{
  {
return ((obj_0xa039cd0_tangent)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_set_bitangent(OBJREF self, vec3 arg)
{
  {
return ((obj_0xa039cd0_bitangent)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_set_normal(OBJREF self, vec3 arg)
{
  {
return ((obj_0xa039cd0_normal)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_tangent(OBJREF self)
{
  {
return obj_0xa039cd0_tangent;
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_bitangent(OBJREF self)
{
  {
return obj_0xa039cd0_bitangent;
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_normal(OBJREF self)
{
  {
return obj_0xa039cd0_normal;
  }
return vec3(0, 0, 0);
}
vec4 TangentSpace_transform_impl(OBJREF self, vec4 v)
{
vec4 v_object = gl_ModelViewMatrixInverse * v - gl_Vertex;
  vec3 result;
  result.x = dot(v_object.xyz, TangentSpace_get_tangent(self));
  result.y = dot(v_object.xyz, TangentSpace_get_bitangent(self));
  result.z = dot(v_object.xyz, TangentSpace_get_normal(self));
  return vec4(result, 1.);}
vec3 Value_inout_0_2_vec3_set_value1(OBJREF self, vec3 arg)
{
  {
return ((obj_0xa040178_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_set_impl1(OBJREF self, vec3 arg1)
{
return Value_inout_0_2_vec3_set_value1(self, arg1);
}
vec3 Settable_vec3_set1(OBJREF self, vec3 arg1)
{
  {
return Value_inout_0_2_vec3_set_impl1(23, arg1);
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_set_eyepoint(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set1(23, arg);
  }
return vec3(0, 0, 0);
}
void TangentSpace_init_vertex_impl(OBJREF self)
{
TangentSpace_set_position(self, gl_Vertex.xyz);
  TangentSpace_set_tangent(self, gl_MultiTexCoord1.xyz);
  TangentSpace_set_bitangent(self, gl_MultiTexCoord2.xyz);
  TangentSpace_set_normal(self, gl_Normal);
  vec4 eyepoint = TangentSpace_transform_impl(self, vec4(0., 0., 0., 1.));
  TangentSpace_set_eyepoint(self, eyepoint.xyz);}
void Initializeable_init_vertex4(OBJREF self)
{
  {
    TangentSpace_init_vertex_impl(19);
  }
}
vec4 Displacement_set_normal(OBJREF self, vec4 arg)
{
  {
return ((obj_0xa03b908_normal)=(arg));
  }
return vec4(0, 0, 0, 0);
}
vec4 Displacement_set_light_normal(OBJREF self, vec4 arg)
{
  {
return ((obj_0xa03b908_light_normal)=(arg));
  }
return vec4(0, 0, 0, 0);
}
vec2 Displacement_set_uvcoord(OBJREF self, vec2 arg)
{
  {
return ((obj_0xa03b908_uvcoord)=(arg));
  }
return vec2(0, 0);
}
void Displacement_init_vertex_impl(OBJREF self, OBJREF coordinate_system, sampler2D texture_unit)
{
Displacement_set_normal(self, gl_ModelViewProjectionMatrix * vec4(gl_Normal, 0.));
  //Displacement_set_light_normal(self, ICoordinateSystem_transform(coordinate_system, vec4(gl_Normal, 0.)));
  Displacement_set_light_normal(self, vec4(0., 0., 1., 0.));  // FIXME Assume we shade in tangent space ...
  Displacement_set_uvcoord(self, gl_MultiTexCoord0.xy);}
void Initializeable_init_vertex5(OBJREF self)
{
  {
    Displacement_init_vertex_impl(47, obj_0xa03b908_coordinate_system, obj_0xa03b908_texture_unit);
  }
}
void Initializeable_init_vertex6(OBJREF self)
{
}
void Initializeable_init_vertex7(OBJREF self)
{
}
vec3 TangentSpace_get_tangent1(OBJREF self)
{
  {
return obj_0xa039cd0_tangent;
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_bitangent1(OBJREF self)
{
  {
return obj_0xa039cd0_bitangent;
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_normal1(OBJREF self)
{
  {
return obj_0xa039cd0_normal;
  }
return vec3(0, 0, 0);
}
vec4 TangentSpace_transform_impl1(OBJREF self, vec4 v)
{
vec4 v_object = gl_ModelViewMatrixInverse * v - gl_Vertex;
  vec3 result;
  result.x = dot(v_object.xyz, TangentSpace_get_tangent1(self));
  result.y = dot(v_object.xyz, TangentSpace_get_bitangent1(self));
  result.z = dot(v_object.xyz, TangentSpace_get_normal1(self));
  return vec4(result, 1.);}
vec4 ICoordinateSystem_transform(OBJREF self, vec4 arg1)
{
  {
return TangentSpace_transform_impl1(5, arg1);
  }
return vec4(0, 0, 0, 0);
}
vec3 Value_uniform_vec3_get_impl(OBJREF self, vec3 value)
{
return value;
}
vec3 Gettable_vec3_get(OBJREF self)
{
  {
return Value_uniform_vec3_get_impl(10, obj_0xa039f38_value);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_position(OBJREF self)
{
  {
return Gettable_vec3_get(10);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_set_value2(OBJREF self, vec3 arg)
{
  {
return ((obj_0xa03e000_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_set_impl2(OBJREF self, vec3 arg1)
{
return Value_inout_0_2_vec3_set_value2(self, arg1);
}
vec3 Settable_vec3_set2(OBJREF self, vec3 arg1)
{
  {
return Value_inout_0_2_vec3_set_impl2(12, arg1);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_set_transformed_position(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set2(12, arg);
  }
return vec3(0, 0, 0);
}
void PointLight_transform_impl(OBJREF self, OBJREF coordinate_system)
{
vec4 pos = ICoordinateSystem_transform(coordinate_system, vec4(PointLight_get_position(self), 1.));
  PointLight_set_transformed_position(self, pos.xyz);}
void ILight_transform(OBJREF self, OBJREF arg1)
{
  {
    PointLight_transform_impl(3, arg1);
  }
}
vec3 Value_uniform_vec3_get_impl1(OBJREF self, vec3 value)
{
return value;
}
vec3 Gettable_vec3_get1(OBJREF self)
{
  {
return Value_uniform_vec3_get_impl1(40, obj_0xa03b550_value);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_position1(OBJREF self)
{
  {
return Gettable_vec3_get1(40);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_set_value3(OBJREF self, vec3 arg)
{
  {
return ((obj_0xa042b50_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_set_impl3(OBJREF self, vec3 arg1)
{
return Value_inout_0_2_vec3_set_value3(self, arg1);
}
vec3 Settable_vec3_set3(OBJREF self, vec3 arg1)
{
  {
return Value_inout_0_2_vec3_set_impl3(42, arg1);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_set_transformed_position1(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set3(42, arg);
  }
return vec3(0, 0, 0);
}
void PointLight_transform_impl1(OBJREF self, OBJREF coordinate_system)
{
vec4 pos = ICoordinateSystem_transform(coordinate_system, vec4(PointLight_get_position1(self), 1.));
  PointLight_set_transformed_position1(self, pos.xyz);}
void ILight_transform1(OBJREF self, OBJREF arg1)
{
  {
    PointLight_transform_impl1(4, arg1);
  }
}
void IlluminatedMaterial_chained_init_vertex(OBJREF self)
{
}
void IlluminatedMaterial_init_vertex_impl(OBJREF self, LIST lights, OBJREF coordinate_system)
{
  {
    OBJREF iter = 3;
ILight_transform(iter, coordinate_system);  }
  {
    OBJREF iter = 4;
ILight_transform1(iter, coordinate_system);  }

  IlluminatedMaterial_chained_init_vertex(self);}
void Initializeable_init_vertex8(OBJREF self)
{
  {
    IlluminatedMaterial_init_vertex_impl(1, obj_0xa0390f0_lights, obj_0xa0390f0_coordinate_system);
  }
}
void shade_initialize()
{
  Initializeable_init_vertex(22);
  Initializeable_init_vertex1(25);
  Initializeable_init_vertex2(36);
  Initializeable_init_vertex3(33);
  Initializeable_init_vertex4(19);
  Initializeable_init_vertex5(47);
  Initializeable_init_vertex6(14);
  Initializeable_init_vertex7(44);
  Initializeable_init_vertex8(1);
}
void Surface_vertex_impl(OBJREF self, OBJREF material, OBJREF geometry)
{
gl_Position = ftransform();}
void Enterable_enter_vertex(OBJREF self)
{
  {
    Surface_vertex_impl(0, obj_0xa038f28_material, obj_0xa038f28_geometry);
  }
}
void main()
{
  shade_initialize();
  Enterable_enter_vertex(0);
}
