import avango
import avango.script
import pygtk
pygtk.require('2.0')
import gtk
import sys
import cStringIO
import subprocess
import os
import tempfile
import inspect

import avango.nodefactory
nodes = avango.nodefactory.NodeFactory(module=__name__)

def _edit(data = ""):
    result = data
    editor = os.environ.get('AVANGO_EDITOR', '')
    if not editor:
        return result
    try:
        buffer = tempfile.mkstemp(suffix = '.py')
        os.write(buffer[0], data)
        os.close(buffer[0])
        subprocess.call([editor, buffer[1]])
        file = open(buffer[1])
        result = file.read()
    finally:
        os.unlink(buffer[1])
    return result

def _remove_indent(block):
    lines = block.splitlines(True)
    min_indent = min([len(x)-len(x.lstrip()) for x in lines if x.lstrip()])
    stripped_lines = []
    for x in lines:
        if not x.lstrip():
            stripped_lines.append('\n')
            continue
        stripped_lines.append(x[min_indent:])
    return ''.join(stripped_lines)

class Instances(object):
    def __getattr__(self, name):
        return avango.get_instance_by_name(name)

class Inspector(avango.script.Script):

    Children = avango.MFContainer()

    def __init__(self):
        self.always_evaluate(True)

        avango.script.register_exception_handler(self._handle_exception)

        self.sandbox = { 'inst': Instances(), 'edit': self._edit }
        self._sources = {}
        self._sources_prefix = "<internal_buffer>:"

        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.set_title("AVANGO Inspector")
        self.window.set_size_request(400, 400)
        vbox = gtk.VBox()
        self.window.add(vbox)

        scrolled_instances = gtk.ScrolledWindow()
        scrolled_instances.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_ALWAYS)
        scrolled_instances.add(self._create_treeview())
        self.view.connect("cursor-changed", self._handle_instance_select, None)

        scrolled_fields = gtk.ScrolledWindow()
        scrolled_fields.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_ALWAYS)
        scrolled_fields.add(self._create_fields_view())

        viewers = gtk.HPaned()
        viewers.props.position = 200
        viewers.pack1(scrolled_instances)
        viewers.pack2(scrolled_fields)

        self.update_model()

        paned = gtk.VPaned()
        paned.props.position = 300
        paned.pack1(viewers)

        output_viewport = gtk.ScrolledWindow()
        output_viewport.set_policy(gtk.POLICY_NEVER, gtk.POLICY_ALWAYS)
        self.output = gtk.TextBuffer()
        self.output_command_tag = self.output.create_tag(foreground="blue")
        self.output_error_tag = self.output.create_tag(foreground="red")
        textview = gtk.TextView(buffer=self.output)
        textview.props.editable = False
        textview.props.cursor_visible = False
        output_viewport.add(textview)
        output_viewport.props.vadjustment.connect("changed", self._handle_output_scroll, None)
        paned.pack2(output_viewport)

        vbox.pack_start(paned)
        self.entry_field = gtk.Entry()
        self.entry_field.connect("activate", self._handle_commandline, None)
        vbox.pack_start(self.entry_field, expand=False)
        self.window.show_all()

    def _create_fields_view(self):
        self.field_model = gtk.ListStore(str, str, object)
        
        self.field_view = gtk.TreeView(self.field_model)
        name_column = gtk.TreeViewColumn('Field')
        value_column = gtk.TreeViewColumn('Value')
        
        self.field_view.append_column(name_column)
        self.field_view.append_column(value_column)
        
        name_cell = gtk.CellRendererText()
        value_cell = gtk.CellRendererText()
        
        name_column.pack_start(name_cell, True)
        value_column.pack_start(value_cell, True)
        
        name_column.add_attribute(name_cell, 'text', 0)
        value_column.add_attribute(value_cell, 'text', 1)

        return self.field_view

    def _create_treeview(self):
        self.model = gtk.TreeStore(str, str, object)
        
        self.view = gtk.TreeView(self.model)
        name_column = gtk.TreeViewColumn('Name')
        value_column = gtk.TreeViewColumn('Value')
        
        self.view.append_column(name_column)
        self.view.append_column(value_column)
        
        name_cell = gtk.CellRendererText()
        value_cell = gtk.CellRendererText()
        
        name_column.pack_start(name_cell, True)
        value_column.pack_start(value_cell, True)
        
        name_column.add_attribute(name_cell, 'text', 0)
        value_column.add_attribute(value_cell, 'text', 1)
        return self.view

    def _handle_commandline(self, widget, data=None):
        self.sandbox['nodes'] = self.Children.value
        self.sandbox['field'] = self.get_selected_field()
        self.sandbox['node'] = self.get_selected_instance()

        self._exec(self.entry_field.get_text())
        self.entry_field.set_text("")

        self.update_model()

    def _exec(self, cmd, filename = "<input>", filetype = "single"):
        self.output.insert_with_tags(self.output.get_end_iter(), cmd+"\n", self.output_command_tag)

        stdout = sys.stdout
        redirected_stdout = cStringIO.StringIO()
        sys.stdout = redirected_stdout
        stderr = sys.stderr
        redirected_stderr = cStringIO.StringIO()
        sys.stderr = redirected_stderr

        try:
            code = compile(cmd, filename, filetype)
            exec code in globals(), self.sandbox
        except:
            self._dump_exception(sys.exc_info())

        sys.stdout = stdout
        sys.stderr = stderr

        self.output.insert_with_tags(self.output.get_end_iter(), redirected_stderr.getvalue(), self.output_error_tag)
        self.output.insert(self.output.get_end_iter(), redirected_stdout.getvalue())

    def _handle_output_scroll(self, widget, data=None):
        widget.set_value(widget.upper)

    def _handle_instance_select(self, widget, data=None):
        self.update_field_model()

    def get_selected_instance(self):
        instance = None
        selection = self.view.get_selection()
        model, iter = selection.get_selected()
        if model and iter:
            instance = model.get(iter, 2)[0]
        return instance

    def get_selected_field(self, column = 2):
        field = None
        selection = self.field_view.get_selection()
        model, iter = selection.get_selected()
        if model and iter:
            field = model.get(iter, column)[0]
        return field

    def update_model(self):
        def recurse_fields(node, parent, selected):
            result = None
            if selected == node:
                result = parent
            for i in xrange(node._get_num_fields()):
                name = node._get_field_name(i)
                field = node._get_field(i)
                value = field.value
                if name != "Children":
                    continue
                for child in value:
                    new_parent_node = self.model.append(parent, [child.Name.value, name, child])
                    possible_result = recurse_fields(child, new_parent_node, selected)
                    if possible_result:
                        result = possible_result
            return result

        selected_instance = self.get_selected_instance()
        self.model.clear()
        selected_row = None
        for node in self.Children.value:
            parent = self.model.append(None, [node.Name.value, "", None])
            selected_row = recurse_fields(node, parent, selected_instance)

        self.view.expand_all()

        if selected_row:
            self.view.get_selection().select_iter(selected_row)

        self.update_field_model()

    def update_field_model(self):
        instance = self.get_selected_instance()
        if not instance:
            return

        selected_field = self.get_selected_field()
        selected_field_name = None
        if selected_field and instance == selected_field.get_container():
            selected_field_name = selected_field._get_name()

        self.field_model.clear()
        selected_row = None
        for i in xrange(instance._get_num_fields()):
            name = instance._get_field_name(i)
            field = instance._get_field(i)
            value = field.value
            field_iter = self.field_model.append([name, value, field])
            if selected_field_name == name:
                selected_row = field_iter

        if selected_row:
            self.field_view.get_selection().select_iter(selected_row)

    def _edit(self, method = None):
        if method:
            if not hasattr(method, 'im_class'):
                return
            sources_name = inspect.getfile(method)
            sources_key = self._get_sources_key(sources_name)
            if sources_key:
                source = self._sources[sources_key]
            else:
                source = inspect.getsource(method)
                sources_name = self._get_sources_name()
                sources_key = self._get_sources_key(sources_name)

            source = _edit(source)
            self.output.insert_with_tags(self.output.get_end_iter(), source+"\n", self.output_command_tag)

            env = {}
            clean_source = _remove_indent(source)
            code = compile(clean_source, sources_name, "exec")
            exec code in method.im_func.func_globals, env

            # FIXME this ugliness is due to the way avango.script works.
            cls = method.im_self._get_object().__class__
            setattr(cls, method.__name__, env[method.__name__])

            self._sources[sources_key] = source

        else:
            cmds = _edit()
            self._exec(cmds, filetype="exec")

    def _get_sources_name(self):
        return self._sources_prefix+str(len(self._sources)+1)

    def _get_sources_key(self, name):
        if not name.startswith(self._sources_prefix):
            return 0
        return int(name[len(self._sources_prefix):])

    def _handle_exception(self, cls, obj, traceback):
        self._dump_exception((cls, obj, traceback))

    def _dump_exception(self, exc_info):
        cls, obj, traceback = exc_info
        if cls is None:
            return
        self.output.insert_with_tags(self.output.get_end_iter(), "%s: %s\n" % (cls.__name__, str(obj)), self.output_error_tag)

    @avango.script.field_has_changed(Children)
    def children_changed(self):
        self.update_model()

    def evaluate(self):
        while gtk.events_pending():
            gtk.main_iteration()

