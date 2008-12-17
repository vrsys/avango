import avango # Trigger type registration
import avango.nodefactory
nodes = avango.nodefactory.NodeFactory(module=__name__)

from _view import *
from _device import *
from _display import *