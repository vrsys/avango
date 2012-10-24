import avango
import avango.osg
import avango.display
import sys

import avango.vrpn

argv = avango.display.init(sys.argv)
view = avango.display.make_view()
view.EnableTrackball.value = True

vrpn_node = avango.vrpn.nodes.Node()
vrpn_node.always_evaluate(True)

position = avango.vrpn.position_node(vrpn_node,34,34,0)

view.Root.value = vrpn_node

avango.display.run()
