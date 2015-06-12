#!/bin/bash

# get directory of script
DIR="$( cd "$( dirname "$0" )" && pwd )"

# assuming a local guacmole version is located properly
LOCAL_GUACAMOLE="$DIR/../../../../guacamole"
LOCAL_AVANGO="$DIR/../../../../avango"

# if not, this path will be used
GUACAMOLE=/opt/guacamole/master
AVANGO=/opt/avango/master

# third party libs
export LD_LIBRARY_PATH=/opt/boost/boost_1_55_0/lib:/opt/zmq/current/lib:/opt/Awesomium/lib:/opt/pbr/inst_cb/lib

# schism
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/schism/current/lib/linux_x86

# avango
export LD_LIBRARY_PATH="$LOCAL_AVANGO/lib":$AVANGO/lib:$LD_LIBRARY_PATH
export PYTHONPATH="$LOCAL_AVANGO/lib/python3.4":"$LOCAL_AVANGO/examples":$AVANGO/lib/python3.4:$AVANGO/examples

# guacamole
export LD_LIBRARY_PATH="$LOCAL_GUACAMOLE/lib":$GUACAMOLE/lib:$LD_LIBRARY_PATH

# run daemon
if [ -f "$LOCAL_AVANGO/examples/examples_common/daemon.py" ]
then
    python3 $LOCAL_AVANGO/examples/examples_common/daemon.py > /dev/null &
else
    python3 $AVANGO/examples/examples_common/daemon.py > /dev/null &
fi

# run daemon
python3.4 ./Daemon.py > /dev/null &

# run program
if [[ $* == *-d* ]]
then
cd "$DIR" && gdb --args python3.4 ./main_mixamo.py
else
cd "$DIR" && python3.4 ./main_mixamo.py
fi

# kill daemon
#kill %1
killall python3.4
killall python3