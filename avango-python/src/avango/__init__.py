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

# Set required dlopen flags for avango and its modules to work
import sys
try:
    try:
        import dl
        _flags = dl.RTLD_NOW | dl.RTLD_GLOBAL
    except:
        # Some systems do no have module dl ...
        _flags = 0x2 | 0x100

    sys.setdlopenflags(_flags)
except:
    pass

# First register first-stage warning hook to record warnings during avango import
import warnings
warning_buffer = []
def showwarning_buffer(message, category, filename, lineno, file = None):
    warning_buffer.append([message, category, filename, lineno])
warnings.showwarning = showwarning_buffer

# Import all public members of C++ export
from _avango import *
# Explicily import hidden members
from _avango import _make_instance_by_name

# Parse command line options to configure logging
import getopt
opts = []
try:
    opts,args = getopt.getopt(sys.argv[1:], "n:l:h", ["notify=","log-file=","help"])
except getopt.GetoptError, err:
    pass

_notify = -1
_logfile = ''

for o, a in opts:
    if o in ("-h", "--help"):
        print "usage: ", sys.argv[0],"[options]"
        print "Avango NG options and arguments:"
        print "--help [-h]           : print help"
        print "--notify= [-n] arg    : log level (accepts 0-5)"
        print "--log-file= [-l] arg  : log to file (none = console)"
#       sys.exit(0)
    elif o in ("-n", "--notify"):
        _notify = a
    elif o in ("-l", "--log-file"):
        _logfile = a
#    else:
#        assert False, "unhandled option"

if _notify > -1:
    if _logfile == '':
        enable_logging(int(_notify))
    else: enable_logging(int(_notify), _logfile)

# Now register proper avango logging warnings hook
def showwarning_logging(message, category, filename, lineno, file = None):
    _avango.log("av::python::warnings", warnings.formatwarning(message, category, filename, lineno))
warnings.showwarning = showwarning_logging
for w in warning_buffer:
    showwarning_logging(*w)

# Now import additional setup code
import _fieldcontainer
from _utility import *

import nodefactory
nodes = nodefactory.NodeFactory(module=__name__)

# Finally add other avango sub-modules (in external directories) to search path
import sys
import os.path

def check_path(module_path, name):
    result = module_path[:]
    for path in sys.path:
        newpath = os.path.join(path, name)
        if os.path.isdir(newpath) and newpath not in result:
            result.append(newpath)
    return result

__path__ = check_path(__path__, __name__)
