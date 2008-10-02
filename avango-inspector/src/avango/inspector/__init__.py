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
        self.window.add(self._create_treeview())
        self.window.show_all()

    def _create_treeview(self):
        self.model = gtk.TreeStore(str, str)
        self.update_model()
        
        view = gtk.TreeView(self.model)
        name_column = gtk.TreeViewColumn('Name')
        value_column = gtk.TreeViewColumn('Value')
        
        view.append_column(name_column)
        view.append_column(value_column)
        
        name_cell = gtk.CellRendererText()
        value_cell = gtk.CellRendererText()
        
        name_column.pack_start(name_cell, True)
        value_column.pack_start(value_cell, True)
        
        name_column.add_attribute(name_cell, 'text', 0)
        value_column.add_attribute(value_cell, 'text', 1)

        return view

    def update_model(self):
        def recurse_fields(node, parent):
            for i in xrange(node._get_num_fields()):
                name = node._get_field_name(i)
                value = node._get_field(i).value
                if name != "Children":
                    self.model.append(parent, [name, value])
                else:
                    new_parent = self.model.append(parent, [name, ""])
                    for child in value:
                        recurse_fields(child, new_parent)

        self.model.clear()
        for node in self.Children.value:
            parent = self.model.append(None, [node.Name.value, ""])
            recurse_fields(node, parent)

    @avango.script.field_has_changed(Children)
    def children_changed(self):
        self.update_model()

    def evaluate(self):
        gtk.main_iteration_do(block=False)

