// SHADE
// IlluminatedMaterial

void IlluminatedMaterial_init_vertex_impl(IlluminatedMaterial_SELF)
{
  FOREACH(iter, lights)
  {
    ILight_transform(iter, coordinate_system);
  }

  IlluminatedMaterial_chained_init_vertex(self);
}

vec4 IlluminatedMaterial_shade_impl(IlluminatedMaterial_SELF)
{
  IlluminatedMaterial_init(self);
  vec3 position = Gettable_vec3_get(coordinate_system);

  FOREACH(iter, lights)
  {
    ILight_illuminate(iter, self, position);
  }

  return IlluminatedMaterial_gather(self);
}

void IlluminatedMaterial_init_impl(IlluminatedMaterial_SELF)
{
  vec4 black = vec4(0.);
  IlluminatedMaterial_set_acc_color(self, black);
}

void IlluminatedMaterial_accum_color_impl(IlluminatedMaterial_SELF, vec4 color)
{
  vec4 accum_color = IlluminatedMaterial_get_acc_color(self);
  accum_color += color;
  IlluminatedMaterial_set_acc_color(self, accum_color);
}

vec4 IlluminatedMaterial_gather_impl(IlluminatedMaterial_SELF)
{
  return IlluminatedMaterial_get_acc_color(self);
}

vec3 IlluminatedMaterial_get_normal_impl(IlluminatedMaterial_SELF)
{
  vec3 position = ICoordinateSystem_get_transformed_normal(coordinate_system);
  return position;
}

vec3 IlluminatedMaterial_get_position_impl(IlluminatedMaterial_SELF)
{
  vec3 position = Gettable_vec3_get(coordinate_system);
  return position;
}

vec3 IlluminatedMaterial_get_eyepoint_impl(IlluminatedMaterial_SELF)
{
  vec3 position = ICoordinateSystem_get_transformed_eyepoint(coordinate_system);
  return position;
}
