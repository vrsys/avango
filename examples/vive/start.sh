#!/bin/bash
killall python3

# Determine script location (i.e. path of this file)
DIR="$( cd "$( dirname "$0" )" && pwd )"

GUACAMOLE=/opt/guacamole/master
AVANGO=/opt/avango/master

export LD_LIBRARY_PATH=$GUACAMOLE/lib:$AVANGO/lib:/opt/boost/boost_1_55_0/lib:/opt/zmq/current/lib:/opt/Awesomium/lib:/opt/pbr/inst_cb/lib:/opt/schism/current/lib/linux_x86

export PYTHONPATH=$AVANGO/lib/python3.4:$AVANGO/examples

# run program
if [[ $* == *-d* ]]
then
    cd "$DIR" && gdb --args python3 ./main.py
else
    cd "$DIR" && python3 ./main.py
fi

kill %1