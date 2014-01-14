/*
 * Standard declarations
 */

#define OBJREF int

#define FOR_EACH(list, index) for(OBJREF index = list.begin; index != list.end; ++index)

struct shade_list
{
  int begin;
  int end;
};


/*
 * Class declarations
 */

/* pseudo-code
struct LightSource
{
  uniform float intensity;
};
*/

#define LightSource_SELF OBJREF self, in float intensity


/* pseudo-code
struct PointLightSource
{
  uniform vec3 position;
};
*/

#define PointLightSource_SELF OBJREF self, in vec3 position


/* pseudo-code
struct Material_const
{
  const shade_list lights;
  uniform vec3 color;
  varying vec3 position;
  varying vec3 normal;
};
*/

#define Material_SELF OBJREF self, in shade_list lights, in vec3 color, in vec3 position, in vec3 normal


/*
 * Object instances
 */

const shade_list obj_0x0_lights = shade_list(1, 2);
uniform vec3 obj_0x0_color;
varying vec3 obj_0x0_position;
varying vec3 obj_0x0_normal;

uniform vec3 obj_0x1_position;
uniform float obj_0x1_intensity;


/*
 * Dispatch forward definitions
 */

vec3 LightSource_illuminate(OBJREF self, vec3 pos, out vec3 dir);
float LightSource_get_intensity(OBJREF self);

void Material_shade(OBJREF self);


/*
 * Included class implementations
 */

vec3 LightSource_illuminate_impl(LightSource_SELF, vec3 pos, out vec3 dir)
{
  return vec3(0., 0., 0.);
}

float LightSource_get_intensity_impl(LightSource_SELF)
{
  return intensity;
}


vec3 PointLightSource_illuminate_impl(PointLightSource_SELF, vec3 pos, out vec3 dir)
{
  dir = position - pos;
  return vec3(LightSource_get_intensity(self));
}


void Material_shade_impl(Material_SELF)
{
  vec3 result_color = vec3(0., 0., 0.);
  FOR_EACH(lights, light)
  {
    vec3 dir = vec3(0., 0., 0.);
    vec3 intensity = LightSource_illuminate(light, position, dir);
    result_color += intensity * max(0., dot(normal, dir));
  }
  gl_FragColor = vec4(color * result_color, 1.);
}


/*
 * Dispatch declarations
 */

vec3 LightSource_illuminate(OBJREF self, vec3 arg1, out vec3 arg2)
{
  if (self == 1)
  {
    return PointLightSource_illuminate_impl(1, obj_0x1_position, arg1, arg2);
  }
  return vec3(0., 0., 0.);
}

float LightSource_get_intensity(OBJREF self)
{
  if (self == 1)
  {
    return LightSource_get_intensity_impl(1, obj_0x1_intensity);
  }
  return 0.;
}

void Material_shade(OBJREF self)
{
  if (self == 0)
  {
    Material_shade_impl(0, obj_0x0_lights, obj_0x0_color, obj_0x0_position, obj_0x0_normal);
    return;
  }
}


/*
 * Main entry point
 */
void main(void)
{
  Material_shade(0);
}
