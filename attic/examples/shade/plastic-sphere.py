# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################

import avango.osg
import avango.shade
import avango.display
import sys

argv = avango.display.init(sys.argv)
view = avango.display.make_view()
view.EnableTrackball.value = True

light = avango.shade.nodes.DirectionalLight()
light_source = avango.shade.nodes.LightSource(LightNum = 1)
light_source.Position.value = avango.osg.Vec4(0., 1., 1., 0.)
light.direction.connect_from(light_source.Shader)
light.color.value = avango.shade.nodes.Value_vec3_uniform(value = avango.osg.Vec3(1., 1., 1.))

coord_sys = avango.shade.nodes.CameraSpace()

plastic = avango.shade.nodes.Plastic(lights = [light],
                                     coordinate_system = coord_sys,
                                     specular=avango.shade.nodes.Value_float_uniform(value=0.3),
                                     diffuse=avango.shade.nodes.Value_float_uniform(value=0.9),
                                     roughness=avango.shade.nodes.Value_float_uniform(value=512.),
                                     )
plastic.color.value = avango.shade.nodes.Value_vec4_uniform(value = avango.osg.Vec4(1., 0., 0., 1.))

surface = avango.shade.nodes.Surface(material=plastic)
state = avango.shade.nodes.State(Shader=surface)

root = avango.shade.nodes.StateGroup(State=state)
sphere = avango.osg.nodes.Sphere(Radius=1.)

root.Children.value = [ sphere, light_source]
view.Root.value = root

avango.display.run()
