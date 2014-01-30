#!/usr/bin/python
# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
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

'''
Learn how to setup and use DeviceDaemon and DeviceService to readout tracking data
from a DTrack device running in an AvangoDaemon process (i.e. avango.daemon.run() ).

This example manipulates the position of a displayed sphere
based on the output the device specified in dtrack-daemon.py generates.
'''

import avango.daemon
import avango.script
import avango.osg.simpleviewer

# create a simple scene
sphere = avango.osg.nodes.Sphere(Radius = 0.15)
transform = avango.osg.nodes.MatrixTransform()
transform.Children.value.append(sphere)

# device service setup
service = avango.daemon.DeviceService()

# a sensor to a specific station (in this case stereo glasses 1)
# Note: Adapt transmitter and receiver offset to your local display setup!
# This values correspond to the parameters of the FhG iCone.
sensor = avango.daemon.nodes.DeviceSensor(DeviceService = service,
                                          Station = "ve-dtrack-head1",
                                          ReceiverOffset = avango.osg.make_trans_mat(-0.089,0.015,-0.040),
                                          TransmitterOffset = avango.osg.make_trans_mat(0, 0,-1.4))

# manipulate the position of the sphere
transform.Matrix.connect_from(sensor.Matrix)

# enter render loop
avango.osg.simpleviewer.run(transform)
