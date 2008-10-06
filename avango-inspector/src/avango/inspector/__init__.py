import avango
import avango.script
import pygtk
pygtk.require('2.0')
import gtk

import avango.nodefactory
nodes = avango.nodefactory.NodeFactory(module=__name__)

class Inspector(avango.script.Script):

    Children = avango.MFContainer()

    def __init__(self):
        self.always_evaluate(True)

        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.set_title("AVANGO Inspector")
        vbox = gtk.VBox()
        self.window.add(vbox)
        vbox.pack_start(self._create_treeview())
        commandline = gtk.HBox()
        vbox.pack_start(commandline, expand=False)
        execute_button = gtk.Button("Execute")
        execute_button.connect("clicked", self.handle_execute_button, None)
        self.entry_field = gtk.Entry()
        commandline.pack_start(self.entry_field)
        commandline.pack_start(execute_button, expand=False)
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

    def handle_execute_button(self, widget, data=None):
        field = None
        selection = self.view.get_selection()
        model, iter = selection.get_selected()
        if model and iter:
            field = model.get(iter, 2)[0]
        sandbox = {'nodes': self.Children.value, 'field': field }

        exec self.entry_field.get_text() in globals(), sandbox
        self.entry_field.set_text("")
        self.update_model()

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


    @avango.script.field_has_changed(Children)
    def children_changed(self):
        self.update_model()

    def evaluate(self):
        gtk.main_iteration_do(block=False)

