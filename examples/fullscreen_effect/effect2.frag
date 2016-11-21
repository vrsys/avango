//#version 450
#version 440
#extension GL_NV_bindless_texture  : require
#extension GL_ARB_bindless_texture : enable
#extension GL_NV_gpu_shader5       : enable

#extension GL_ARB_shader_storage_buffer_object : enable
#extension GL_ARB_separate_shader_objects : enable

#ifdef GL_NV_shader_atomic_int64
#extension GL_NV_shader_atomic_int64 : enable
#endif
// 4 float layout
layout (std140, binding=0) uniform cameraBlock {
  mat4  gua_view_matrix;
  mat4  gua_projection_matrix;
  mat4  gua_inverse_projection_matrix;
  mat4  gua_inverse_projection_view_matrix;
  vec4  gua_camera_position_4;
  vec4  gua_clipping_planes[64];
  uvec2 gua_resolution;
  uvec2 gua_noise_texture;
  vec4  gua_cyclops_position_4;
  int   gua_clipping_plane_count;
  int   gua_view_id;
  float gua_clip_near;
  float gua_clip_far;
};

vec3 gua_camera_position = gua_camera_position_4.xyz;
vec3 gua_cyclops_position = gua_cyclops_position_4.xyz;

uniform mat4 gua_model_matrix;
uniform mat4 gua_model_view_matrix;
uniform mat4 gua_model_view_inverse_matrix;
uniform mat4 gua_model_view_projection_matrix;
uniform mat4 gua_normal_matrix;
uniform int  gua_rendering_mode; // 0: normal, 1: lowfi shadows, 2: hifi shadows
uniform uvec2 gua_gbuffer_color;
uniform uvec2 gua_gbuffer_pbr;
uniform uvec2 gua_gbuffer_normal;
uniform uvec2 gua_gbuffer_flags;
uniform uvec2 gua_gbuffer_depth;

uniform float gua_texel_width;
uniform float gua_texel_height;

uniform float time;

// quad coords -----------------------------------------------------------------
vec2 gua_get_quad_coords() {
  return vec2(gl_FragCoord.x * gua_texel_width, gl_FragCoord.y * gua_texel_height);
}

// color -----------------------------------------------------------------------
vec3 gua_get_color(vec2 frag_pos) {
    return texture2D(sampler2D(gua_gbuffer_color), frag_pos).rgb;
}

vec3 gua_get_color() {
    return gua_get_color(gua_get_quad_coords());
}

//layout(location = 0) out vec4 outColor;
layout(location=0) out vec3 gua_out_color;

#define SMOOTH(r) (mix(1.0, 0.0, smoothstep(0.9,1.0, r)))
#define M_PI 3.1415926535897932384626433832795

float movingRing(vec2 uv, vec2 center, float r1, float r2)
{
    vec2 d = uv - center;
    float r = sqrt( dot( d, d ) );
    d = normalize(d);
    float theta = -atan(d.y,d.x);
    theta  = mod(-time+0.5*(1.0+theta/M_PI), 1.0);

    //anti aliasing for the ring's head (thanks to TDM !)
    theta -= max(theta - 1.0 + 1e-2, 0.0) * 1e2;
    return theta*(SMOOTH(r/r2)-SMOOTH(r/r1));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord.xy;
    float ring = movingRing(uv,
    vec2(float(gua_resolution.x)/2.0,float(gua_resolution.y)/2.0), 20.0, 30.0);
    fragColor = vec4( 0.1 + 0.9*ring );
}

void main() {
  vec4 color;
  mainImage(color, gl_FragCoord.xy);
  gua_out_color = mix(vec3(color), gua_get_color(), sin(time/4.0));
}
