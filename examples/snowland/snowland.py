# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# Avango is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# Avango is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Avango. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# Avango is a trademark owned by FhG.                                    #
#                                                                        #
##########################################################################

import math
import avango.script
from avango.script import field_has_changed
import avango.osg.simpleviewer
import avango.osg.particle
import avango.daemon

class FakeSensor(avango.script.Script):
    TimeIn = avango.SFDouble()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        self.MatrixOut.value = avango.osg.make_trans_mat(0., -0.03, 0.)

def make_fake_sensor():
    time = avango.nodes.TimeSensor()
    sensor = FakeSensor()
    sensor.TimeIn.connect_from(time.Time)
    return sensor


class DumpMatrix(avango.script.Script):
    Matrix = avango.osg.SFMatrix()
    def evaluate(self):
        trans = self.Matrix.value.get_translate()
        print trans.x, trans.y, trans.z

class Scale(avango.script.Script):
    MatrixIn = avango.osg.SFMatrix()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        values = self.get_values()
        trans = values.MatrixIn.get_translate()
        trans /= 10000.
        matrix = avango.osg.make_trans_mat(trans)
        values.MatrixOut = matrix

class Accumulate(avango.script.Script):
    MatrixRef = avango.osg.SFMatrix()
    MatrixIn = avango.osg.SFMatrix()
    MatrixOut = avango.osg.SFMatrix()

    def evaluate(self):
        self.MatrixOut.value = self.MatrixIn.value * self.MatrixRef.value

class Collision(avango.script.Script):
    MatrixRef = avango.osg.SFMatrix()
    MatrixIn = avango.osg.SFMatrix()
    MatrixOut = avango.osg.SFMatrix()
    RootNode = avango.osg.SFNode()

    def evaluate(self):
        values = self.get_values()
        trans_in = values.MatrixIn.get_translate()
        trans_ref = values.MatrixRef.get_translate()
        hit = avango.osg.line_intersect(trans_ref, trans_in, values.RootNode)
        if hit != trans_in:
            new_matrix = values.MatrixRef
        else:
            new_matrix = values.MatrixIn
        values.MatrixOut = new_matrix

class GroundFollowing(avango.script.Script):
    MatrixIn = avango.osg.SFMatrix()
    MatrixOut = avango.osg.SFMatrix()
    RootNode = avango.osg.SFNode()
    Height = avango.SFFloat()
    JumpLength = avango.SFFloat()
    FallLength = avango.SFFloat()

    def evaluate(self):
        values = self.get_values()
        matrix = values.MatrixIn
        translation = matrix.get_translate()
        line_start = translation + avango.osg.Vec3(0, 0, values.JumpLength)
        line_end = translation - avango.osg.Vec3(0, 0, values.FallLength)
        hit = avango.osg.line_intersect(line_start, line_end, values.RootNode)
        if hit != line_end:
            height_offset = avango.osg.Vec3(0, 0, values.Height)
            matrix.set_translate(hit+height_offset)
        values.MatrixOut = matrix


class MatrixFeedback(avango.script.Script):
    IsSource = avango.SFBool()
    MatrixIn = avango.osg.SFMatrix()
    MatrixOut = avango.osg.SFMatrix()

    @field_has_changed(IsSource)
    def set_source(self):
        self.MatrixIn.enable_dependency(not self.IsSource.value)

    def evaluate(self):
        self.MatrixOut.value = self.MatrixIn.value

def make_ball(root):
    ball = avango.osg.nodes.Sphere(Color = avango.osg.Vec4(1,1,0,1), Radius = 0.2)
    feedback_source = MatrixFeedback(IsSource = True)
    feedback_sink = MatrixFeedback(IsSource = False)
    feedback_source.MatrixIn.connect_from(feedback_sink.MatrixOut)

    #sensor = make_fake_sensor()
    service = avango.daemon.DeviceService()
    sensor = avango.daemon.nodes.DeviceSensor(DeviceService = service,
                                              Station = "noyo-minispace")

    scale = Scale()
    accum = Accumulate()
    collision = Collision()
    ground_following = GroundFollowing(Height = 0.1, JumpLength = 0.1, FallLength = 0.5)

    scale.MatrixIn.connect_from(sensor.Matrix)
    accum.MatrixIn.connect_from(scale.MatrixOut)
    accum.MatrixRef.connect_from(feedback_source.MatrixOut)
    ground_following.RootNode.value = root
    ground_following.MatrixIn.connect_from(accum.MatrixOut)
    #collision.RootNode.value = root
    #collision.MatrixIn.connect_from(accum.MatrixOut)
    #collision.MatrixRef.connect_from(feedback_source.MatrixOut)

    pipeline_end = ground_following
    feedback_sink.MatrixIn.connect_from(pipeline_end.MatrixOut)
    ball.Matrix.connect_from(pipeline_end.MatrixOut)

    return ball


def make_precipitation():
    # setup a precipitation effect with fog
    precip = avango.osg.particle.nodes.PrecipitationEffect(Fog = avango.osg.nodes.Fog())
    precip.CellSize.value = avango.osg.Vec3(10,10,10)

    # setup stateset
    state = avango.osg.nodes.StateSet(FogMode = 1, Fog = precip.Fog.value)

    return precip, state


def make_land():
    land = avango.osg.nodes.LoadFile(Filename = 'Models/lzsnow.osg')
    land.Matrix.value = avango.osg.make_rot_mat(math.radians(63), 0, 0, 1) * avango.osg.make_trans_mat(-20, -20, -98)
    return land


def make_sky():
    sky = avango.osg.nodes.LoadFile(Filename = 'Models/skydome.osg')
    sky.Matrix.value = avango.osg.make_trans_mat(-26634.0, -37420.0, -2500) * avango.osg.make_scale_mat(0.07, 0.07, 0.15)
    skystate = avango.osg.nodes.StateSet(Fog = avango.osg.nodes.Fog(), FogMode = 1)
    sky.StateSet.value = skystate
    return sky, skystate.Fog.value


class Main(object):
    def __init__(self):
        self.precip, self.state = make_precipitation()
        self.land = make_land()
        self.sky, self.skyfog = make_sky()
        self.skyfog.Color.connect_from(self.precip.Fog.value.Color)
        self.root = avango.osg.nodes.Group(StateSet = self.state)
        self.ball = make_ball(self.root)
        self.root.Children.value = [ self.land, self.sky, self.precip, self.ball]

    def run(self):
        # start simpleviewer
        avango.osg.simpleviewer.run(self.root)

    def snow(self, value):
        self.precip.Snow.value = value
        self.precip.Fog.value.Density.value *= 1.5
        self.skyfog.Density.value = self.precip.Fog.value.Density.value * 0.2
        self.precip.Wind.value = avango.osg.Vec3(value * -4, 0, -1)

    def rain(self, value):
        self.precip.Rain = value
        self.precip.Fog.value.Density.value *= 1.5
        self.skyfog.Density.value = self.precip.Fog.value.Density.value * 0.2
        self.precip.Wind.value = avango.osg.Vec3(value * -4, 0, -1)


def start():
    main = Main()
    main.snow(0.3)
    main.run()

if __name__ == '__main__':
    start()
