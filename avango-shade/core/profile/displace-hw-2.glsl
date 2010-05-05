#version 120
#extension GL_EXT_geometry_shader4: require

varying in vec3 obj_0xa03e000_value[3];
varying out vec3 obj_0xa03e100_value;
varying in vec4 obj_0xa03b908_normal[3];
varying in vec4 obj_0xa03b908_light_normal[3];
varying in vec3 obj_0xa03f9b8_value[3];
varying out vec3 obj_0xa03fab8_value;
varying in vec3 obj_0xa040178_value[3];
varying out vec3 obj_0xa0401d0_value;
varying in vec2 obj_0xa041c50_value[3];
varying out vec2 obj_0xa041d10_value;
uniform sampler2D obj_0xa03b908_texture_unit;
varying in vec2 obj_0xa03b908_uvcoord[3];
varying in vec3 obj_0xa042b50_value[3];
varying out vec3 obj_0xa042c50_value;

void shade_initialize_post(vec4 light_normal, vec3 uvw)
{
  vec3 value = uvw.x * obj_0xa03f9b8_value[0];
  value+=(uvw).y * obj_0xa03f9b8_value[1];
  value+=(uvw).z * obj_0xa03f9b8_value[2];
  obj_0xa03fab8_value = value;

  value = (uvw).x * obj_0xa040178_value[0];
  value+=(uvw).y * obj_0xa040178_value[1];
  value+=(uvw).z * obj_0xa040178_value[2];
  value = value - light_normal.xyz;
  obj_0xa0401d0_value = value;

  vec2 value2 = (uvw).x*obj_0xa041c50_value[0];
  value2+=(uvw).y*obj_0xa041c50_value[1];
  value2+=(uvw).z*obj_0xa041c50_value[2];
  obj_0xa041d10_value = value2;

  value = uvw.x * (obj_0xa03e000_value)[0];
  value += uvw.y * (obj_0xa03e000_value)[1];
  value += uvw.z * (obj_0xa03e000_value)[2];
  value = value - light_normal.xyz;
  obj_0xa03e100_value = value;

  value = uvw.x * (obj_0xa042b50_value)[0];
  value += uvw.y * (obj_0xa042b50_value)[1];
  value += uvw.z * (obj_0xa042b50_value)[2];
  value = value - light_normal.xyz;
  obj_0xa042c50_value = value;
}

void shade_emit_vertex(vec4 light_normal, vec3 factor)
{
  shade_initialize_post(light_normal, factor);
  EmitVertex();
}

void Displacement_interpolate(vec3 factor)
{
  vec2 uvcoord_0 = obj_0xa03b908_uvcoord[0];
  vec2 uvcoord_1 = obj_0xa03b908_uvcoord[1];
  vec2 uvcoord_2 = obj_0xa03b908_uvcoord[2];
  float offset = 0.05 * (texture2D(obj_0xa03b908_texture_unit, factor.x*uvcoord_0 + factor.y*uvcoord_1 + factor.z*uvcoord_2).a - 1.);
  vec4 position = factor.x*gl_PositionIn[0] + factor.y*gl_PositionIn[1] + factor.z*gl_PositionIn[2];
  vec4 normal_0 = obj_0xa03b908_normal[0];
  vec4 normal_1 = obj_0xa03b908_normal[1];
  vec4 normal_2 = obj_0xa03b908_normal[2];
  vec4 normal = factor.x*normal_0 + factor.y*normal_1 + factor.z*normal_2;
  gl_Position = position + offset* normal;

  vec4 light_normal_0 = obj_0xa03b908_light_normal[0];
  vec4 light_normal_1 = obj_0xa03b908_light_normal[1];
  vec4 light_normal_2 = obj_0xa03b908_light_normal[2];
  vec4 light_normal = factor.x*light_normal_0 + factor.y*light_normal_1 + factor.z*light_normal_2;
  light_normal *= offset;

  shade_emit_vertex(light_normal, factor);
}

void main()
{
  const int segments = 8;
  float interpolations[segments];
  for (int i = 0; i != segments; ++i)
   {
    interpolations[i] = float(i)/float(segments-1);
   }
  Displacement_interpolate(vec3(1., 0., 0.));
  Displacement_interpolate(vec3(interpolations[segments-2], interpolations[1], 0.));
  Displacement_interpolate(vec3(interpolations[segments-2], 0., interpolations[1]));
  for (int i = 2; i < segments-1; ++i)
   {
    float factor = float(i-1)/float(segments-2);
    Displacement_interpolate(vec3((1.-factor)*interpolations[segments-2], interpolations[1], factor*interpolations[segments-2]));
    Displacement_interpolate(vec3(interpolations[segments-i-1], 0., interpolations[i]));
   }
  Displacement_interpolate(vec3(0., interpolations[1], interpolations[segments-2]));
  Displacement_interpolate(vec3(0., 0., 1.));
  EndPrimitive();
  for (int j = 1; j < segments-2; ++j)
   {
    Displacement_interpolate(vec3(interpolations[segments-j-1], interpolations[j], 0.));
    Displacement_interpolate(vec3(interpolations[segments-j-2], interpolations[j+1], 0.));
    float factor = 1./float(segments-j-1);
    Displacement_interpolate(vec3((1.-factor)*interpolations[segments-j-1], interpolations[j], factor*interpolations[segments-j-1]));
    for (int i = 2; i < segments-j-1; ++i)
     {
      factor = float(i-1)/float(segments-j-2);
      Displacement_interpolate(vec3((1.-factor)*interpolations[segments-j-2], interpolations[j+1], factor*interpolations[segments-j-2]));
      factor = float(i)/float(segments-j-1);
      Displacement_interpolate(vec3((1.-factor)*interpolations[segments-j-1], interpolations[j], factor*interpolations[segments-j-1]));
     }
    Displacement_interpolate(vec3(0., interpolations[j+1], interpolations[segments-j-2]));
    Displacement_interpolate(vec3(0., interpolations[j], interpolations[segments-j-1]));
    EndPrimitive();
   }
  Displacement_interpolate(vec3(interpolations[1], interpolations[segments-2], 0.));
  Displacement_interpolate(vec3(0., 1., 0.));
  Displacement_interpolate(vec3(0., interpolations[segments-2], interpolations[1]));
  EndPrimitive();
}
