uniform vec3 light_color_0;
varying vec3 trans_light_pos_0;
uniform vec3 light_color_1;
varying vec3 trans_light_pos_1;
varying vec3 obj_position;
varying vec3 obj_normal;
varying vec3 obj_eyepoint;
uniform float plastic_specular;
uniform float plastic_diffuse;
uniform float plastic_roughness;
uniform vec4 plastic_color;

 // main procedure, the original name was main
void main()
{
  vec4 result = vec4(0.);

  vec3 normal = normalize(obj_normal);

  vec3 light_direction = normalize(trans_light_pos_0 - obj_position);
  vec3 eye = normalize(obj_eyepoint-obj_position);
  float diffuse_intensity = dot(light_direction, normal);
  if (diffuse_intensity > 0.)
   {
    float specular_intensity = pow(max(0., dot(normal, normalize(light_direction+eye))), plastic_roughness);
    vec4 fragment_color = diffuse_intensity * plastic_diffuse * vec4(light_color_0, 1.) * plastic_color;
    fragment_color += specular_intensity * plastic_specular * vec4(light_color_0, 1.);
    result += fragment_color;
   }

  light_direction = normalize(trans_light_pos_1 - obj_position);
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
