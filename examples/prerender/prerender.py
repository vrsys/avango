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

from avango import display
import avango
import avango.osg
import avango.script
import math
import sys

class RotateMatrix(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        self.MatrixOut.value = avango.osg.make_rot_mat(self.TimeIn.value, 0., 1., 0.)

class SwingMatrix(avango.script.Script):
    TimeIn = avango.SFFloat()
    Scale = avango.SFFloat()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        self.MatrixOut.value = avango.osg.make_trans_mat(0., self.Scale.value*math.sin(self.TimeIn.value), 0.)


def build_texture(time_sensor, root):
    texture2d = avango.osg.nodes.Texture2D()
    texture2d.TextureWidth.value = 48
    texture2d.TextureHeight.value = 48

    camera_attachment = avango.osg.nodes.CameraAttachment(Texture=texture2d)

    camera = avango.osg.nodes.Camera(Attachment=[camera_attachment],
                                     RenderOrder=avango.osg.RenderOrder.PRE_RENDER,
                                     RenderTargetImplementation=avango.osg.RenderTargetImplementation.FRAME_BUFFER_OBJECT, # only frame buffer works at all
                                     ClearColor=avango.osg.Vec4(0., 1., 0.,  1.),
                                     Viewport=avango.osg.Vec4(0., 0., 2048., 2048),
                                     ClearMask=avango.osg.ClearMask.GL_COLOR_BUFFER_BIT|avango.osg.ClearMask.GL_DEPTH_BUFFER_BIT|avango.osg.ClearMask.GL_ACCUM_BUFFER_BIT|avango.osg.ClearMask.GL_STENCIL_BUFFER_BIT,
                                     ReferenceFrame="ABSOLUTE_RF",
                                    )
    #set the view matrix
    position = avango.osg.Vec3(0,0,1)
    center = avango.osg.Vec3(0,0,0)
    up = avango.osg.Vec3(0,1,0)
    view_matrix = avango.osg.make_lookat_mat(position,center,up)
    camera.ViewMatrix.value = view_matrix

    #set the projection matrix
    near = 0.1
    far = 10000
    if ortho_projection:
        left = -1.0
        right = 1.0
        bottom = -1.0
        top = 1.0
        projection_matrix = avango.osg.make_ortho_mat(left, right, bottom, top, near, far)
        camera.ProjectionMatrix.value = projection_matrix
    else:
        fov = 75.0
        aspect = 1.0
        projection_matrix = avango.osg.make_perspective_mat(fov,aspect,near,far)
        camera.ProjectionMatrix.value = projection_matrix

    sphere = avango.osg.nodes.Sphere(Radius=1.0, Matrix=avango.osg.make_trans_mat(0., 0., -1.))

    swing = SwingMatrix(Scale=1.0)
    swing.TimeIn.connect_from(time_sensor.Time)

    transform = avango.osg.nodes.MatrixTransform()
    transform.Matrix.connect_from(swing.MatrixOut)
    transform.Children.value = [sphere]

    camera.Children.value.append(transform)
    root.Children.value.append(camera)

    return texture2d, [swing]


def build_quad(time_sensor):
    geometry = avango.osg.nodes.Geometry(Mode = 7, NormalBinding = 1, ColorBinding = 1)

    vertex_coords = []
    tex_coords = []
    uvs = [ (1., 1.), (0., 1.), (0., 0.), (1., 0.) ]
    for u, v in uvs:
        vertex_coords.append(avango.osg.Vec3(0.1*(2*u-1.), 0.1*(2.*v-1.), 0.))
        tex_coords.append(avango.osg.Vec2(u, v))

    geometry.VertexArray.value = vertex_coords
    geometry.TexCoordArray.value = tex_coords
    geometry.PrimitiveIndices.value = [0,1,2,3]
    geometry.PrimitiveLengths.value = [4]

    root = avango.osg.nodes.Group()
    rtt_root = avango.osg.nodes.Group()
    texture2d, keep_alive = build_texture(time_sensor, rtt_root)
    state_set = avango.osg.nodes.StateSet(Texture0 = texture2d)

    rotation = RotateMatrix()
    rotation.TimeIn.connect_from(time_sensor.Time)

    transform = avango.osg.nodes.MatrixTransform()
    transform.Matrix.connect_from(rotation.MatrixOut)
    transform.Children.value = [avango.osg.nodes.Geode(Drawables=[geometry], StateSet=state_set)]

    root.Children.value.append(transform)
    return root, rtt_root, keep_alive+[rotation]


argv = display.init(sys.argv)
view1 = display.make_view()
view1.BackgroundColor.value = avango.osg.Vec4(0.5, 0.5, 0.5, 1.)

ortho_projection = False
if "ortho" in argv:
    ortho_projection = True

time_sensor = avango.nodes.TimeSensor()
quad_geom, rtt_root, keep_alive = build_quad(time_sensor)
transform = avango.osg.nodes.MatrixTransform(Children=[quad_geom,rtt_root], Matrix=avango.osg.make_trans_mat(0, 1.7, -2.2))
view1.Root.value = transform

display.run()
