#!/bin/bash

# get directory of script
DIR="$( cd "$( dirname "$0" )" && pwd )"

# assuming a local guacmole version is located properly
LOCAL_GUACAMOLE="$DIR/../../../guacamole"
LOCAL_AVANGO="$DIR/../../../avango"

# if not, this path will be used
GUACAMOLE=/opt/guacamole/current/guacamole
AVANGO=/opt/guacamole/current/avango

# third party libs
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/boost/current/lib:/opt/openscenegraph/3.0.1/lib64/:/opt/zmq/current/lib

# schism
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/schism/current/lib/linux_x86

# avango
export LD_LIBRARY_PATH="$LOCAL_AVANGO/lib":$AVANGO/lib:$LD_LIBRARY_PATH
export PYTHONPATH="$LOCAL_AVANGO/lib/python2.7":"$LOCAL_AVANGO/examples":$AVANGO/lib/python2.7:$AVANGO/examples:$PYTHONPATH

# guacamole
export LD_LIBRARY_PATH="$LOCAL_GUACAMOLE/lib":$GUACAMOLE/lib:$LD_LIBRARY_PATH

# run program
cd "$DIR" && ./main.py
