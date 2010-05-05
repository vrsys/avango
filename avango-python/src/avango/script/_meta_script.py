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
    def __init__(cls, classname, bases, classdict):
        super(ScriptMetaclass, cls).__init__(classname, bases, classdict)

        wrapped_bases = []
        for base in bases:
            if base == object:
                base = _script._Script
            else:
                base = base._wrapper
            wrapped_bases.append(base)
        if len(wrapped_bases) != 1:
            raise "Unsupported multiple inheritance"
        wrapped_base = wrapped_bases[0]
        class Wrapper(wrapped_base):
            pass
        cls._wrapper = Wrapper

        for name, attribute in classdict.iteritems():
            setattr(Wrapper, name, attribute)

        def create():
            return Wrapper()
        mangled_classname = avango.nodefactory.mangle_class_name(classdict['__module__'], classname)
        Wrapper._type = _script._create_type(mangled_classname, create)

        def __new(cls, **args):
            return cls._wrapper()
        setattr(cls, '__new__', staticmethod(__new))

class Script(object):
    __metaclass__ = ScriptMetaclass

    def __init__(self):
        self.super(Script, self).__init__(self._type)

    @staticmethod
    def super(cls, inst):
        'Replacement for built-in super to be used by subclasses'
        return super(cls._wrapper, inst)
