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
##########################################################################
import avango
import avango.osg
import avango.display

from _tree_model import *

from PySide import QtCore, QtGui

import sys
import random


def update_field_container_model(model, rootNode):
    """
    Updates the given model
    """
    def update_tree_model(node, tree_item_parent):
        for i in xrange(node._get_num_fields()):
            name = node._get_field_name(i)
            field = node._get_field(i)
            value = field.value
            
            n = TreeItem([str(name), str(value)], tree_item_parent)
            tree_item_parent.appendChild(n)
            
            if name != "Children":
                continue
            
            for child in value:
                new_parent_node = TreeItem([child.Name.value, name],n)
                n.appendChild(new_parent_node)
                update_tree_model(child,new_parent_node)
            
    rootItem = TreeItem([rootNode.Name.value, "Root"],model.rootItem)
    model.rootItem.appendChild(rootItem)
    for child in rootNode.Children.value:
        new_parent_node = TreeItem([child.Name.value, ""],rootItem)
        rootItem.appendChild(new_parent_node)
        update_tree_model(child, new_parent_node)

    model.reset()  
    

class QTMainWindow(QtGui.QMainWindow):
    
    signal_exit = QtCore.Signal()
    
    def __init__(self, parent=None):
        super(QTMainWindow, self).__init__()

        self._create_widgets()
        
        self.setWindowTitle(self.tr("AVANGO FieldContainers"))
        #set window size and position
        self.resize(500, 500)
        self.move(604,0)
        
    def _create_widgets(self):
        widget = QtGui.QWidget() 
        self.fieldsLayout = QtGui.QVBoxLayout()
        widget.setLayout(self.fieldsLayout)
        
        self._view = QtGui.QTreeView()
        self.model = TreeModel()
        self._view.setModel(self.model)
        self.fieldsLayout.addWidget(self._view)
        
        self._exit_button = QtGui.QPushButton("Exit")
        self._exit_button.released.connect(self._slot_exit_button_released)
        self.fieldsLayout.addWidget(self._exit_button)
        
        self.setCentralWidget(widget)
        
    def updateTreeNodeModel(self,root):
        update_field_container_model(self.model, root)
        
    def _slot_exit_button_released(self):
        self.signal_exit.emit()
        
    def closeEvent(self, event):
        self.signal_exit.emit()
    
class QTApplicationConnector(avango.script.Script):

    Root = avango.osg.SFGroup()

    def __init__(self):
        self.super(QTApplicationConnector).__init__()
        
        self.always_evaluate(True)

        self.app = QtGui.QApplication(sys.argv)
        
        self.main_window = QTMainWindow()
        self.main_window.show()
        self.main_window.signal_exit.connect(self.slot_exit)

    @avango.script.field_has_changed(Root)
    def children_changed(self):
        self.main_window.updateTreeNodeModel(self.Root.value)

    def evaluate(self):
        if self.app.hasPendingEvents():
            self.app.processEvents()
            
    def slot_exit(self):
        avango.display.done()
        

argv = avango.display.init(sys.argv)
view = avango.display.make_view("")
view.EnableTrackball.value = True
#set the window size and position
window = avango.display.get_window("")
window.WantedWidth.value = 500
window.WantedHeight.value = 500
window.WantedPositionX.value = 100
window.WantedPositionY.value = 0

root = avango.osg.nodes.Group(Name="RootNode")
view.Root.value = root

#add 2 spheres
sphere1 = avango.osg.nodes.Sphere(Name="RedSphere", Color=avango.osg.Vec4(1., 0., 0., 1))
sphere1.Matrix.value = avango.osg.make_trans_mat(1, 0, 0)

sphere2 = avango.osg.nodes.Sphere(Name="GreenSphere", Color=avango.osg.Vec4(0., 1., 0., 1))
sphere2.Matrix.value = avango.osg.make_trans_mat(-1, 0, 0)

root.Children.value = [ sphere1, sphere2]

#create an QT app runner
qt_runner = QTApplicationConnector()
qt_runner.Root.value = root

avango.display.run()