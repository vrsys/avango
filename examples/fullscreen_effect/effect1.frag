#version 440
// header
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

// quad coords -----------------------------------------------------------------
vec2 gua_get_quad_coords() {
  return vec2(gl_FragCoord.x * gua_texel_width, gl_FragCoord.y * gua_texel_height);
}

// depth -----------------------------------------------------------------------

float gua_get_unscaled_depth() {
    vec2 frag_pos = gua_get_quad_coords();
    return texture2D(sampler2D(gua_gbuffer_depth), frag_pos).x;
}

float gua_get_depth(vec2 frag_pos) {
    return texture2D(sampler2D(gua_gbuffer_depth), frag_pos).x * 2.0 - 1.0;
}

float gua_get_depth() {
    vec2 frag_pos = gua_get_quad_coords();
    return texture2D(sampler2D(gua_gbuffer_depth), frag_pos).x * 2.0 - 1.0;
}

float gua_get_depth(sampler2D depth_texture, vec2 frag_pos) {
    return texture2D(depth_texture, frag_pos).x * 2.0 - 1.0;
}

float gua_get_depth(sampler2D depth_texture) {
    vec2 frag_pos = gua_get_quad_coords(); 
    return texture2D(depth_texture, frag_pos).x * 2.0 - 1.0;
}

// position --------------------------------------------------------------------
vec3 gua_get_position(vec2 frag_pos) {
    vec4 screen_space_pos = vec4(frag_pos * 2.0 - 1.0, gua_get_depth(frag_pos), 1.0);
    vec4 h = gua_inverse_projection_view_matrix * screen_space_pos;
    h /= h.w; 
    return h.xyz;
}

vec3 gua_get_position() {
    return gua_get_position(gua_get_quad_coords());
}

vec3 gua_get_position(sampler2D depth_texture, vec2 frag_pos) {
    vec4 screen_space_pos = vec4(frag_pos * 2.0 - 1.0, gua_get_depth(depth_texture, frag_pos), 1.0);
    vec4 h = gua_inverse_projection_view_matrix * screen_space_pos;
    h /= h.w;
    return h.xyz;
}

vec3 gua_get_position(sampler2D depth_texture) {
    return gua_get_position(depth_texture, gua_get_quad_coords());
} 

// color -----------------------------------------------------------------------
vec3 gua_get_color(vec2 frag_pos) {
    return texture2D(sampler2D(gua_gbuffer_color), frag_pos).rgb;
}

vec3 gua_get_color() {
    return gua_get_color(gua_get_quad_coords());
}

// normal ----------------------------------------------------------------------
vec3 gua_get_normal(vec2 frag_pos) {
    return texture2D(sampler2D(gua_gbuffer_normal), frag_pos).rgb * 2 - 1;
}

vec3 gua_get_normal() {
    return gua_get_normal(gua_get_quad_coords());
}

// pbr -------------------------------------------------------------------------
vec3 gua_get_pbr(vec2 frag_pos) {
    return texture2D(sampler2D(gua_gbuffer_pbr), frag_pos).rgb;
}

vec3 gua_get_pbr() {
    //return gua_get_pbr(gua_get_quad_coords());
    return texelFetch(sampler2D(gua_gbuffer_pbr), ivec2(gl_FragCoord.xy), 0).rgb;
}

// flags -----------------------------------------------------------------------
uint gua_get_flags(vec2 frag_pos) {
    return uint(texture2D(usampler2D(gua_gbuffer_flags), frag_pos).r);
}

uint gua_get_flags() {
    return uint(texelFetch(usampler2D(gua_gbuffer_flags), ivec2(gl_FragCoord.xy), 0).r);
}

layout(location=0) out vec3 gua_out_color;
void main() { 
  gua_out_color = 0.5 * gua_get_color();
}
