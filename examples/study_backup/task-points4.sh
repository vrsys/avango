#!/bin/bash

# get directory of script
DIR="$( cd "$( dirname "$0" )" && pwd )"

# assuming a local guacmole version is located properly
LOCAL_GUACAMOLE="$DIR/../../../guacamole"
LOCAL_AVANGO="$DIR/../../../avango"

# if not, this path will be used
GUACAMOLE=/opt/guacamole/master
AVANGO=/opt/avango/master

# third party libs
export LD_LIBRARY_PATH=/opt/boost/current/lib:/opt/zmq/current/lib

# schism
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/schism/current/lib/linux_x86

# avango
export LD_LIBRARY_PATH="$LOCAL_AVANGO/lib":$AVANGO/lib:$LD_LIBRARY_PATH:/home/ephtron/Projects/lamure/install/lib:/opt/Awesomium/lib
# avango
export LD_LIBRARY_PATH="$LOCAL_AVANGO/lib":$AVANGO/lib:$LD_LIBRARY_PATH:/home/senu8384/Desktop/master-thesis/lamure/install/lib:/opt/Awesomium/lib
export PYTHONPATH="$LOCAL_AVANGO/lib/python3.5":"$LOCAL_AVANGO/examples":$AVANGO/lib/python3.5:$AVANGO/examples

# guacamole
export LD_LIBRARY_PATH="$LOCAL_GUACAMOLE/lib":$GUACAMOLE/lib:$LD_LIBRARY_PATH


if [[ $* == *-d* ]]
then
echo "starting daemon"
python3 daemon.py
else
echo "starting daemon && application"
python3 ./daemon.py > /dev/null &

cd "$DIR" && DISPLAY=:0.0 python3 ./main.py 'points' 4 $1
fi

# kill daemon & client
kill %1
kill %2