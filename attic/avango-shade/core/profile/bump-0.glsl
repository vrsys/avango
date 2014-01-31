#define SHADE_VERTEX_SHADER 1
#define OBJREF int
#define FOREACH(index, list) for(OBJREF index = list.begin; index != list.end; ++index)
struct LIST
{
  int begin;
  int end;
};
const OBJREF obj_0x95dacb0_material = 1;
const LIST obj_0x95db188_lights = LIST(2, 4);
uniform vec3 obj_0x95dbed8_value;
uniform vec3 obj_0x95d9f98_value;
varying vec3 obj_0x95e9e28_value;
uniform vec3 obj_0x95da290_value;
uniform vec3 obj_0x95e7ff0_value;
varying vec3 obj_0x95de9e8_value;
const OBJREF obj_0x95db188_coordinate_system = 4;
varying vec3 obj_0x95df090_value;
varying vec3 obj_0x95df320_value;
vec3 obj_0x95dbc70_tangent;
vec3 obj_0x95dbc70_bitangent;
vec3 obj_0x95dbc70_normal;
varying vec2 obj_0x95dffc8_value;
uniform sampler2D obj_0x95dc2d0_texture_unit;
vec4 obj_0x95db188_acc_color;
uniform float obj_0x95db628_value;
uniform float obj_0x95db7c0_value;
uniform float obj_0x95db988_value;
uniform vec4 obj_0x95dbb50_value;
const OBJREF obj_0x95dacb0_geometry = 0;
vec2 Value_inout_0_1_vec2_set_value(OBJREF self, vec2 arg)
{
  {
return ((obj_0x95dffc8_value)=(arg));
  }
return vec2(0, 0);
}
vec2 Value_inout_0_1_vec2_set_impl(OBJREF self, vec2 arg1)
{
return Value_inout_0_1_vec2_set_value(self, arg1);
}
vec2 Settable_vec2_set(OBJREF self, vec2 arg1)
{
  {
return Value_inout_0_1_vec2_set_impl(19, arg1);
  }
return vec2(0, 0);
}
vec2 UVCoord_set_uv_coord(OBJREF self, vec2 arg)
{
  {
return Settable_vec2_set(19, arg);
  }
return vec2(0, 0);
}
void UVCoord_init_vertex_impl(OBJREF self)
{
UVCoord_set_uv_coord(self, gl_MultiTexCoord0.xy);}
void Initializeable_init_vertex(OBJREF self)
{
  {
    UVCoord_init_vertex_impl(18);
  }
}
vec3 Value_inout_0_1_vec3_set_value(OBJREF self, vec3 arg)
{
  {
return ((obj_0x95df090_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_set_impl(OBJREF self, vec3 arg1)
{
return Value_inout_0_1_vec3_set_value(self, arg1);
}
vec3 Settable_vec3_set(OBJREF self, vec3 arg1)
{
  {
return Value_inout_0_1_vec3_set_impl(15, arg1);
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_set_position(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set(15, arg);
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_set_tangent(OBJREF self, vec3 arg)
{
  {
return ((obj_0x95dbc70_tangent)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_set_bitangent(OBJREF self, vec3 arg)
{
  {
return ((obj_0x95dbc70_bitangent)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_set_normal(OBJREF self, vec3 arg)
{
  {
return ((obj_0x95dbc70_normal)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_tangent(OBJREF self)
{
  {
return obj_0x95dbc70_tangent;
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_bitangent(OBJREF self)
{
  {
return obj_0x95dbc70_bitangent;
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_get_normal(OBJREF self)
{
  {
return obj_0x95dbc70_normal;
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
vec3 Value_inout_0_1_vec3_set_value1(OBJREF self, vec3 arg)
{
  {
return ((obj_0x95df320_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_set_impl1(OBJREF self, vec3 arg1)
{
return Value_inout_0_1_vec3_set_value1(self, arg1);
}
vec3 Settable_vec3_set1(OBJREF self, vec3 arg1)
{
  {
return Value_inout_0_1_vec3_set_impl1(16, arg1);
  }
return vec3(0, 0, 0);
}
vec3 TangentSpace_set_eyepoint(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set1(16, arg);
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
void Initializeable_init_vertex1(OBJREF self)
{
  {
    TangentSpace_init_vertex_impl(4);
  }
}
vec4 ICoordinateSystem_transform(OBJREF self, vec4 arg1)
{
  {
return TangentSpace_transform_impl(4, arg1);
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
return Value_uniform_vec3_get_impl(9, obj_0x95dbed8_value);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_position(OBJREF self)
{
  {
return Gettable_vec3_get(9);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_set_value2(OBJREF self, vec3 arg)
{
  {
return ((obj_0x95e9e28_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_set_impl2(OBJREF self, vec3 arg1)
{
return Value_inout_0_1_vec3_set_value2(self, arg1);
}
vec3 Settable_vec3_set2(OBJREF self, vec3 arg1)
{
  {
return Value_inout_0_1_vec3_set_impl2(11, arg1);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_set_transformed_position(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set2(11, arg);
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
    PointLight_transform_impl(2, arg1);
  }
}
vec3 Value_uniform_vec3_get_impl1(OBJREF self, vec3 value)
{
return value;
}
vec3 Gettable_vec3_get1(OBJREF self)
{
  {
return Value_uniform_vec3_get_impl1(12, obj_0x95da290_value);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_get_position1(OBJREF self)
{
  {
return Gettable_vec3_get1(12);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_set_value3(OBJREF self, vec3 arg)
{
  {
return ((obj_0x95de9e8_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_set_impl3(OBJREF self, vec3 arg1)
{
return Value_inout_0_1_vec3_set_value3(self, arg1);
}
vec3 Settable_vec3_set3(OBJREF self, vec3 arg1)
{
  {
return Value_inout_0_1_vec3_set_impl3(14, arg1);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_set_transformed_position1(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set3(14, arg);
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
    PointLight_transform_impl1(3, arg1);
  }
}
void IlluminatedMaterial_chained_init_vertex(OBJREF self)
{
}
void IlluminatedMaterial_init_vertex_impl(OBJREF self, LIST lights, OBJREF coordinate_system)
{
  {
    OBJREF iter = 2;
ILight_transform(iter, coordinate_system);  }
  {
    OBJREF iter = 3;
ILight_transform1(iter, coordinate_system);  }

  IlluminatedMaterial_chained_init_vertex(self);}
void Initializeable_init_vertex2(OBJREF self)
{
  {
    IlluminatedMaterial_init_vertex_impl(1, obj_0x95db188_lights, obj_0x95db188_coordinate_system);
  }
}
void shade_initialize()
{
  Initializeable_init_vertex(18);
  Initializeable_init_vertex1(4);
  Initializeable_init_vertex2(1);
}
void Surface_vertex_impl(OBJREF self, OBJREF material, OBJREF geometry)
{
gl_Position = ftransform();}
void Enterable_enter_vertex(OBJREF self)
{
  {
    Surface_vertex_impl(0, obj_0x95dacb0_material, obj_0x95dacb0_geometry);
  }
}
void main()
{
  shade_initialize();
  Enterable_enter_vertex(0);
}
