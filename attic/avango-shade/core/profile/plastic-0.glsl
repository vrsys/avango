#define SHADE_VERTEX_SHADER 1
#define OBJREF int
#define FOREACH(index, list) for(OBJREF index = list.begin; index != list.end; ++index)
struct LIST
{
  int begin;
  int end;
};
const OBJREF obj_0x97afcb8_material = 1;
const LIST obj_0x97b0190_lights = LIST(2, 4);
uniform vec3 obj_0x97b10e8_value;
uniform vec3 obj_0x97b12b8_value;
varying vec3 obj_0x97b35f0_value;
uniform vec3 obj_0x97b1628_value;
uniform vec3 obj_0x97b17f8_value;
varying vec3 obj_0x97b3f28_value;
const OBJREF obj_0x97b0190_coordinate_system = 4;
varying vec3 obj_0x97b45d0_value;
varying vec3 obj_0x97b4860_value;
varying vec3 obj_0x97b4b68_value;
vec4 obj_0x97b0190_acc_color;
uniform float obj_0x97b0630_value;
uniform float obj_0x97b07c8_value;
uniform float obj_0x97b0990_value;
uniform vec4 obj_0x97b0b58_value;
const OBJREF obj_0x97afcb8_geometry = 0;
vec3 Value_inout_0_1_vec3_set_value(OBJREF self, vec3 arg)
{
  {
return ((obj_0x97b45d0_value)=(arg));
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
vec3 ObjectSpace_set_position(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set(15, arg);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_set_value1(OBJREF self, vec3 arg)
{
  {
return ((obj_0x97b4860_value)=(arg));
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
vec3 ObjectSpace_set_normal(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set1(16, arg);
  }
return vec3(0, 0, 0);
}
vec4 ObjectSpace_transform_impl(OBJREF self, vec4 v)
{
return gl_ModelViewMatrixInverse * v;}
vec3 Value_inout_0_1_vec3_set_value2(OBJREF self, vec3 arg)
{
  {
return ((obj_0x97b4b68_value)=(arg));
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
return Value_inout_0_1_vec3_set_impl2(17, arg1);
  }
return vec3(0, 0, 0);
}
vec3 ObjectSpace_set_eyepoint(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set2(17, arg);
  }
return vec3(0, 0, 0);
}
void ObjectSpace_init_vertex_impl(OBJREF self)
{
ObjectSpace_set_position(self, gl_Vertex.xyz);
  ObjectSpace_set_normal(self, gl_Normal);
  vec4 eyepoint = ObjectSpace_transform_impl(self, vec4(0., 0., 0., 1.));
  ObjectSpace_set_eyepoint(self, eyepoint.xyz);}
void Initializeable_init_vertex(OBJREF self)
{
  {
    ObjectSpace_init_vertex_impl(4);
  }
}
vec4 ICoordinateSystem_transform(OBJREF self, vec4 arg1)
{
  {
return ObjectSpace_transform_impl(4, arg1);
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
return Value_uniform_vec3_get_impl(9, obj_0x97b10e8_value);
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
vec3 Value_inout_0_1_vec3_set_value3(OBJREF self, vec3 arg)
{
  {
return ((obj_0x97b35f0_value)=(arg));
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
return Value_inout_0_1_vec3_set_impl3(11, arg1);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_set_transformed_position(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set3(11, arg);
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
return Value_uniform_vec3_get_impl1(12, obj_0x97b1628_value);
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
vec3 Value_inout_0_1_vec3_set_value4(OBJREF self, vec3 arg)
{
  {
return ((obj_0x97b3f28_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_1_vec3_set_impl4(OBJREF self, vec3 arg1)
{
return Value_inout_0_1_vec3_set_value4(self, arg1);
}
vec3 Settable_vec3_set4(OBJREF self, vec3 arg1)
{
  {
return Value_inout_0_1_vec3_set_impl4(14, arg1);
  }
return vec3(0, 0, 0);
}
vec3 PointLight_set_transformed_position1(OBJREF self, vec3 arg)
{
  {
return Settable_vec3_set4(14, arg);
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
void Initializeable_init_vertex1(OBJREF self)
{
  {
    IlluminatedMaterial_init_vertex_impl(1, obj_0x97b0190_lights, obj_0x97b0190_coordinate_system);
  }
}
void shade_initialize()
{
  Initializeable_init_vertex(4);
  Initializeable_init_vertex1(1);
}
void Surface_vertex_impl(OBJREF self, OBJREF material, OBJREF geometry)
{
gl_Position = ftransform();}
void Enterable_enter_vertex(OBJREF self)
{
  {
    Surface_vertex_impl(0, obj_0x97afcb8_material, obj_0x97afcb8_geometry);
  }
}
void main()
{
  shade_initialize();
  Enterable_enter_vertex(0);
}
