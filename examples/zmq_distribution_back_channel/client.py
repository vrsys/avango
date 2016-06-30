# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################
'''
A distributed viewer setup: This Python scripts starts an
avango.osg.simpleviewer and connects to the distribution group "testgroup". If
there is another instance loading some geometry under this node this model
should also appear in the client.  (see also simpleviewer-srv.py)
'''

import avango
import avango.script
import avango.gua
from avango.script import field_has_changed
import asyncio
import pickle
import sys

from examples_common.GuaVE import GuaVE

#avango.enable_logging(4, "client.log")

SERVER_IP = '127.0.0.1'


class TimedRotate(avango.script.Script):
    TimeIn = avango.SFFloat()
    MatrixOut = avango.gua.SFMatrix4()

    @field_has_changed(TimeIn)
    def update(self):
        self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value * 2.0,
                                                       0.0, 1.0, 0.0)


class NetInit(avango.script.Script):
    NetChildren = avango.gua.MFNode()
    WindowName = avango.SFString()
    Viewer = avango.gua.nodes.Viewer()
    """
  @field_has_changed(NetChildren)
  def update(self):
    for tmp in self.NetChildren.value:
      for child in tmp.Children.value:
        if isinstance(child, avango.gua.CameraNode):
          if child.OutputWindowName.value == self.WindowName.value:
            # self.Viewer.CameraNodes.value = [child]
            pass
  """


nettrans = avango.gua.nodes.NetTransform(
    Name="net",
    # specify role, ip, and port
    Groupname="AVCLIENT|" + SERVER_IP + "|7432")

timed_rotate = TimedRotate()
timer = avango.nodes.TimeSensor()
timed_rotate.TimeIn.connect_from(timer.Time)

loader = avango.gua.nodes.TriMeshLoader()

graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
graph.Root.value.Children.value = [nettrans]

size = avango.gua.Vec2ui(800, 600)

window = avango.gua.nodes.GlfwWindow(Size=size,
                                     LeftResolution=size,
                                     Title="client_window")
avango.gua.register_window("client_window", window)

logger = avango.gua.nodes.Logger(EnableWarning=False)

viewer = avango.gua.nodes.Viewer()
viewer.SceneGraphs.value = [graph]
viewer.Windows.value = [window]

init = NetInit()
init.WindowName.value = "client_window"
init.Viewer = viewer
init.NetChildren.connect_from(nettrans.Children)

#guaVE = GuaVE()
#guaVE.start(locals(), globals())


@asyncio.coroutine
def viewer_loop():
    while True:
        viewer.frame()
        yield from asyncio.sleep(0.01)


@asyncio.coroutine
def send_matrix(host, port, loop, timed_rotate):
    print("try connecting")
    while True:
        try:
            reader, writer = yield from asyncio.open_connection(host=host,
                                                                port=port)
        except ConnectionRefusedError:
            print("server " + host + ":" + str(port) + " not online, waiting")
            yield from asyncio.sleep(3)
            continue
        break

    print("connected to " + host + ":" + str(port))
    while True:
        try:
            #matrix = avango.gua.make_rot_mat(loop.time() * 2.0, 0.0, 1.0, 0.0)
            matrix = timed_rotate.MatrixOut.value

            writer.write(pickle.dumps(avango.gua.to_list(matrix)))

            nr_of_bytes = 2
            response = yield from reader.read(nr_of_bytes)
            if response:
                message = response.decode()
        except ConnectionResetError:
            print("lost connection to " + host + ":" + str(port))
            writer.close()
            print("stop event loop")
            loop.stop()
            loop.close()
        except KeyboardInterrupt:
            writer.close()
            loop.stop()
            loop.close()
        except:
            print("Unexpected error:", sys.exc_info()[0])
            #raise
            break
        yield from asyncio.sleep(0.001)

# should be asyncio.ensure_future(viewer_loop()) but our version of python is
# to old
viewer_task = asyncio.gather(viewer_loop())
loop = asyncio.get_event_loop()
send_task = asyncio.gather(send_matrix(SERVER_IP, 8888, loop, timed_rotate))

try:
    loop.run_forever()
    #loop.run_until_complete(send_task)
except KeyboardInterrupt:
    pass
except:
    print("Unexpected error:", sys.exc_info()[0])

loop.stop()
loop.close()
