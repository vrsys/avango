#!/bin/bash

#avango + python
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/avango/vr_application_lib:/opt/boost/current/lib:/opt/openscenegraph/3.0.1/lib64/:/opt/avango/current/lib:/opt/zmq/current/lib
export PYTHONPATH=$PYTHONPATH:/opt/avango/vr_application_lib/:/opt/avango/current/lib/python2.7

# guacamole
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/guacamole/guacamole/lib:/opt/guacamole/avango-gua/lib/avango
export PYTHONPATH=$PYTHONPATH:/opt/guacamole/avango-gua/lib:/opt/guacamole/avango-gua/examples


/opt/guacamole/avango-gua/examples/examples_common/daemon.py > /dev/null &

DIR="$( cd "$( dirname "$0" )" && pwd )"
cd $DIR

./main.py

kill %1
