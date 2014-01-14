uniform vec3 light_pos_0;
varying vec3 trans_light_pos_0;
uniform vec3 light_pos_1;
varying vec3 trans_light_pos_1;
varying vec3 obj_position;
varying vec3 obj_normal;
varying vec3 obj_eyepoint;
varying vec2 obj_texco;

varying vec3 obj_0xa03e000_value;
varying vec4 obj_0xa03b908_normal;
varying vec4 obj_0xa03b908_light_normal;
varying vec3 obj_0xa03f9b8_value;
varying vec3 obj_0xa040178_value;
varying vec2 obj_0xa041c50_value;
varying vec2 obj_0xa03b908_uvcoord;
varying vec3 obj_0xa042b50_value;

void main()
{
  obj_0xa03f9b8_value = gl_Vertex.xyz;
  obj_0xa041c50_value = gl_MultiTexCoord0.xy;

  obj_0xa03b908_uvcoord = gl_MultiTexCoord0.xy;
  obj_0xa03b908_normal = gl_ModelViewProjectionMatrix * vec4(gl_Normal, 0.);
  obj_0xa03b908_light_normal = vec4(0., 0., 1., 0.);

  vec3 obj_tangent = gl_MultiTexCoord1.xyz;
  vec3 obj_bitangent = gl_MultiTexCoord2.xyz;
  vec3 obj_normal = gl_Normal;

  vec4 v_object = gl_ModelViewMatrixInverse * vec4(0., 0., 0., 1.) - gl_Vertex;
  obj_0xa040178_value.x = dot(v_object.xyz, obj_tangent);
  obj_0xa040178_value.y = dot(v_object.xyz, obj_bitangent);
  obj_0xa040178_value.z = dot(v_object.xyz, obj_normal);

  v_object = gl_ModelViewMatrixInverse * vec4(light_pos_0, 1.) - gl_Vertex;
  obj_0xa03e000_value.x = dot(v_object.xyz, obj_tangent);
  obj_0xa03e000_value.y = dot(v_object.xyz, obj_bitangent);
  obj_0xa03e000_value.z = dot(v_object.xyz, obj_normal);

  v_object = gl_ModelViewMatrixInverse * vec4(light_pos_1, 1.) - gl_Vertex;
  obj_0xa042b50_value.x = dot(v_object.xyz, obj_tangent);
  obj_0xa042b50_value.y = dot(v_object.xyz, obj_bitangent);
  obj_0xa042b50_value.z = dot(v_object.xyz, obj_normal);

  gl_Position = ftransform();
} // main end
