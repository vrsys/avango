#version 120
#extension GL_EXT_geometry_shader4: require
#define SHADE_GEOMETRY_SHADER 1
#define OBJREF int
#define FOREACH(index, list) for(OBJREF index = list.begin; index != list.end; ++index)
struct LIST
{
  int begin;
  int end;
};
int shade_num_classes = 48;
int shade_vertices_in = 3;
const OBJREF obj_0xa038f28_material = 1;
const LIST obj_0xa0390f0_lights = LIST(3, 5);
uniform vec3 obj_0xa039f38_value;
uniform vec3 obj_0xa03b1e0_value;
varying in vec3 obj_0xa03e000_value[3];
varying out vec3 obj_0xa03e100_value;
const OBJREF obj_0xa03e208_geometry = 17;
vec3 obj_0xa03bbb8_0_value;
vec3 obj_0xa03bbb8_1_value;
vec3 obj_0xa03bbb8_2_value;
varying in vec4 obj_0xa03b908_normal[3];
varying in vec4 obj_0xa03b908_light_normal[3];
vec4 obj_0xa03b908_0_interpolated_light_normal;
vec4 obj_0xa03b908_1_interpolated_light_normal;
vec4 obj_0xa03b908_2_interpolated_light_normal;
const OBJREF obj_0xa03b908_coordinate_system = 19;
varying in vec3 obj_0xa03f9b8_value[3];
varying out vec3 obj_0xa03fab8_value;
const OBJREF obj_0xa03fbc0_geometry = 29;
varying in vec3 obj_0xa040178_value[3];
varying out vec3 obj_0xa0401d0_value;
const OBJREF obj_0xa0402a0_geometry = 32;
vec3 obj_0xa039cd0_0_tangent;
vec3 obj_0xa039cd0_1_tangent;
vec3 obj_0xa039cd0_2_tangent;
vec3 obj_0xa039cd0_0_bitangent;
vec3 obj_0xa039cd0_1_bitangent;
vec3 obj_0xa039cd0_2_bitangent;
vec3 obj_0xa039cd0_0_normal;
vec3 obj_0xa039cd0_1_normal;
vec3 obj_0xa039cd0_2_normal;
varying in vec2 obj_0xa041c50_value[3];
varying out vec2 obj_0xa041d10_value;
const OBJREF obj_0xa041e10_geometry = 39;
uniform sampler2D obj_0xa03a330_texture_unit;
uniform sampler2D obj_0xa03b908_texture_unit;
varying in vec2 obj_0xa03b908_uvcoord[3];
uniform vec3 obj_0xa03b550_value;
uniform vec3 obj_0xa03b720_value;
varying in vec3 obj_0xa042b50_value[3];
varying out vec3 obj_0xa042c50_value;
const OBJREF obj_0xa042d58_geometry = 47;
const OBJREF obj_0xa0390f0_coordinate_system = 5;
vec4 obj_0xa0390f0_0_acc_color;
vec4 obj_0xa0390f0_1_acc_color;
vec4 obj_0xa0390f0_2_acc_color;
uniform float obj_0xa039658_value;
uniform float obj_0xa039820_value;
uniform float obj_0xa0399e8_value;
uniform vec4 obj_0xa039bb0_value;
const OBJREF obj_0xa038f28_geometry = 2;
OBJREF shade_get_base_object(OBJREF index)
{
return int(mod(index, 48));
}
void Initializeable_init_geometry(OBJREF self)
{
  OBJREF base_self = shade_get_base_object(self);
}
OBJREF shade_get_base_object1(OBJREF index)
{
return int(mod(index, 48));
}
void Initializeable_init_geometry1(OBJREF self)
{
  OBJREF base_self = shade_get_base_object1(self);
}
OBJREF shade_get_base_object2(OBJREF index)
{
return int(mod(index, 48));
}
void Initializeable_init_geometry2(OBJREF self)
{
  OBJREF base_self = shade_get_base_object2(self);
}
OBJREF shade_get_base_object3(OBJREF index)
{
return int(mod(index, 48));
}
void Initializeable_init_geometry3(OBJREF self)
{
  OBJREF base_self = shade_get_base_object3(self);
}
OBJREF shade_get_base_object4(OBJREF index)
{
return int(mod(index, 48));
}
void Initializeable_init_geometry4(OBJREF self)
{
  OBJREF base_self = shade_get_base_object4(self);
}
OBJREF shade_get_base_object5(OBJREF index)
{
return int(mod(index, 48));
}
void Initializeable_init_geometry5(OBJREF self)
{
  OBJREF base_self = shade_get_base_object5(self);
}
OBJREF shade_get_base_object6(OBJREF index)
{
return int(mod(index, 48));
}
void Initializeable_init_geometry6(OBJREF self)
{
  OBJREF base_self = shade_get_base_object6(self);
}
OBJREF shade_get_base_object7(OBJREF index)
{
return int(mod(index, 48));
}
void Initializeable_init_geometry7(OBJREF self)
{
  OBJREF base_self = shade_get_base_object7(self);
}
OBJREF shade_get_base_object8(OBJREF index)
{
return int(mod(index, 48));
}
void Initializeable_init_geometry8(OBJREF self)
{
  OBJREF base_self = shade_get_base_object8(self);
}
void shade_initialize()
{
  Initializeable_init_geometry(22);
  Initializeable_init_geometry1(25);
  Initializeable_init_geometry2(36);
  Initializeable_init_geometry3(33);
  Initializeable_init_geometry4(19);
  Initializeable_init_geometry5(47);
  Initializeable_init_geometry6(14);
  Initializeable_init_geometry7(44);
  Initializeable_init_geometry8(1);
}
OBJREF shade_get_base_object9(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object10(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object11(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index(OBJREF index)
{
return ((index-shade_get_base_object11(index))/48);
}
vec2 Displacement_get_uvcoord(OBJREF self)
{
  OBJREF base_self = shade_get_base_object11(self);
  OBJREF index = shade_get_object_index(self);
  {
    int shade_comparison_value = base_self;
    if (shade_comparison_value == 2)
    {
return ((obj_0xa03b908_uvcoord)[index]);
    }
    else if (shade_comparison_value == 17)
    {
return ((obj_0xa03b908_uvcoord)[index]);
    }
    else if (shade_comparison_value == 29)
    {
return ((obj_0xa03b908_uvcoord)[index]);
    }
    else if (shade_comparison_value == 32)
    {
return ((obj_0xa03b908_uvcoord)[index]);
    }
    else if (shade_comparison_value == 39)
    {
return ((obj_0xa03b908_uvcoord)[index]);
    }
    else if (shade_comparison_value == 47)
    {
return ((obj_0xa03b908_uvcoord)[index]);
    }
  }
return vec2(0, 0);
}
OBJREF shade_get_object(OBJREF obj, OBJREF index)
{
return (shade_get_base_object10(obj)+(48*index));
}
vec4 Displacement_get_normal(OBJREF self)
{
  OBJREF base_self = shade_get_base_object11(self);
  OBJREF index = shade_get_object_index(self);
  {
    int shade_comparison_value = base_self;
    if (shade_comparison_value == 2)
    {
return ((obj_0xa03b908_normal)[index]);
    }
    else if (shade_comparison_value == 17)
    {
return ((obj_0xa03b908_normal)[index]);
    }
    else if (shade_comparison_value == 29)
    {
return ((obj_0xa03b908_normal)[index]);
    }
    else if (shade_comparison_value == 32)
    {
return ((obj_0xa03b908_normal)[index]);
    }
    else if (shade_comparison_value == 39)
    {
return ((obj_0xa03b908_normal)[index]);
    }
    else if (shade_comparison_value == 47)
    {
return ((obj_0xa03b908_normal)[index]);
    }
  }
return vec4(0, 0, 0, 0);
}
vec4 Displacement_get_light_normal(OBJREF self)
{
  OBJREF base_self = shade_get_base_object11(self);
  OBJREF index = shade_get_object_index(self);
  {
    int shade_comparison_value = base_self;
    if (shade_comparison_value == 2)
    {
return ((obj_0xa03b908_light_normal)[index]);
    }
    else if (shade_comparison_value == 17)
    {
return ((obj_0xa03b908_light_normal)[index]);
    }
    else if (shade_comparison_value == 29)
    {
return ((obj_0xa03b908_light_normal)[index]);
    }
    else if (shade_comparison_value == 32)
    {
return ((obj_0xa03b908_light_normal)[index]);
    }
    else if (shade_comparison_value == 39)
    {
return ((obj_0xa03b908_light_normal)[index]);
    }
    else if (shade_comparison_value == 47)
    {
return ((obj_0xa03b908_light_normal)[index]);
    }
  }
return vec4(0, 0, 0, 0);
}
vec4 Displacement_set_interpolated_light_normal(OBJREF self, vec4 arg)
{
  {
return ((obj_0xa03b908_0_interpolated_light_normal)=(arg));
  }
return vec4(0, 0, 0, 0);
}
OBJREF shade_get_base_object12(OBJREF index)
{
return int(mod(index, 48));
}
vec3 Value_vec3_set_value(OBJREF self, vec3 arg)
{
  {
return ((obj_0xa03bbb8_0_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_vec3_set_impl(OBJREF self, vec3 arg1)
{
return Value_vec3_set_value(self, arg1);
}
vec3 Settable_vec3_set(OBJREF self, vec3 arg1)
{
  OBJREF base_self = shade_get_base_object12(self);
  {
return Value_vec3_set_impl(self, arg1);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index1(OBJREF index)
{
return ((index-shade_get_base_object10(index))/48);
}
OBJREF shade_get_linked_object(OBJREF reference, OBJREF index)
{
return (shade_get_base_object12(index)+(48*shade_get_object_index1(reference)));
}
vec3 Geometry_set_uvw(OBJREF self, vec3 arg)
{
  OBJREF base_self = shade_get_base_object10(self);
  {
return Settable_vec3_set(shade_get_linked_object(self, 18), arg);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_base_object13(OBJREF index)
{
return int(mod(index, 48));
}
vec3 Value_vec3_get_value(OBJREF self)
{
  {
return obj_0xa03bbb8_0_value;
  }
return vec3(0, 0, 0);
}
vec3 Value_vec3_get_impl(OBJREF self)
{
return Value_vec3_get_value(self);
}
vec3 Gettable_vec3_get(OBJREF self)
{
  OBJREF base_self = shade_get_base_object12(self);
  {
return Value_vec3_get_impl(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index2(OBJREF index)
{
return ((index-shade_get_base_object13(index))/48);
}
OBJREF shade_get_linked_object1(OBJREF reference, OBJREF index)
{
return (shade_get_base_object12(index)+(48*shade_get_object_index2(reference)));
}
vec3 Geometry_get_uvw(OBJREF self)
{
  OBJREF base_self = shade_get_base_object13(self);
  {
return Gettable_vec3_get(shade_get_linked_object1(self, 18));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_base_object14(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index3(OBJREF index)
{
return ((index-shade_get_base_object14(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value(OBJREF self)
{
  OBJREF base_self = shade_get_base_object14(self);
  OBJREF index = shade_get_object_index3(self);
  {
return ((obj_0xa03f9b8_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl(OBJREF self)
{
return Value_inout_0_2_vec3_get_value(self);
}
vec3 Gettable_vec3_get1(OBJREF self)
{
  OBJREF base_self = shade_get_base_object14(self);
  {
return Value_inout_0_2_vec3_get_impl(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index4(OBJREF index)
{
return ((index-shade_get_base_object(index))/48);
}
OBJREF shade_get_linked_object2(OBJREF reference, OBJREF index)
{
return (shade_get_base_object14(index)+(48*shade_get_object_index4(reference)));
}
vec3 Interpolator_vec3_get_input(OBJREF self)
{
  OBJREF base_self = shade_get_base_object(self);
  {
return Gettable_vec3_get1(shade_get_linked_object2(self, 27));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object1(OBJREF obj, OBJREF index)
{
return (shade_get_base_object(obj)+(48*index));
}
OBJREF shade_get_base_object15(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object16(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index5(OBJREF index)
{
return ((index-shade_get_base_object16(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value1(OBJREF self)
{
  OBJREF base_self = shade_get_base_object16(self);
  OBJREF index = shade_get_object_index5(self);
  {
return ((obj_0xa03f9b8_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl1(OBJREF self)
{
return Value_inout_0_2_vec3_get_value1(self);
}
vec3 Gettable_vec3_get2(OBJREF self)
{
  OBJREF base_self = shade_get_base_object16(self);
  {
return Value_inout_0_2_vec3_get_impl1(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index6(OBJREF index)
{
return ((index-shade_get_base_object15(index))/48);
}
OBJREF shade_get_linked_object3(OBJREF reference, OBJREF index)
{
return (shade_get_base_object14(index)+(48*shade_get_object_index6(reference)));
}
vec3 Interpolator_vec3_get_input1(OBJREF self)
{
  OBJREF base_self = shade_get_base_object15(self);
  {
return Gettable_vec3_get2(shade_get_linked_object3(self, 27));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object2(OBJREF obj, OBJREF index)
{
return (shade_get_base_object(obj)+(48*index));
}
OBJREF shade_get_base_object17(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object18(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index7(OBJREF index)
{
return ((index-shade_get_base_object18(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value2(OBJREF self)
{
  OBJREF base_self = shade_get_base_object18(self);
  OBJREF index = shade_get_object_index7(self);
  {
return ((obj_0xa03f9b8_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl2(OBJREF self)
{
return Value_inout_0_2_vec3_get_value2(self);
}
vec3 Gettable_vec3_get3(OBJREF self)
{
  OBJREF base_self = shade_get_base_object18(self);
  {
return Value_inout_0_2_vec3_get_impl2(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index8(OBJREF index)
{
return ((index-shade_get_base_object17(index))/48);
}
OBJREF shade_get_linked_object4(OBJREF reference, OBJREF index)
{
return (shade_get_base_object14(index)+(48*shade_get_object_index8(reference)));
}
vec3 Interpolator_vec3_get_input2(OBJREF self)
{
  OBJREF base_self = shade_get_base_object17(self);
  {
return Gettable_vec3_get3(shade_get_linked_object4(self, 27));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object3(OBJREF obj, OBJREF index)
{
return (shade_get_base_object(obj)+(48*index));
}
OBJREF shade_get_base_object19(OBJREF index)
{
return int(mod(index, 48));
}
vec3 Value_inout_2_1_vec3_set_value(OBJREF self, vec3 arg)
{
  OBJREF base_self = shade_get_base_object19(self);
  {
return ((obj_0xa03fab8_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_2_1_vec3_set_impl(OBJREF self, vec3 arg1)
{
return Value_inout_2_1_vec3_set_value(self, arg1);
}
vec3 Settable_vec3_set1(OBJREF self, vec3 arg1)
{
  OBJREF base_self = shade_get_base_object19(self);
  {
return Value_inout_2_1_vec3_set_impl(self, arg1);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_linked_object5(OBJREF reference, OBJREF index)
{
return (shade_get_base_object19(index)+(48*shade_get_object_index4(reference)));
}
vec3 Interpolator_vec3_set_output(OBJREF self, vec3 arg)
{
  OBJREF base_self = shade_get_base_object(self);
  {
return Settable_vec3_set1(shade_get_linked_object5(self, 28), arg);
  }
return vec3(0, 0, 0);
}
void Interpolator_vec3_init_post_geometry_impl(OBJREF self, OBJREF geometry)
{
  vec3 uvw = Geometry_get_uvw(geometry);
  vec3 value = ((uvw).x*Interpolator_vec3_get_input(shade_get_object1(self, 0)));
  value+=((uvw).y*Interpolator_vec3_get_input1(shade_get_object2(self, 1)));
  value+=((uvw).z*Interpolator_vec3_get_input2(shade_get_object3(self, 2)));
  Interpolator_vec3_set_output(self, value);
}
void Initializeable_init_post_geometry(OBJREF self)
{
  OBJREF base_self = shade_get_base_object(self);
  {
    Interpolator_vec3_init_post_geometry_impl(self, obj_0xa03fbc0_geometry);
  }
}
OBJREF shade_get_base_object20(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index9(OBJREF index)
{
return ((index-shade_get_base_object20(index))/48);
}
OBJREF shade_get_linked_object6(OBJREF reference, OBJREF index)
{
return (shade_get_base_object12(index)+(48*shade_get_object_index9(reference)));
}
vec3 Geometry_get_uvw1(OBJREF self)
{
  OBJREF base_self = shade_get_base_object20(self);
  {
return Gettable_vec3_get(shade_get_linked_object6(self, 18));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_base_object21(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index10(OBJREF index)
{
return ((index-shade_get_base_object21(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value3(OBJREF self)
{
  OBJREF base_self = shade_get_base_object21(self);
  OBJREF index = shade_get_object_index10(self);
  {
return ((obj_0xa040178_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl3(OBJREF self)
{
return Value_inout_0_2_vec3_get_value3(self);
}
vec3 Gettable_vec3_get4(OBJREF self)
{
  OBJREF base_self = shade_get_base_object21(self);
  {
return Value_inout_0_2_vec3_get_impl3(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index11(OBJREF index)
{
return ((index-shade_get_base_object1(index))/48);
}
OBJREF shade_get_linked_object7(OBJREF reference, OBJREF index)
{
return (shade_get_base_object21(index)+(48*shade_get_object_index11(reference)));
}
vec3 InverseTransformInterpolator_vec3_get_input(OBJREF self)
{
  OBJREF base_self = shade_get_base_object1(self);
  {
return Gettable_vec3_get4(shade_get_linked_object7(self, 30));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object4(OBJREF obj, OBJREF index)
{
return (shade_get_base_object1(obj)+(48*index));
}
OBJREF shade_get_base_object22(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object23(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index12(OBJREF index)
{
return ((index-shade_get_base_object23(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value4(OBJREF self)
{
  OBJREF base_self = shade_get_base_object23(self);
  OBJREF index = shade_get_object_index12(self);
  {
return ((obj_0xa040178_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl4(OBJREF self)
{
return Value_inout_0_2_vec3_get_value4(self);
}
vec3 Gettable_vec3_get5(OBJREF self)
{
  OBJREF base_self = shade_get_base_object23(self);
  {
return Value_inout_0_2_vec3_get_impl4(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index13(OBJREF index)
{
return ((index-shade_get_base_object22(index))/48);
}
OBJREF shade_get_linked_object8(OBJREF reference, OBJREF index)
{
return (shade_get_base_object21(index)+(48*shade_get_object_index13(reference)));
}
vec3 InverseTransformInterpolator_vec3_get_input1(OBJREF self)
{
  OBJREF base_self = shade_get_base_object22(self);
  {
return Gettable_vec3_get5(shade_get_linked_object8(self, 30));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object5(OBJREF obj, OBJREF index)
{
return (shade_get_base_object1(obj)+(48*index));
}
OBJREF shade_get_base_object24(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object25(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index14(OBJREF index)
{
return ((index-shade_get_base_object25(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value5(OBJREF self)
{
  OBJREF base_self = shade_get_base_object25(self);
  OBJREF index = shade_get_object_index14(self);
  {
return ((obj_0xa040178_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl5(OBJREF self)
{
return Value_inout_0_2_vec3_get_value5(self);
}
vec3 Gettable_vec3_get6(OBJREF self)
{
  OBJREF base_self = shade_get_base_object25(self);
  {
return Value_inout_0_2_vec3_get_impl5(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index15(OBJREF index)
{
return ((index-shade_get_base_object24(index))/48);
}
OBJREF shade_get_linked_object9(OBJREF reference, OBJREF index)
{
return (shade_get_base_object21(index)+(48*shade_get_object_index15(reference)));
}
vec3 InverseTransformInterpolator_vec3_get_input2(OBJREF self)
{
  OBJREF base_self = shade_get_base_object24(self);
  {
return Gettable_vec3_get6(shade_get_linked_object9(self, 30));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object6(OBJREF obj, OBJREF index)
{
return (shade_get_base_object1(obj)+(48*index));
}
vec4 Displacement_get_interpolated_light_normal(OBJREF self)
{
  {
return obj_0xa03b908_0_interpolated_light_normal;
  }
return vec4(0, 0, 0, 0);
}
vec3 Displacement_transform_inverse_impl(OBJREF self, OBJREF coordinate_system, sampler2D texture_unit, vec3 value)
{
return value - Displacement_get_interpolated_light_normal(self).xyz;}
vec3 Geometry_transform_inverse(OBJREF self, vec3 arg1)
{
  OBJREF base_self = shade_get_base_object20(self);
  {
return Displacement_transform_inverse_impl(self, obj_0xa03b908_coordinate_system, obj_0xa03b908_texture_unit, arg1);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_base_object26(OBJREF index)
{
return int(mod(index, 48));
}
vec3 Value_inout_2_1_vec3_set_value1(OBJREF self, vec3 arg)
{
  OBJREF base_self = shade_get_base_object26(self);
  {
return ((obj_0xa0401d0_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_2_1_vec3_set_impl1(OBJREF self, vec3 arg1)
{
return Value_inout_2_1_vec3_set_value1(self, arg1);
}
vec3 Settable_vec3_set2(OBJREF self, vec3 arg1)
{
  OBJREF base_self = shade_get_base_object26(self);
  {
return Value_inout_2_1_vec3_set_impl1(self, arg1);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_linked_object10(OBJREF reference, OBJREF index)
{
return (shade_get_base_object26(index)+(48*shade_get_object_index11(reference)));
}
vec3 InverseTransformInterpolator_vec3_set_output(OBJREF self, vec3 arg)
{
  OBJREF base_self = shade_get_base_object1(self);
  {
return Settable_vec3_set2(shade_get_linked_object10(self, 31), arg);
  }
return vec3(0, 0, 0);
}
void InverseTransformInterpolator_vec3_init_post_geometry_impl(OBJREF self, OBJREF geometry)
{
  vec3 uvw = Geometry_get_uvw1(geometry);
  vec3 value = ((uvw).x*InverseTransformInterpolator_vec3_get_input(shade_get_object4(self, 0)));
  value+=((uvw).y*InverseTransformInterpolator_vec3_get_input1(shade_get_object5(self, 1)));
  value+=((uvw).z*InverseTransformInterpolator_vec3_get_input2(shade_get_object6(self, 2)));
  ((value)=(Geometry_transform_inverse(geometry, value)));
  InverseTransformInterpolator_vec3_set_output(self, value);
}
void Initializeable_init_post_geometry1(OBJREF self)
{
  OBJREF base_self = shade_get_base_object1(self);
  {
    InverseTransformInterpolator_vec3_init_post_geometry_impl(self, obj_0xa0402a0_geometry);
  }
}
OBJREF shade_get_base_object27(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index16(OBJREF index)
{
return ((index-shade_get_base_object27(index))/48);
}
OBJREF shade_get_linked_object11(OBJREF reference, OBJREF index)
{
return (shade_get_base_object12(index)+(48*shade_get_object_index16(reference)));
}
vec3 Geometry_get_uvw2(OBJREF self)
{
  OBJREF base_self = shade_get_base_object27(self);
  {
return Gettable_vec3_get(shade_get_linked_object11(self, 18));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_base_object28(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index17(OBJREF index)
{
return ((index-shade_get_base_object28(index))/48);
}
vec2 Value_inout_0_2_vec2_get_value(OBJREF self)
{
  OBJREF base_self = shade_get_base_object28(self);
  OBJREF index = shade_get_object_index17(self);
  {
return ((obj_0xa041c50_value)[index]);
  }
return vec2(0, 0);
}
vec2 Value_inout_0_2_vec2_get_impl(OBJREF self)
{
return Value_inout_0_2_vec2_get_value(self);
}
vec2 Gettable_vec2_get(OBJREF self)
{
  OBJREF base_self = shade_get_base_object28(self);
  {
return Value_inout_0_2_vec2_get_impl(self);
  }
return vec2(0, 0);
}
OBJREF shade_get_object_index18(OBJREF index)
{
return ((index-shade_get_base_object2(index))/48);
}
OBJREF shade_get_linked_object12(OBJREF reference, OBJREF index)
{
return (shade_get_base_object28(index)+(48*shade_get_object_index18(reference)));
}
vec2 Interpolator_vec2_get_input(OBJREF self)
{
  OBJREF base_self = shade_get_base_object2(self);
  {
return Gettable_vec2_get(shade_get_linked_object12(self, 37));
  }
return vec2(0, 0);
}
OBJREF shade_get_object7(OBJREF obj, OBJREF index)
{
return (shade_get_base_object2(obj)+(48*index));
}
OBJREF shade_get_base_object29(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object30(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index19(OBJREF index)
{
return ((index-shade_get_base_object30(index))/48);
}
vec2 Value_inout_0_2_vec2_get_value1(OBJREF self)
{
  OBJREF base_self = shade_get_base_object30(self);
  OBJREF index = shade_get_object_index19(self);
  {
return ((obj_0xa041c50_value)[index]);
  }
return vec2(0, 0);
}
vec2 Value_inout_0_2_vec2_get_impl1(OBJREF self)
{
return Value_inout_0_2_vec2_get_value1(self);
}
vec2 Gettable_vec2_get1(OBJREF self)
{
  OBJREF base_self = shade_get_base_object30(self);
  {
return Value_inout_0_2_vec2_get_impl1(self);
  }
return vec2(0, 0);
}
OBJREF shade_get_object_index20(OBJREF index)
{
return ((index-shade_get_base_object29(index))/48);
}
OBJREF shade_get_linked_object13(OBJREF reference, OBJREF index)
{
return (shade_get_base_object28(index)+(48*shade_get_object_index20(reference)));
}
vec2 Interpolator_vec2_get_input1(OBJREF self)
{
  OBJREF base_self = shade_get_base_object29(self);
  {
return Gettable_vec2_get1(shade_get_linked_object13(self, 37));
  }
return vec2(0, 0);
}
OBJREF shade_get_object8(OBJREF obj, OBJREF index)
{
return (shade_get_base_object2(obj)+(48*index));
}
OBJREF shade_get_base_object31(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object32(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index21(OBJREF index)
{
return ((index-shade_get_base_object32(index))/48);
}
vec2 Value_inout_0_2_vec2_get_value2(OBJREF self)
{
  OBJREF base_self = shade_get_base_object32(self);
  OBJREF index = shade_get_object_index21(self);
  {
return ((obj_0xa041c50_value)[index]);
  }
return vec2(0, 0);
}
vec2 Value_inout_0_2_vec2_get_impl2(OBJREF self)
{
return Value_inout_0_2_vec2_get_value2(self);
}
vec2 Gettable_vec2_get2(OBJREF self)
{
  OBJREF base_self = shade_get_base_object32(self);
  {
return Value_inout_0_2_vec2_get_impl2(self);
  }
return vec2(0, 0);
}
OBJREF shade_get_object_index22(OBJREF index)
{
return ((index-shade_get_base_object31(index))/48);
}
OBJREF shade_get_linked_object14(OBJREF reference, OBJREF index)
{
return (shade_get_base_object28(index)+(48*shade_get_object_index22(reference)));
}
vec2 Interpolator_vec2_get_input2(OBJREF self)
{
  OBJREF base_self = shade_get_base_object31(self);
  {
return Gettable_vec2_get2(shade_get_linked_object14(self, 37));
  }
return vec2(0, 0);
}
OBJREF shade_get_object9(OBJREF obj, OBJREF index)
{
return (shade_get_base_object2(obj)+(48*index));
}
OBJREF shade_get_base_object33(OBJREF index)
{
return int(mod(index, 48));
}
vec2 Value_inout_2_1_vec2_set_value(OBJREF self, vec2 arg)
{
  OBJREF base_self = shade_get_base_object33(self);
  {
return ((obj_0xa041d10_value)=(arg));
  }
return vec2(0, 0);
}
vec2 Value_inout_2_1_vec2_set_impl(OBJREF self, vec2 arg1)
{
return Value_inout_2_1_vec2_set_value(self, arg1);
}
vec2 Settable_vec2_set(OBJREF self, vec2 arg1)
{
  OBJREF base_self = shade_get_base_object33(self);
  {
return Value_inout_2_1_vec2_set_impl(self, arg1);
  }
return vec2(0, 0);
}
OBJREF shade_get_linked_object15(OBJREF reference, OBJREF index)
{
return (shade_get_base_object33(index)+(48*shade_get_object_index18(reference)));
}
vec2 Interpolator_vec2_set_output(OBJREF self, vec2 arg)
{
  OBJREF base_self = shade_get_base_object2(self);
  {
return Settable_vec2_set(shade_get_linked_object15(self, 38), arg);
  }
return vec2(0, 0);
}
void Interpolator_vec2_init_post_geometry_impl(OBJREF self, OBJREF geometry)
{
  vec3 uvw = Geometry_get_uvw2(geometry);
  vec2 value = ((uvw).x*Interpolator_vec2_get_input(shade_get_object7(self, 0)));
  value+=((uvw).y*Interpolator_vec2_get_input1(shade_get_object8(self, 1)));
  value+=((uvw).z*Interpolator_vec2_get_input2(shade_get_object9(self, 2)));
  Interpolator_vec2_set_output(self, value);
}
void Initializeable_init_post_geometry2(OBJREF self)
{
  OBJREF base_self = shade_get_base_object2(self);
  {
    Interpolator_vec2_init_post_geometry_impl(self, obj_0xa041e10_geometry);
  }
}
void Initializeable_init_post_geometry3(OBJREF self)
{
  OBJREF base_self = shade_get_base_object3(self);
}
void Initializeable_init_post_geometry4(OBJREF self)
{
  OBJREF base_self = shade_get_base_object4(self);
}
void Initializeable_init_post_geometry5(OBJREF self)
{
  OBJREF base_self = shade_get_base_object5(self);
}
OBJREF shade_get_base_object34(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index23(OBJREF index)
{
return ((index-shade_get_base_object34(index))/48);
}
OBJREF shade_get_linked_object16(OBJREF reference, OBJREF index)
{
return (shade_get_base_object12(index)+(48*shade_get_object_index23(reference)));
}
vec3 Geometry_get_uvw3(OBJREF self)
{
  OBJREF base_self = shade_get_base_object34(self);
  {
return Gettable_vec3_get(shade_get_linked_object16(self, 18));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_base_object35(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index24(OBJREF index)
{
return ((index-shade_get_base_object35(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value6(OBJREF self)
{
  OBJREF base_self = shade_get_base_object35(self);
  OBJREF index = shade_get_object_index24(self);
  {
return ((obj_0xa03e000_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl6(OBJREF self)
{
return Value_inout_0_2_vec3_get_value6(self);
}
vec3 Gettable_vec3_get7(OBJREF self)
{
  OBJREF base_self = shade_get_base_object35(self);
  {
return Value_inout_0_2_vec3_get_impl6(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index25(OBJREF index)
{
return ((index-shade_get_base_object6(index))/48);
}
OBJREF shade_get_linked_object17(OBJREF reference, OBJREF index)
{
return (shade_get_base_object35(index)+(48*shade_get_object_index25(reference)));
}
vec3 InverseTransformInterpolator_vec3_get_input3(OBJREF self)
{
  OBJREF base_self = shade_get_base_object6(self);
  {
return Gettable_vec3_get7(shade_get_linked_object17(self, 15));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object10(OBJREF obj, OBJREF index)
{
return (shade_get_base_object6(obj)+(48*index));
}
OBJREF shade_get_base_object36(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object37(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index26(OBJREF index)
{
return ((index-shade_get_base_object37(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value7(OBJREF self)
{
  OBJREF base_self = shade_get_base_object37(self);
  OBJREF index = shade_get_object_index26(self);
  {
return ((obj_0xa03e000_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl7(OBJREF self)
{
return Value_inout_0_2_vec3_get_value7(self);
}
vec3 Gettable_vec3_get8(OBJREF self)
{
  OBJREF base_self = shade_get_base_object37(self);
  {
return Value_inout_0_2_vec3_get_impl7(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index27(OBJREF index)
{
return ((index-shade_get_base_object36(index))/48);
}
OBJREF shade_get_linked_object18(OBJREF reference, OBJREF index)
{
return (shade_get_base_object35(index)+(48*shade_get_object_index27(reference)));
}
vec3 InverseTransformInterpolator_vec3_get_input4(OBJREF self)
{
  OBJREF base_self = shade_get_base_object36(self);
  {
return Gettable_vec3_get8(shade_get_linked_object18(self, 15));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object11(OBJREF obj, OBJREF index)
{
return (shade_get_base_object6(obj)+(48*index));
}
OBJREF shade_get_base_object38(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object39(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index28(OBJREF index)
{
return ((index-shade_get_base_object39(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value8(OBJREF self)
{
  OBJREF base_self = shade_get_base_object39(self);
  OBJREF index = shade_get_object_index28(self);
  {
return ((obj_0xa03e000_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl8(OBJREF self)
{
return Value_inout_0_2_vec3_get_value8(self);
}
vec3 Gettable_vec3_get9(OBJREF self)
{
  OBJREF base_self = shade_get_base_object39(self);
  {
return Value_inout_0_2_vec3_get_impl8(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index29(OBJREF index)
{
return ((index-shade_get_base_object38(index))/48);
}
OBJREF shade_get_linked_object19(OBJREF reference, OBJREF index)
{
return (shade_get_base_object35(index)+(48*shade_get_object_index29(reference)));
}
vec3 InverseTransformInterpolator_vec3_get_input5(OBJREF self)
{
  OBJREF base_self = shade_get_base_object38(self);
  {
return Gettable_vec3_get9(shade_get_linked_object19(self, 15));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object12(OBJREF obj, OBJREF index)
{
return (shade_get_base_object6(obj)+(48*index));
}
vec4 Displacement_get_interpolated_light_normal1(OBJREF self)
{
  {
return obj_0xa03b908_0_interpolated_light_normal;
  }
return vec4(0, 0, 0, 0);
}
vec3 Displacement_transform_inverse_impl1(OBJREF self, OBJREF coordinate_system, sampler2D texture_unit, vec3 value)
{
return value - Displacement_get_interpolated_light_normal1(self).xyz;}
vec3 Geometry_transform_inverse1(OBJREF self, vec3 arg1)
{
  OBJREF base_self = shade_get_base_object34(self);
  {
return Displacement_transform_inverse_impl1(self, obj_0xa03b908_coordinate_system, obj_0xa03b908_texture_unit, arg1);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_base_object40(OBJREF index)
{
return int(mod(index, 48));
}
vec3 Value_inout_2_1_vec3_set_value2(OBJREF self, vec3 arg)
{
  OBJREF base_self = shade_get_base_object40(self);
  {
return ((obj_0xa03e100_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_2_1_vec3_set_impl2(OBJREF self, vec3 arg1)
{
return Value_inout_2_1_vec3_set_value2(self, arg1);
}
vec3 Settable_vec3_set3(OBJREF self, vec3 arg1)
{
  OBJREF base_self = shade_get_base_object40(self);
  {
return Value_inout_2_1_vec3_set_impl2(self, arg1);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_linked_object20(OBJREF reference, OBJREF index)
{
return (shade_get_base_object40(index)+(48*shade_get_object_index25(reference)));
}
vec3 InverseTransformInterpolator_vec3_set_output1(OBJREF self, vec3 arg)
{
  OBJREF base_self = shade_get_base_object6(self);
  {
return Settable_vec3_set3(shade_get_linked_object20(self, 16), arg);
  }
return vec3(0, 0, 0);
}
void InverseTransformInterpolator_vec3_init_post_geometry_impl1(OBJREF self, OBJREF geometry)
{
  vec3 uvw = Geometry_get_uvw3(geometry);
  vec3 value = ((uvw).x*InverseTransformInterpolator_vec3_get_input3(shade_get_object10(self, 0)));
  value+=((uvw).y*InverseTransformInterpolator_vec3_get_input4(shade_get_object11(self, 1)));
  value+=((uvw).z*InverseTransformInterpolator_vec3_get_input5(shade_get_object12(self, 2)));
  ((value)=(Geometry_transform_inverse1(geometry, value)));
  InverseTransformInterpolator_vec3_set_output1(self, value);
}
void Initializeable_init_post_geometry6(OBJREF self)
{
  OBJREF base_self = shade_get_base_object6(self);
  {
    InverseTransformInterpolator_vec3_init_post_geometry_impl1(self, obj_0xa03e208_geometry);
  }
}
OBJREF shade_get_object_index30(OBJREF index)
{
return ((index-shade_get_base_object5(index))/48);
}
OBJREF shade_get_linked_object21(OBJREF reference, OBJREF index)
{
return (shade_get_base_object12(index)+(48*shade_get_object_index30(reference)));
}
vec3 Geometry_get_uvw4(OBJREF self)
{
  OBJREF base_self = shade_get_base_object5(self);
  {
return Gettable_vec3_get(shade_get_linked_object21(self, 18));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_base_object41(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index31(OBJREF index)
{
return ((index-shade_get_base_object41(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value9(OBJREF self)
{
  OBJREF base_self = shade_get_base_object41(self);
  OBJREF index = shade_get_object_index31(self);
  {
return ((obj_0xa042b50_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl9(OBJREF self)
{
return Value_inout_0_2_vec3_get_value9(self);
}
vec3 Gettable_vec3_get10(OBJREF self)
{
  OBJREF base_self = shade_get_base_object41(self);
  {
return Value_inout_0_2_vec3_get_impl9(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index32(OBJREF index)
{
return ((index-shade_get_base_object7(index))/48);
}
OBJREF shade_get_linked_object22(OBJREF reference, OBJREF index)
{
return (shade_get_base_object41(index)+(48*shade_get_object_index32(reference)));
}
vec3 InverseTransformInterpolator_vec3_get_input6(OBJREF self)
{
  OBJREF base_self = shade_get_base_object7(self);
  {
return Gettable_vec3_get10(shade_get_linked_object22(self, 45));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object13(OBJREF obj, OBJREF index)
{
return (shade_get_base_object7(obj)+(48*index));
}
OBJREF shade_get_base_object42(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object43(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index33(OBJREF index)
{
return ((index-shade_get_base_object43(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value10(OBJREF self)
{
  OBJREF base_self = shade_get_base_object43(self);
  OBJREF index = shade_get_object_index33(self);
  {
return ((obj_0xa042b50_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl10(OBJREF self)
{
return Value_inout_0_2_vec3_get_value10(self);
}
vec3 Gettable_vec3_get11(OBJREF self)
{
  OBJREF base_self = shade_get_base_object43(self);
  {
return Value_inout_0_2_vec3_get_impl10(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index34(OBJREF index)
{
return ((index-shade_get_base_object42(index))/48);
}
OBJREF shade_get_linked_object23(OBJREF reference, OBJREF index)
{
return (shade_get_base_object41(index)+(48*shade_get_object_index34(reference)));
}
vec3 InverseTransformInterpolator_vec3_get_input7(OBJREF self)
{
  OBJREF base_self = shade_get_base_object42(self);
  {
return Gettable_vec3_get11(shade_get_linked_object23(self, 45));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object14(OBJREF obj, OBJREF index)
{
return (shade_get_base_object7(obj)+(48*index));
}
OBJREF shade_get_base_object44(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_base_object45(OBJREF index)
{
return int(mod(index, 48));
}
OBJREF shade_get_object_index35(OBJREF index)
{
return ((index-shade_get_base_object45(index))/48);
}
vec3 Value_inout_0_2_vec3_get_value11(OBJREF self)
{
  OBJREF base_self = shade_get_base_object45(self);
  OBJREF index = shade_get_object_index35(self);
  {
return ((obj_0xa042b50_value)[index]);
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_0_2_vec3_get_impl11(OBJREF self)
{
return Value_inout_0_2_vec3_get_value11(self);
}
vec3 Gettable_vec3_get12(OBJREF self)
{
  OBJREF base_self = shade_get_base_object45(self);
  {
return Value_inout_0_2_vec3_get_impl11(self);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object_index36(OBJREF index)
{
return ((index-shade_get_base_object44(index))/48);
}
OBJREF shade_get_linked_object24(OBJREF reference, OBJREF index)
{
return (shade_get_base_object41(index)+(48*shade_get_object_index36(reference)));
}
vec3 InverseTransformInterpolator_vec3_get_input8(OBJREF self)
{
  OBJREF base_self = shade_get_base_object44(self);
  {
return Gettable_vec3_get12(shade_get_linked_object24(self, 45));
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_object15(OBJREF obj, OBJREF index)
{
return (shade_get_base_object7(obj)+(48*index));
}
vec4 Displacement_get_interpolated_light_normal2(OBJREF self)
{
  {
return obj_0xa03b908_0_interpolated_light_normal;
  }
return vec4(0, 0, 0, 0);
}
vec3 Displacement_transform_inverse_impl2(OBJREF self, OBJREF coordinate_system, sampler2D texture_unit, vec3 value)
{
return value - Displacement_get_interpolated_light_normal2(self).xyz;}
vec3 Geometry_transform_inverse2(OBJREF self, vec3 arg1)
{
  OBJREF base_self = shade_get_base_object5(self);
  {
return Displacement_transform_inverse_impl2(self, obj_0xa03b908_coordinate_system, obj_0xa03b908_texture_unit, arg1);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_base_object46(OBJREF index)
{
return int(mod(index, 48));
}
vec3 Value_inout_2_1_vec3_set_value3(OBJREF self, vec3 arg)
{
  OBJREF base_self = shade_get_base_object46(self);
  {
return ((obj_0xa042c50_value)=(arg));
  }
return vec3(0, 0, 0);
}
vec3 Value_inout_2_1_vec3_set_impl3(OBJREF self, vec3 arg1)
{
return Value_inout_2_1_vec3_set_value3(self, arg1);
}
vec3 Settable_vec3_set4(OBJREF self, vec3 arg1)
{
  OBJREF base_self = shade_get_base_object46(self);
  {
return Value_inout_2_1_vec3_set_impl3(self, arg1);
  }
return vec3(0, 0, 0);
}
OBJREF shade_get_linked_object25(OBJREF reference, OBJREF index)
{
return (shade_get_base_object46(index)+(48*shade_get_object_index32(reference)));
}
vec3 InverseTransformInterpolator_vec3_set_output2(OBJREF self, vec3 arg)
{
  OBJREF base_self = shade_get_base_object7(self);
  {
return Settable_vec3_set4(shade_get_linked_object25(self, 46), arg);
  }
return vec3(0, 0, 0);
}
void InverseTransformInterpolator_vec3_init_post_geometry_impl2(OBJREF self, OBJREF geometry)
{
  vec3 uvw = Geometry_get_uvw4(geometry);
  vec3 value = ((uvw).x*InverseTransformInterpolator_vec3_get_input6(shade_get_object13(self, 0)));
  value+=((uvw).y*InverseTransformInterpolator_vec3_get_input7(shade_get_object14(self, 1)));
  value+=((uvw).z*InverseTransformInterpolator_vec3_get_input8(shade_get_object15(self, 2)));
  ((value)=(Geometry_transform_inverse2(geometry, value)));
  InverseTransformInterpolator_vec3_set_output2(self, value);
}
void Initializeable_init_post_geometry7(OBJREF self)
{
  OBJREF base_self = shade_get_base_object7(self);
  {
    InverseTransformInterpolator_vec3_init_post_geometry_impl2(self, obj_0xa042d58_geometry);
  }
}
void Initializeable_init_post_geometry8(OBJREF self)
{
  OBJREF base_self = shade_get_base_object8(self);
}
void shade_initialize_post()
{
  Initializeable_init_post_geometry(22);
  Initializeable_init_post_geometry1(25);
  Initializeable_init_post_geometry2(36);
  Initializeable_init_post_geometry3(33);
  Initializeable_init_post_geometry4(19);
  Initializeable_init_post_geometry5(47);
  Initializeable_init_post_geometry6(14);
  Initializeable_init_post_geometry7(44);
  Initializeable_init_post_geometry8(1);
}
void shade_emit_vertex()
{
  shade_initialize_post();
  EmitVertex();
}
void Displacement_interpolate_impl(OBJREF self, OBJREF coordinate_system, sampler2D texture_unit, vec3 factor)
{
vec2 uvcoord_0 = Displacement_get_uvcoord(shade_get_object(self, 0));
  vec2 uvcoord_1 = Displacement_get_uvcoord(shade_get_object(self, 1));
  vec2 uvcoord_2 = Displacement_get_uvcoord(shade_get_object(self, 2));
  float offset = 0.05 * (texture2D(texture_unit, factor.x*uvcoord_0 + factor.y*uvcoord_1 + factor.z*uvcoord_2).a - 1.);
  vec4 position = factor.x*gl_PositionIn[0] + factor.y*gl_PositionIn[1] + factor.z*gl_PositionIn[2];
  vec4 normal_0 = Displacement_get_normal(shade_get_object(self, 0));
  vec4 normal_1 = Displacement_get_normal(shade_get_object(self, 1));
  vec4 normal_2 = Displacement_get_normal(shade_get_object(self, 2));
  vec4 normal = factor.x*normal_0 + factor.y*normal_1 + factor.z*normal_2;
  gl_Position = position + offset* normal;
  vec4 light_normal_0 = Displacement_get_light_normal(shade_get_object(self, 0));
  vec4 light_normal_1 = Displacement_get_light_normal(shade_get_object(self, 1));
  vec4 light_normal_2 = Displacement_get_light_normal(shade_get_object(self, 2));
  vec4 light_normal = factor.x*light_normal_0 + factor.y*light_normal_1 + factor.z*light_normal_2;
  light_normal *= offset;
  Displacement_set_interpolated_light_normal(self, light_normal);
  Geometry_set_uvw(self, factor);
  shade_emit_vertex();}
void Displacement_interpolate(OBJREF self, vec3 arg1)
{
  OBJREF base_self = shade_get_base_object10(self);
  {
    Displacement_interpolate_impl(self, obj_0xa03b908_coordinate_system, obj_0xa03b908_texture_unit, arg1);
  }
}
void Displacement_enter_impl(OBJREF self, OBJREF coordinate_system, sampler2D texture_unit)
{
const int segments = 8;
  float interpolations[segments];
  for (int i = 0; i != segments; ++i)
   {
    interpolations[i] = float(i)/float(segments-1);
   }
  Displacement_interpolate(self, vec3(1., 0., 0.));
  Displacement_interpolate(self, vec3(interpolations[segments-2], interpolations[1], 0.));
  Displacement_interpolate(self, vec3(interpolations[segments-2], 0., interpolations[1]));
  for (int i = 2; i < segments-1; ++i)
   {
    float factor = float(i-1)/float(segments-2);
    Displacement_interpolate(self, vec3((1.-factor)*interpolations[segments-2], interpolations[1], factor*interpolations[segments-2]));
    Displacement_interpolate(self, vec3(interpolations[segments-i-1], 0., interpolations[i]));
   }
  Displacement_interpolate(self, vec3(0., interpolations[1], interpolations[segments-2]));
  Displacement_interpolate(self, vec3(0., 0., 1.));
  EndPrimitive();
  for (int j = 1; j < segments-2; ++j)
   {
    Displacement_interpolate(self, vec3(interpolations[segments-j-1], interpolations[j], 0.));
    Displacement_interpolate(self, vec3(interpolations[segments-j-2], interpolations[j+1], 0.));
    float factor = 1./float(segments-j-1);
    Displacement_interpolate(self, vec3((1.-factor)*interpolations[segments-j-1], interpolations[j], factor*interpolations[segments-j-1]));
    for (int i = 2; i < segments-j-1; ++i)
     {
      factor = float(i-1)/float(segments-j-2);
      Displacement_interpolate(self, vec3((1.-factor)*interpolations[segments-j-2], interpolations[j+1], factor*interpolations[segments-j-2]));
      factor = float(i)/float(segments-j-1);
      Displacement_interpolate(self, vec3((1.-factor)*interpolations[segments-j-1], interpolations[j], factor*interpolations[segments-j-1]));
     }
    Displacement_interpolate(self, vec3(0., interpolations[j+1], interpolations[segments-j-2]));
    Displacement_interpolate(self, vec3(0., interpolations[j], interpolations[segments-j-1]));
    EndPrimitive();
   }
  Displacement_interpolate(self, vec3(interpolations[1], interpolations[segments-2], 0.));
  Displacement_interpolate(self, vec3(0., 1., 0.));
  Displacement_interpolate(self, vec3(0., interpolations[segments-2], interpolations[1]));
  EndPrimitive();}
void Geometry_enter(OBJREF self)
{
  OBJREF base_self = shade_get_base_object10(self);
  {
    Displacement_enter_impl(self, obj_0xa03b908_coordinate_system, obj_0xa03b908_texture_unit);
  }
}
void Surface_geometry_impl(OBJREF self, OBJREF material, OBJREF geometry)
{
Geometry_enter(geometry);}
void Enterable_enter_geometry(OBJREF self)
{
  OBJREF base_self = shade_get_base_object9(self);
  {
    Surface_geometry_impl(self, obj_0xa038f28_material, obj_0xa038f28_geometry);
  }
}
void main()
{
  shade_initialize();
  Enterable_enter_geometry(0);
}
