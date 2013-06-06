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
    exit(0)
#check if the model exists
elif not os.path.exists(filename):
    print "File: " + str(filename) + " does not exist. Exit."
    exit(0)
        

#create mouse device
mouse = avango.display.make_dominant_user_device(interface="Mouse")

# set up scene graph
obj_trans = avango.osg.nodes.MatrixTransform(Matrix=avango.osg.make_trans_mat(0,1.7,-1))
view.Root.value = avango.osg.nodes.Group(Children = [obj_trans])

#geode = avango.utils.load_pcd_file("table_scene_mug_stereo_textured.pcd")
#geode = avango.utils.load_pcd_file("/tmp/frame-pc-33.0.pcd")
geode = avango.utils.load_pcd_file(filename)
obj_trans.Children.value.append(geode)


# run evaluation and render loop
avango.display.run()
