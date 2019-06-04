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
import sys
import avango
import avango.script
import avango.gua
import avango.gua.lod
from avango.script import field_has_changed

from examples_common.GuaVE import GuaVE

#avango.enable_logging(4, "client.log")

## print the subgraph under a given node to the console
def print_graph(root_node):
    stack = [(root_node, 0)]
    while stack:
        node, level = stack.pop()
        print("│   " * level + "├── {0} <{1}>".format(
          node.Name.value, node.__class__.__name__))
        stack.extend(
          [(child, level + 1) for child in reversed(node.Children.value)])

class NetInit(avango.script.Script):
    NetChildren = avango.gua.MFNode()
    WindowName = avango.SFString()
    Viewer = avango.gua.nodes.Viewer()

    def __init__(self):
      self.super(NetInit).__init__()
      self.always_evaluate(True)
      self.is_initialized = False

    def my_constructor(self, scenegraph, atlas_path, vt_backend):
      self.scenegraph = scenegraph
      self.atlas_path = atlas_path
      self.vt_backend = vt_backend
      # print('setup!')

    def evaluate(self):
      if len(self.NetChildren.value) > 0 and self.is_initialized:
        node = self.scenegraph["/net/multi_view_trans_node"]

        
        # print(node.Name.value)
        # print(node.WorldTransform.value)
        #node.clear_vertices()
        #node.push_vertex(1, 0, 0, 1, 0, 0)
        #node.push_vertex(-1, 0, 0, 1, 0, 0)

    @field_has_changed(NetChildren)
    def update(self):      
      if len(self.NetChildren.value) > 0 and not self.is_initialized:
        # print("JAAAA  ")
        print_graph(self.scenegraph.Root.value)
        node = self.scenegraph["/net/multi_view_trans_node/multi_window_vis"]
        if node:
          print(node.Name.value)
          # print('VT flag',node.Material.value.EnableVirtualTexturing.value )
          # print("!!!!!!!!!!!!", node.Material.value.m_serializedUniforms.value)
          #node.Material.value.EnableVirtualTexturing.value = True
          node.Material.value.set_uniform("vt_images", atlas_path)
        # print('######################', node.Name.value)
        # node.RenderVolumetric.value = False
        self.is_initialized = True
        cam = self.scenegraph["/net/client_navigation/client_cam"]
        vt_backend.add_camera(cam)
        vt_backend.start_backend()

if __name__ == '__main__':
  user_id = 0
  if len(sys.argv) == 3:
    task = str(sys.argv[1])
    part = int(sys.argv[2])
    if task == 'points':
        if part == 1 or part == 4:
            study_geo_version = 1 if part == 1 else 2
            if study_geo_version == 1:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-terra-version1/tp_terra_version1_tex.atlas"
            elif study_geo_version == 2:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-terra-version2/tp_terra_version2_tex.atlas"

        elif part == 2 or part == 5:
            study_geo_version = 1 if part == 2 else 2
            if study_geo_version == 1:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-head-version1/tp_head_version1_tex.atlas"
            elif study_geo_version == 2:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-head-version2/tp_head_version2_tex.atlas"

        elif part == 3 or part == 6:
            study_geo_version = 1 if part == 3 else 2            
            if study_geo_version == 1:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-wappen-version1/tp_wappen_version1_tex.atlas"
            elif study_geo_version == 2:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-points/tp-wappen-version2/tp_wappen_version2_tex.atlas"
    if task == 'real':
        if part == 1 or part == 3:
            study_geo_version = 1 if part == 1 else 2
            
            if study_geo_version == 1:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-real/tr-head-version1/tr_head_version1_tex.atlas"
            elif study_geo_version == 2:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-real/tr-head-version2/tr_head_version2_tex.atlas"

        elif part == 2 or part == 4:
            study_geo_version = 1 if part == 2 else 2
            
            if study_geo_version == 1:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-real/tr-wappen-version1/tr_wappen_version1_tex.atlas"
            elif study_geo_version == 2:
                atlas_path = "/home/senu8384/Desktop/master-thesis/data/study/task-real/tr-wappen-version2/tr_wappen_version2_tex.atlas"


    nettrans = avango.gua.nodes.NetTransform(Name="net",
                                             # specify role, ip, and port
                                             # Groupname="AVCLIENT|141.54.147.59|7432")
                                             Groupname="AVCLIENT|141.54.147.60|7432")
                                             # Groupname="AVCLIENT|127.0.0.1|7432")
    
    graph = avango.gua.nodes.SceneGraph(Name="scenegraph")
    graph.Root.value.Children.value = [nettrans]

    size = avango.gua.Vec2ui(7680, 4320)
    # window = avango.gua.nodes.GlfwWindow(Size=size,
    window = avango.gua.nodes.Window(Size=size,
                                     LeftResolution=size,
                                     Title="client_window")
    avango.gua.register_window("client_window", window)

    logger = avango.gua.nodes.Logger(EnableWarning=False)

    vt_backend = avango.gua.VTBackend()

    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [graph]
    viewer.Windows.value = [window]

    init = NetInit()
    init.my_constructor(graph, atlas_path, vt_backend)
    init.WindowName.value = "client_window"
    init.Viewer = viewer
    init.NetChildren.connect_from(nettrans.Children)

    guaVE = GuaVE()
    guaVE.start(locals(), globals())

    viewer.run()
  else:
      print("Restart client with - ./cleint.sh #task #part ")

