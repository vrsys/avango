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

class Instances(object):
    def __getattr__(self, name):
        return avango.get_instance_by_name(name)

class Inspector(avango.script.Script):

    Children = avango.MFContainer()

    def __init__(self):
        self.always_evaluate(True)

        self.sandbox = { 'inst': Instances(), 'edit': self._edit }

        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.set_title("AVANGO Inspector")
        self.window.set_size_request(400, 400)
        vbox = gtk.VBox()
        self.window.add(vbox)
        scrolled_window = gtk.ScrolledWindow()
        scrolled_window.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_ALWAYS)
        scrolled_window.add(self._create_treeview())

        paned = gtk.VPaned()
        paned.props.position = 300
        paned.pack1(scrolled_window)

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

        self.update_model()
        return self.view

    def _handle_commandline(self, widget, data=None):
        field = None
        selection = self.view.get_selection()
        model, iter = selection.get_selected()
        if model and iter:
            field = model.get(iter, 2)[0]
        self.sandbox['nodes'] = self.Children.value
        self.sandbox['field'] = field

        self._exec(self.entry_field.get_text())
        self.entry_field.set_text("")

        self.update_model()

    def _exec(self, cmd):
        self.output.insert_with_tags(self.output.get_end_iter(), cmd+"\n", self.output_command_tag)

        stdout = sys.stdout 
        redirected_stdout = cStringIO.StringIO()
        sys.stdout = redirected_stdout
        stderr = sys.stderr 
        redirected_stderr = cStringIO.StringIO()
        sys.stderr = redirected_stderr

        try:
            exec cmd in globals(), self.sandbox
        except:
            cls, obj, traceback = sys.exc_info()
            print >> sys.stderr, "%s: %s" % (cls.__name__, str(obj))

        sys.stdout = stdout
        sys.stderr = stderr

        self.output.insert_with_tags(self.output.get_end_iter(), redirected_stderr.getvalue(), self.output_error_tag)
        self.output.insert(self.output.get_end_iter(), redirected_stdout.getvalue())

    def _handle_output_scroll(self, widget, data=None):
        widget.set_value(widget.upper)

    def update_model(self):
        def recurse_fields(node, parent):
            for i in xrange(node._get_num_fields()):
                name = node._get_field_name(i)
                field = node._get_field(i)
                value = field.value
                if name != "Children":
                    self.model.append(parent, [name, value, field])
                else:
                    children_node = self.model.append(parent, [name, "", field])
                    for child in value:
                        parent_node = self.model.append(children_node, [child.Name.value, "", None])
                        recurse_fields(child, parent_node)

        self.model.clear()
        for node in self.Children.value:
            parent = self.model.append(None, [node.Name.value, "", None])
            recurse_fields(node, parent)

        self.view.expand_all()

    def _edit(self):
        cmds = _edit()
        self._exec(cmds)

    @avango.script.field_has_changed(Children)
    def children_changed(self):
        self.update_model()

    def evaluate(self):
        gtk.main_iteration_do(block=False)

