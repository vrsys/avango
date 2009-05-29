# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# Avango is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# Avango is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Avango. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# Avango is a trademark owned by FhG.                                    #
#                                                                        #
##########################################################################

from _avango import _make_instance_by_name

def mangle_class_name(module_name, class_name=''):
    module_path = module_name.split('.')
    if module_path[0] == "avango":
        module_path[0] = "av"
    module_path = [ x for x in module_path if not x.startswith('_') ]
    module_path.append(class_name)
    return '::'.join(module_path)

class NodeFactory(object):
    def __init__(self, prefix='', module=''):
        if prefix != '':
            self._prefix = prefix
        elif module != '':
            self._prefix = mangle_class_name(module)
        else:
            self._prefix = ''
    def __getattr__(self, name):
        """Return a referred node which is callable and will acutally
           instanciate the node"""

        class ProxyClass(object):
            _name = self._prefix+name
            def __new__(self, **args):
                try:
                    node = _make_instance_by_name(self._name)
                    values = node.get_values()
                    for key, value in args.iteritems():
                        setattr(values, key, value)
                    return node
                except RuntimeError: # We don't have a expection translation ...
                    raise ImportError("Unknown node %s" % self._name)
                except ValueError:   # ... but boost does (since boost changeset 43546)
                    raise ImportError("Unknown node %s" % self._name)

        return ProxyClass
