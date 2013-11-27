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
export LD_LIBRARY_PATH="$LOCAL_AVANGO/avango_out":$AVANGO/lib:$LD_LIBRARY_PATH
export PYTHONPATH="$LOCAL_AVANGO/avango_out":"$LOCAL_AVANGO/examples":$AVANGO/lib:$AVANGO/examples:$PYTHONPATH

# guacamole
export LD_LIBRARY_PATH="$LOCAL_GUACAMOLE/lib":$GUACAMOLE/lib:$LD_LIBRARY_PATH

"$DIR"/daemon.py > /dev/null &

# run daemon
#if [ -f "$LOCAL_AVANGO/examples/oculus_tracking/daemon.py" ]
#then
#    echo "Local"
#    "$LOCAL_AVANGO/examples/examples_common/daemon.py" > /dev/null &
#else
#	echo "Global"
#    "$AVANGO/examples/examples_common/daemon.py" > /dev/null &
#fi

# run program
cd "$DIR" && ./main.py

# kill daemon
kill %1
