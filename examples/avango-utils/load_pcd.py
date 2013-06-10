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

import avango.osg.viewer
import avango.moving
import avango.tools
import avango.display

import os
import sys
import getopt
import sys

#options
short_options="h"
long_options=["pcd="]

#initialize the display
args = avango.display.init(sys.argv, short_options, long_options)
view = avango.display.make_view()
view.EnableTrackball.value = True

#default model
filename = ""

#parse options
try:
    opts, args = getopt.getopt(args, short_options, long_options)
except getopt.GetoptError, err:
    print "error: wrong arguments"
    opts = [("-h", "")]    


for opt, arg in opts:
    if opt in ("-h", "--help"):
        print "Options:"
        print " --pcd=<filename>        Specify the pcd file to be loaded"
        
        exit(0)
    elif opt in ("--pcd"):
        filename = arg

if filename == "":
    print "No pcd file given. Use --pcd=<filename>. Exit."
    sys.exit()
#check if the model exists
elif not os.path.exists(filename):
    print "File: " + str(filename) + " does not exist. Exit."
    sys.exit(0)
        

#create mouse device
mouse = avango.display.make_dominant_user_device(interface="Mouse")

# set up scene graph
view.Root.value = avango.osg.nodes.Group()

#geode = avango.utils.load_pcd_file(filename)
#obj_trans.Children.value.append(geode)

#load the points
pcd_cloud = avango.utils.nodes.PCLPointCloud(Filename=filename)

#create a geometry and connect the points and colors
geometry = avango.osg.nodes.Geometry()
geometry.VertexArray.connect_from(pcd_cloud.Points)
geometry.ColorArray.connect_from(pcd_cloud.Colors)
geometry.ColorBinding.value = 4 # per vertex
geometry.Mode.value = 0 # points

#add the geometry into a geode and add it to the scenegraph
geode = avango.osg.nodes.Geode(Drawables=[geometry])
view.Root.value.Children.value.append(geode)


# run evaluation and render loop
avango.display.run()
