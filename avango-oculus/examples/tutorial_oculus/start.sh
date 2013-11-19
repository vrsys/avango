#!/bin/bash

#avango + python
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/avango/vr_application_lib:/opt/boost/current/lib:/opt/openscenegraph/3.0.1/lib64/:/home/lopa1693/CurrentFramework/avango/lib:/opt/zmq/current/lib
#export PYTHONPATH=$PYTHONPATH:/opt/avango/vr_application_lib/:/opt/avango/current/lib/python2.7

# guacamole
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/lopa1693/CurrentFramework/guacamole/lib:/home/lopa1693/CurrentFramework/avango/avango-gua/lib/avango
#export PYTHONPATH=$PYTHONPATH:/home/lopa1693/CurrentFramework/avango/avango-gua/lib:/home/lopa1693/CurrentFramework/avango/examples

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/boost/current/lib:/home/lopa1693/CurrentFramework/guacamole/lib:/home/lopa1693/CurrentFramework/avango/lib
export PYTHONPATH=$PYTHONPATH:/home/lopa1693/CurrentFramework/avango/lib:/home/lopa1693/CurrentFramework/avango/lib/avango:/home/lopa1693/CurrentFramework/avango/examples

/home/lopa1693/CurrentFramework/avango/examples/examples_common/daemon.py > /dev/null &

DIR="$( cd "$( dirname "$0" )" && pwd )"
cd $DIR

./main.py

kill %1
