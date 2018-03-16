#!/bin/bash

# get directory of script
DIR="$( cd "$( dirname "$0" )" && pwd )"

LOCAL_GUACAMOLE="/home/xaf/guacamole/guacamole"
LOCAL_AVANGO="/home/xaf/guacamole/avango"

# third party libs
export LD_LIBRARY_PATH=/home/xaf/.local/lib:/usr/lib/x86_64-linux-gnu:/home/xaf/guacamole/lamure/install/lib

# schism
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/xaf/guacamole/schism/lib/linux_x86

# avango
export LD_LIBRARY_PATH="$LOCAL_AVANGO/lib":$LD_LIBRARY_PATH
export PYTHONPATH="$LOCAL_AVANGO/lib/python3.5":"$LOCAL_AVANGO/examples"

# guacamole
export LD_LIBRARY_PATH="$LOCAL_GUACAMOLE/lib":$LD_LIBRARY_PATH

# run program
if [[ $* == *-d* ]]
then
cd "$DIR" && gdb --args python3 ./main.py
else
cd "$DIR" && python3.5 ./main.py
fi

# kill daemon
kill %1
