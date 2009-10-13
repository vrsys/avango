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

import avango
import avango.script
import avango.nodefactory
import _script
import avango._avango


def field_has_changed(field):
    class container(object):
        def __init__(self, field):
            self.field = field
        def __call__(self, function):
            function.field_has_changed_field = self.field
            return function
    if isinstance(field, avango.Field):
        return container(field)
    else:
        field.field_has_changed_field = None
        return field


_nodes = avango.nodefactory.NodeFactory()
def _Script_new(cls, **args):
    return getattr(_nodes, cls.__name__)(**args)

class MetaScript(type):
    def __new__(cls, classname, bases, classdict):
        newdict = {}
        fields = {}
        callbacks = {}

        # Extract fields from given class
        for name, obj in classdict.iteritems():
            if isinstance(obj, avango.Field):
                fields[obj] = name
            newdict[name] = obj

        # Extract fields from base classes. We do not need to care for class
        # appearing more than once as we store the fields by reference in a
        # directory and thus each field only once.
        for b in bases:
            for class_ in b.__mro__:
               for name, obj in vars(class_).iteritems():
                   if isinstance(obj, avango.Field):
                       fields[obj] = name

        # Collect callbacks from given class
        for name, obj in classdict.iteritems():
            try:
                if not obj.field_has_changed_field:
                    callbacks[None] = name
                callbacks[fields[obj.field_has_changed_field]] = name
            except:
                pass

        # Collect callbacks from base classes (see above for class appearing
        # multiple times in bases.__mro__)
        for b in bases:
            for class_ in b.__mro__:
                for name, obj in vars(class_).iteritems():
                    try:
                        if not obj.field_has_changed_field and None not in callbacks:
                            callbacks[None] = name
                        callbacks[fields[obj.field_has_changed_field]] = name
                    except:
                        pass

        # Create new class
        newdict['__new__'] = _Script_new
        mangled_classname = avango.nodefactory.mangle_class_name(classdict['__module__'], classname)
        newclass = type.__new__(cls, mangled_classname, bases, newdict)

        # Register class with Avango
        fields = [ (b,a) for (a,b) in fields.items() ]
        _script._create_type(mangled_classname, newclass, fields, callbacks)

        return newclass


class Script(object):
    'Base class for all script nodes'
    __metaclass__ = MetaScript

    def __init__(self):
        pass

    @classmethod
    def _new(cls):
        'Access to standard __new__ method overriden by metaclass'
        return super(Script, cls).__new__(cls)

    def evaluate(self):
        pass

    def init_super(self, class_):
        class_.__init__.im_func(self)

    def super(self, cls = None):
        class SuperWrapper(object):
            script = None
            def __getattribute__(self, name):
                script = object.__getattribute__(self, 'script')
                if not script:
                    raise AttributeError, 'Super class not initialized'
                if cls:
                    script_class = cls
                else:
                    script_class = script._get_object().__class__
                mro = script._get_object().__class__.__mro__
                # FIXME the following loop is a reimplementation of mro.index, which does not work
                index = 0
                for i in xrange(len(mro)):
                    if script_class == mro[i]:
                        index = i+1
                        break
                for class_ in mro[index:]:
                    if class_ == script_class:
                        continue
                    if name not in class_.__dict__:
                        continue
                    attribute = class_.__dict__[name]
                    return type(attribute).__get__(attribute, script, _script._Script)
                raise AttributeError, 'No attribute '+name+' in super class'
        result = SuperWrapper()
        result.script = self
        return result


def Script_getattr(self, name):
    # TODO this is slower than it should be. Maybe seal class at
    # initialization time and use properties.
    field = self._get_field(name)
    if field is not None:
        return field
    self_object = self._get_object()
    base_attribute = None
    for class_ in self_object.__class__.__mro__:
        if name in class_.__dict__:
            v = class_.__dict__[name]
            v_type = type(v)
            if hasattr(v_type, '__get__'):
                 if hasattr(v_type, '__set__'):
                     return v_type.__get__(v, self, _script._Script)
                 v = v_type.__get__(v, self, _script._Script)
            if not base_attribute:
                base_attribute = v
    if name in self_object.__dict__:
        return self_object.__dict__[name]
    if base_attribute:
        return base_attribute
    return getattr(self_object, name)

def Script_setattr(self, name, value):
    if self._get_field(name) is not None:
        raise AttributeError, 'Field %s not overridable' % name
    setattr(self._get_object(), name, value)

_script._Script.__getattr__ = Script_getattr
_script._Script.__setattr__ = Script_setattr
