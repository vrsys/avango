// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 2007 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <shade/Shader.h>
#include <shade/Program.h>
#include <shade/GLSLInstance.h>
#include <shade/types/float.h>
#include <shade/types/uniform.h>
#include <shade/types/const.h>
#include <shade/types/objref.h>
#include <shade/types/ValueReference.h>
#include <shade/types/vec2.h>
#include <shade/types/sampler2D.h>
#include <shade/shaders/Enterable.h>
#include <shade/types/inout.h>
#include <boost/shared_ptr.hpp>
#include <GL/glew.h>
#include <cstdlib>
#include <cmath>
#include "Texture.h"
#include "example.h"

namespace shaders = shade::shaders;

namespace
{
class Transform : public shade::ShaderBase<Transform>
{
  public:
    virtual shade::void_<> transform(void) { return invoke<shade::void_<>>("Transform_transform_impl"); }

  private:
    SHADE_BASE_DECL(Transform)
};
SHADE_CLASS_INIT(Transform, "Switch.glsl", SHADE_ENV_DEFS(shade::vertex_shader, (transform)), SHADE_NONE)

class WaveTransform : public shade::ShaderBase<WaveTransform, Transform>
{
  public:
    /*virtual*/ shade::void_<> transform(void) { return invoke<shade::void_<>>("WaveTransform_transform_impl"); }

    shade::float_<shade::uniform> value;

  private:
    SHADE_DERIVED_DECL(WaveTransform, Transform)
};
SHADE_CLASS_INIT(WaveTransform, "Switch.glsl", SHADE_NONE, SHADE_DEFS((value)))

class Switch : public shade::ShaderBase<Switch, shaders::Enterable>
{
  public:
    /*virtual*/ shade::void_<> enter_vertex(void) { return invoke<shade::void_<>>("Switch_vertex_impl"); }

    /*virtual*/ shade::void_<> enter_fragment(void) { return invoke<shade::void_<>>("Switch_fragment_impl"); }

    shade::objref<boost::shared_ptr<Transform>, shade::const_> obj_1;
    shade::objref<boost::shared_ptr<Transform>, shade::const_> obj_2;

    shade::vec2<shade::raw_varying> uv;
    shade::sampler2D texture_unit;

  private:
    SHADE_DERIVED_DECL(Switch, Enterable)
};
SHADE_CLASS_INIT(Switch, "Switch.glsl", SHADE_NONE, SHADE_DEFS((obj_1)(obj_2)(uv)(texture_unit)))

boost::shared_ptr<Switch> shader;
boost::shared_ptr<shade::Program> program;
boost::shared_ptr<shade::GLSLWrapper> state;
boost::shared_ptr<WaveTransform> wave;
float frame_counter = 0.;
} // namespace

void init(void)
{
    shader = boost::shared_ptr<Switch>(new Switch);
    shader->obj_1 = boost::shared_ptr<Transform>(new Transform);
    wave = boost::shared_ptr<WaveTransform>(new WaveTransform);
    shader->obj_2 = wave;
    shader->texture_unit.set(example::make_texture("examples/pattern.dds"));

    state = shade::create_GLSL_wrapper();
    program = boost::shared_ptr<shade::Program>(new shade::Program(shader, state));
}

void display(void)
{
    if(program->requires_compilation())
        program->compile();

    state->make_current();

    frame_counter += 0.04;
    wave->value.set(0.05 * sin(frame_counter));

    if(program->requires_upload())
        program->upload();

    example::setup_camera();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int steps = 10;
    float step_size = 1. / (2 * float(steps));
    glColor4f(1., 1., 1., 1.);
    for(int y = -steps; y != steps; ++y)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for(int x = -steps; x != steps; ++x)
        {
            glColor4f(1., 1., 1., (x * y > 0) ? 1. : 2.);
            glTexCoord2f(x * step_size, y * step_size);
            glVertex3f(x * step_size, y * step_size, y * step_size);
            glColor4f(1., 1., 1., (x * (y + 1) > 0) ? 1. : 2.);
            glTexCoord2f(x * step_size, (y + 1) * step_size);
            glVertex3f(x * step_size, (y + 1) * step_size, (y + 1) * step_size);
        }
        glEnd();
    }
}

int main(int argc, char* argv[])
{
    setenv("SHADE_DEBUG_NO_TRANSFORM", "1", 1);

    example::init(argc, argv, "SHADE Constant Material");
    example::set_display_func(display);

    init();

    example::run_main_loop();
    return 0;
}
