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
import avango.nodefactory
import _script

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

class ScriptMetaclass(type):
    def __init__(cls, classname, bases, classdict):
        super(ScriptMetaclass, cls).__init__(classname, bases, classdict)

        wrapped_bases = []
        for base in bases:
            if base == object:
                base = _script._Script
            else:
                base = base._Script__wrapper
            wrapped_bases.append(base)
        if len(wrapped_bases) != 1:
            raise "Unsupported multiple inheritance"
        wrapped_base = wrapped_bases[0]
        class Wrapper(wrapped_base):
            pass
        cls._Script__wrapper = Wrapper

        Wrapper._Script__fields = list(getattr(base, '_Script__fields', []))
        Wrapper._Script__field_has_changed = dict(getattr(base, '_Script__field_has_changed', {}))
        for name, attribute in classdict.iteritems():
            if isinstance(attribute, avango.Field):
                Wrapper._Script__fields.append( (name, attribute) )
                continue

            if hasattr(attribute, "field_has_changed_field"):
                field = attribute.field_has_changed_field
                Wrapper._Script__field_has_changed[field] = attribute

            setattr(Wrapper, name, attribute)

        def create():
            return Wrapper()
        mangled_classname = avango.nodefactory.mangle_class_name(classdict['__module__'], classname)
        Wrapper._Script__type = _script._create_type(mangled_classname, create)

        def __new(cls, **args):
            result = cls._Script__wrapper()
            for name, value in args.iteritems():
                getattr(result, name).value = value
            return result
        setattr(cls, '__new__', staticmethod(__new))

class Script(object):
    __metaclass__ = ScriptMetaclass

    def __init__(self):
        self.super(Script, self).__init__(self._Script__type)

        # Fields that are added are clones of the prototype given in the class
        # definition. We therefore need to transform the dictionary mapping
        # fields to registered callbacks
        transformed_field_has_changed = {}

        for name, field in self._Script__fields:
            self.add_field(field, name)
            if field in self._Script__field_has_changed:
                transformed_field_has_changed[self._get_field(name)] = self._Script__field_has_changed[field]

        if None in self._Script__field_has_changed:
            transformed_field_has_changed[None] = self._Script__field_has_changed[None]

        self._Script__field_has_changed = transformed_field_has_changed

        if self._Script__field_has_changed:
            self._Script__enable_field_has_changed()

    @staticmethod
    def super(cls, inst):
        'Replacement for built-in super to be used by subclasses'
        return super(cls._Script__wrapper, inst)

    def evaluate(self):
        pass

    def _Script__fieldHasChanged(self, field):
        if field in self._Script__field_has_changed:
            self._Script__field_has_changed[field](self)
        if None in self._Script__field_has_changed:
            self._Script__field_has_changed[None](self, field._get_name(), field)
