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
Simply loads a model from a file and displays it
'''

import avango.osg
import avango.display
import avango.tools
import avango.utils

import os
import sys
import getopt

#options
short_options="h"
long_options=["model="]

#initialize the display
args = avango.display.init(sys.argv, short_options, long_options)
view = avango.display.make_view("")
view.EnableTrackball.value = True


#default model
default_model = "monkey.obj"
filename = default_model

#parse options
try:
    opts, args = getopt.getopt(args, short_options, long_options)
except getopt.GetoptError, err:
    print "error: wrong arguments"
    opts = [("-h", "")]    


for opt, arg in opts:
    if opt in ("-h", "--help"):
        print "Options:"
        print " --model=<filename>        Specify the model to be loaded"
        
        print "Default model <monkey.obj> will be loaded."
        print "Press c to capture a screenshot"
    elif opt in ("--model"):
        filename = arg
    

#check if the model exists
if not os.path.exists(filename):
    print "File: " + str(filename) + " does not exist. Load default model " + str(default_model) +"."
    filename = default_model


root = avango.osg.nodes.Group()

model = avango.osg.nodes.LoadFile(Filename=filename)
root.Children.value = [model]

view.Root.value = root

#create a keyboard device
keyboard = avango.display.make_dominant_user_device(interface="Keyboard")
#add some custom keys
keyboard.add_key("KeyC",99)
avView = avango.display.get_view("")
#create screen capture script and assign a view. Press the c button to start capturing
capture = avango.utils.ScreenCapture(View=avView, NumFrames=30, OutputFolder="/tmp", BaseFilename="screen_shot")
capture.Start.connect_from(avango.utils.make_key_released_trigger(keyboard.KeyC))


avango.display.run()
