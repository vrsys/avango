uniform vec3 light_color_0;
uniform vec3 light_color_1;
varying out vec3 obj_0xa03e100_value;
varying out vec3 obj_0xa03fab8_value;
varying out vec3 obj_0xa0401d0_value;
varying out vec2 obj_0xa041d10_value;
varying out vec3 obj_0xa042c50_value;
uniform sampler2D normal_map;
uniform float plastic_specular;
uniform float plastic_diffuse;
uniform float plastic_roughness;
uniform vec4 plastic_color;

void main()
{
  vec4 result = vec4(0.);

  vec3 normal = normalize(texture2D(normal_map, obj_0xa041d10_value).xyz - vec3(0.5, 0.5, 0.5));

  vec3 light_direction = normalize(obj_0xa03e100_value - vec3(0.));
  vec3 eye = normalize(obj_0xa0401d0_value-vec3(0.));
  float diffuse_intensity = dot(light_direction, normal);
  if (diffuse_intensity > 0.)
   {
    float specular_intensity = pow(max(0., dot(normal, normalize(light_direction+eye))), plastic_roughness);
    vec4 fragment_color = diffuse_intensity * plastic_diffuse * vec4(light_color_0, 1.) * plastic_color;
    fragment_color += specular_intensity * plastic_specular * vec4(light_color_0, 1.);
    result += fragment_color;
   }

  light_direction = normalize(obj_0xa042c50_value - vec3(0.));
  diffuse_intensity = dot(light_direction, normal);
  if (diffuse_intensity > 0.)
   {
    float specular_intensity = pow(max(0., dot(normal, normalize(light_direction+eye))), plastic_roughness);
    vec4 fragment_color = diffuse_intensity * plastic_diffuse * vec4(light_color_1, 1.) * plastic_color;
    fragment_color += specular_intensity * plastic_specular * vec4(light_color_1, 1.);
    result += fragment_color;
   }

  gl_FragColor = result;
} // main end
