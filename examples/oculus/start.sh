#!/bin/bash

# get directory of script
DIR="$( cd "$( dirname "$0" )" && pwd )"

# assuming a local guacmole version is located properly
LOCAL_GUACAMOLE="$DIR/../../../guacamole"
LOCAL_AVANGO="$DIR/../../../avango"

# if not, this path will be used
GUACAMOLE=/opt/guacamole/master
AVANGO=/opt/avango/master

echo "Starting Oculus Rift Daemon..."
/opt/OculusSDK/currentSDK2/Service/OVRServer/Bin/Linux/x86_64/ReleaseStatic/ovrd&

sleep 3
echo "Done."

# third party libs
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/boost/current/lib:/opt/openscenegraph/3.0.1/lib64/:/opt/zmq/current/lib:/opt/lamure/install/lib:/opt/Awesomium/lib:/opt/OculusSDK/currentSDK2/LibOVR/Lib/Linux/x86_64/Release

# schism
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/schism/current/lib/linux_x86

# avango
export LD_LIBRARY_PATH="$LOCAL_AVANGO/lib":$AVANGO/lib:$LD_LIBRARY_PATH
export PYTHONPATH="$LOCAL_AVANGO/lib/python3.4":"$LOCAL_AVANGO/examples":$AVANGO/lib/python3.4:$AVANGO/examples:$PYTHONPATH

# guacamole
export LD_LIBRARY_PATH="$LOCAL_GUACAMOLE/lib":$GUACAMOLE/lib:$LD_LIBRARY_PATH

# run program
cd "$DIR" && python3 ./main.py
