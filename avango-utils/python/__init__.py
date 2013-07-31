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

from _nodes import *

from _utils import *
from _MFMerger import *
from _bool_script_merge import *
from _bool_scripts import *
from _converter import *
from _property_modifier import *
from _task_scheduler import *
from _triggers import *
from _normalization import *
from _field_dump import *
from _interconnect import *
from _range_checks import *
from _node_scalers import *
from _interpolators import *
from _node_optimizer import *
from _screen_capture import *


import Leap
from _leap_motion import *
    
#check for leapMotion sdk
leap_motion_sdk_found = False
try:
    import Leap
    from _leap_motion import *
except ImportError:
    print "#############################################"
    print "#No LeapMotion SDK found. No driver support.#"
    print "#You won't be able to use any of the        #"
    print "#LeapMotion functionality                   #"
    print "#############################################"
