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
export LD_LIBRARY_PATH=/opt/boost/boost_1_55_0/lib:/opt/zmq/current/lib

# schism
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ephtron/Projects/schism/lib/linux_x86:/home/ephtron/Projects/lamure/install/lib

# avango
export LD_LIBRARY_PATH="$LOCAL_AVANGO/lib":$AVANGO/lib:$LD_LIBRARY_PATH
export PYTHONPATH="$LOCAL_AVANGO/lib/python3.5":"$LOCAL_AVANGO/examples":$AVANGO/lib/python3.5:$AVANGO/examples

# guacamole
export LD_LIBRARY_PATH="$LOCAL_GUACAMOLE/lib":$GUACAMOLE/lib:$LD_LIBRARY_PATH

## ephra st
# run daemon
# if [ -f "$LOCAL_AVANGO/examples/examples_common/daemon.py" ]
# then
#     python3.5 $LOCAL_AVANGO/examples/examples_common/daemon.py 
#     # > /dev/null &
# else
#     python3.5 $AVANGO/examples/examples_common/daemon.py > /dev/null &
# fi
## end

python3.5 $LOCAL_AVANGO/examples/examples_common/daemon.py > /dev/null &
# run daemon
#cd "$DIR" && python3 ./mouse-daemon.py > /dev/null &

# run program
cd "$DIR" && python3 ./mouse-app.py

# kill daemon
kill %1
