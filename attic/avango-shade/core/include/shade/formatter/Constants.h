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

#ifndef shade_formatter_Constants_H
#define shade_formatter_Constants_H shade_formatter_Constants_H

namespace shade
{
namespace formatter
{
struct Constants
{
    enum Qualifier
    {
        none,
        uniform,
        const_,
        input,
        output
    };

    enum Type
    {
        float_,
        vec2,
        vec3,
        vec4,
        matrix4,
        sampler2D,
        sampler2DShadow,
        sampler2DArray,
        usampler2D,
        samplerCube,
        objref,
        list,
        void_,
        int_
    };

    enum Primitive
    {
        points,
        lines,
        lines_adjacency,
        triangles,
        triangles_adjacency
    };

    enum Operator
    {
        pre_increment,
        post_increment,
        equal,
        not_equal,
        assignment,
        array_access,
        component_access,
        multiply,
        divide,
        add,
        subtract,
        assignment_add
    };
};
} // namespace formatter
} // namespace shade

#endif /* shade_formatter_Constants_H */
