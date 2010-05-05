# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       #
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
# AVANGO is a trademark owned by FhG.                                    #
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

        # Script is not derived from _Script as with boost.python we can not
        # use metaclasses, which we need to be informed about new classes.
        # We thus create an identical class hierarchy, with object exchanged
        # with _script._Script.
        # For this we also need to create a new class for our given class with
        # the bases exchanged properly. As the number of base classes can be
        # varying we have to rely on exec.
        wrapped_bases = []
        for base in bases:
            if base == object:
                base = _script._Script
            else:
                base = base._Script__wrapper
            wrapped_bases.append(base)
        wrapped_code = "class %s(%s): pass" % (classname, ','.join([ 'wrapped_bases[%i]'%i for i in xrange(len(wrapped_bases)) ]))
        wrapped_locals = { 'wrapped_bases': wrapped_bases }
        exec wrapped_code in wrapped_locals
        Wrapper = wrapped_locals[classname]
        cls._Script__wrapper = Wrapper

        # Build list of all fields (including fields of bases) and
        # field_has_changes methods
        Wrapper._Script__fields = list(reduce(lambda x,y: x+y, [ getattr(base, '_Script__fields', []) for base in wrapped_bases ]))
        bases_field_has_changed = [ getattr(base, '_Script__field_has_changed', {}) for base in wrapped_bases ]
        Wrapper._Script__field_has_changed = {}
        for base_field_has_changed in bases_field_has_changed:
            Wrapper._Script__field_has_changed.update(base_field_has_changed)

        # Copy all attributes to our wrapping class. Ignore fields as these
        # will be set in class __init__ function
        for name, attribute in classdict.iteritems():
            if isinstance(attribute, avango.Field):
                Wrapper._Script__fields.append( (name, attribute) )
                continue

            if hasattr(attribute, "field_has_changed_field"):
                field = attribute.field_has_changed_field
                Wrapper._Script__field_has_changed[field] = attribute

            setattr(Wrapper, name, attribute)

        # Register AVANGO type
        def create():
            return Wrapper()
        mangled_classname = avango.nodefactory.mangle_class_name(classdict['__module__'], classname)
        Wrapper._Script__type = _script._create_type(mangled_classname, create)

        # The user-defined class must create our wrapping class
        def __new(cls, **args):
            result = cls._Script__wrapper()
            for name, value in args.iteritems():
                getattr(result, name).value = value
            return result
        setattr(cls, '__new__', staticmethod(__new))

class Script(object):
    __metaclass__ = ScriptMetaclass

    def __init__(self):
        self.super(Script).__init__(self._Script__type)

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

    def super(self, cls):
        """Replacement for built-in super to be used by subclasses.
           This is normally called like:
                self.super(SelfClass).__init__()
        """
        return super(cls._Script__wrapper, self)

    def init_super(self, cls):
        'Deprecated replacement for built-in super. Use self.super(Classname)'
        cls._Script__wrapper.__init__.im_func(self)

    def evaluate(self):
        pass

    def _Script__fieldHasChanged(self, field):
        if field in self._Script__field_has_changed:
            self._Script__field_has_changed[field](self)
        if None in self._Script__field_has_changed:
            self._Script__field_has_changed[None](self, field._get_name(), field)
