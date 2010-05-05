uniform vec3 light_color_0;
uniform vec3 light_color_1;
varying vec3 trans_light_pos_0;
varying vec3 trans_light_pos_1;
varying vec3 obj_normal;
varying vec3 obj_eyepoint;
varying vec2 obj_texco;
uniform sampler2D normal_map;
uniform float plastic_specular;
uniform float plastic_diffuse;
uniform float plastic_roughness;
uniform vec4 plastic_color;

void main()
{
  vec4 result = vec4(0.);

  vec3 normal = normalize(texture2D(normal_map, obj_texco).xyz - vec3(0.5, 0.5, 0.5));

  vec3 light_direction = normalize(trans_light_pos_0 - vec3(0.));
  vec3 eye = normalize(obj_eyepoint-vec3(0.));
  float diffuse_intensity = dot(light_direction, normal);
  if (diffuse_intensity > 0.)
   {
    float specular_intensity = pow(max(0., dot(normal, normalize(light_direction+eye))), plastic_roughness);
    vec4 fragment_color = diffuse_intensity * plastic_diffuse * vec4(light_color_0, 1.) * plastic_color;
    fragment_color += specular_intensity * plastic_specular * vec4(light_color_0, 1.);
    result += fragment_color;
   }

  light_direction = normalize(trans_light_pos_1 - vec3(0.));
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
