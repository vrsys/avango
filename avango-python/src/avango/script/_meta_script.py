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

import avango.nodefactory
import _script

class ScriptMetaclass(type):
    def __init__(cls, name, bases, dict):
        super(ScriptMetaclass, cls).__init__(name, bases, dict)

        class Wrapper(_script._Script):
            def __init__(self):
                super(Wrapper, self).__init__(self._type)

        def create():
            return Wrapper()
        mangled_classname = avango.nodefactory.mangle_class_name(dict['__module__'], name)
        Wrapper._type = _script._create_type(mangled_classname, create)


class Script(object):
    __metaclass__ = ScriptMetaclass
