// SHADE
// Vec4ToVec3

vec3 Vec4ToVec3_get_impl(Vec4ToVec3_SELF)
{
  return Gettable_vec4_get(value).xyz;
}
