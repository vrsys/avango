# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       #
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

import avango.osg
import avango.osg.simpleviewer
import avango.script

from elasticnodes import *
import sys
import random
from PySide import QtCore, QtGui
#from PyQt4 import QtCore, QtGui


class TreeItem:
    def __init__(self, data, parent=None):
        self.parentItem = parent
        self.itemData = data
        self.childItems = []

    def appendChild(self, item):
        self.childItems.append(item)

    def child(self, row):
        return self.childItems[row]

    def childCount(self):
        return len(self.childItems)

    def columnCount(self):
        return len(self.itemData)

    def data(self, column):
        return self.itemData[column]

    def parent(self):
        return self.parentItem

    def row(self):
        if self.parentItem:
            return self.parentItem.childItems.index(self)

        return 0


class TreeModel(QtCore.QAbstractItemModel):
    def __init__(self, columnDesc, parent=None):
        QtCore.QAbstractItemModel.__init__(self, parent)

        self.columnDesc = []
        for desc in columnDesc:
            self.columnDesc.append(QtCore.QVariant(desc))

        self.rootItem = TreeItem(self.columnDesc)

    def clear(self):
         self.rootItem = TreeItem(self.columnDesc)

    def columnCount(self, parent):
        if parent.isValid():
            return parent.internalPointer().columnCount()
        else:
            return self.rootItem.columnCount()

    def data(self, index, role):
        if not index.isValid():
            return QtCore.QVariant()

        if role != QtCore.Qt.DisplayRole:
            return QtCore.QVariant()

        item = index.internalPointer()

        return QtCore.QVariant(item.data(index.column()))

    def flags(self, index):
        if not index.isValid():
            return QtCore.Qt.ItemIsEnabled

        return QtCore.Qt.ItemIsEnabled | QtCore.Qt.ItemIsSelectable

    def headerData(self, section, orientation, role):
        if orientation == QtCore.Qt.Horizontal and role == QtCore.Qt.DisplayRole:
            return self.rootItem.data(section)

        return QtCore.QVariant()

    def index(self, row, column, parent):
        if row < 0 or column < 0 or row >= self.rowCount(parent) or column >= self.columnCount(parent):
            return QtCore.QModelIndex()

        if not parent.isValid():
            parentItem = self.rootItem
        else:
            parentItem = parent.internalPointer()

        childItem = parentItem.child(row)
        if childItem:
            return self.createIndex(row, column, childItem)
        else:
            return QtCore.QModelIndex()

    def parent(self, index):
        if not index.isValid():
            return QtCore.QModelIndex()

        childItem = index.internalPointer()
        parentItem = childItem.parent()

        if parentItem == self.rootItem:
            return QtCore.QModelIndex()

        return self.createIndex(parentItem.row(), 0, parentItem)

    def rowCount(self, parent):
        if parent.column() > 0:
            return 0

        if not parent.isValid():
            parentItem = self.rootItem
        else:
            parentItem = parent.internalPointer()

        return parentItem.childCount()

#    def setupModelData(self, rootNode):
#        print "setupModelData"
#
#        def update_tree_model(node,tree_item_parent):
#            for i in xrange(node._get_num_fields()):
#                name = node._get_field_name(i)
#                field = node._get_field(i)
#                value = field.value
#                if name != "Children":
#                    continue
#
#                for child in value:
#                    new_parent_node = TreeItem([child.Name.value, name],tree_item_parent)
#                    tree_item_parent.appendChild(new_parent_node)
#                    update_tree_model(child,new_parent_node)
#
#
#        for child in rootNode.value:
#            new_parent_node = TreeItem([child.Name.value, "RootChild"],self.rootItem)
#            self.rootItem.appendChild(new_parent_node)
#            update_tree_model(child, new_parent_node)
#
#        self.reset()

class NodeTreeModel(TreeModel):

    def __init__(self, columnDesc, parent=None):
        super(NodeTreeModel,self).__init__(columnDesc,parent)


    def setupModelData(self, rootNode):
        print "setupModelData"

        def update_tree_model(node,tree_item_parent):
            for i in xrange(node._get_num_fields()):
                name = node._get_field_name(i)
                field = node._get_field(i)
                value = field.value
                if name != "Children":
                    continue

                for child in value:
                    new_parent_node = TreeItem([child.Name.value, name],tree_item_parent)
                    tree_item_parent.appendChild(new_parent_node)
                    update_tree_model(child,new_parent_node)


        for child in rootNode.value:
            new_parent_node = TreeItem([child.Name.value, "RootChild"],self.rootItem)
            self.rootItem.appendChild(new_parent_node)
            update_tree_model(child, new_parent_node)

        self.reset()

def createGraphNodes(rootNode,graphWidget):

    def update_tree_model(node, tree_item_parent, graphWidget, items):

        for i in xrange(node._get_num_fields()):
            name = node._get_field_name(i)
            field = node._get_field(i)
            value = field.value
            if name != "Children":
                continue

            for child in value:
                #create new node
                actual_node = Node(graphWidget, child.Name.value, child)
                random.seed(42)
                x = random.randint(0,75)
                y = random.randint(0,75)
                actual_node.setPos(x, y)
                #add to list
                items.append(actual_node)
                items.append(Edge(tree_item_parent, actual_node))

                update_tree_model(child,actual_node,graphWidget,items)

    items = []
    parent_node = Node(graphWidget,"Root",rootNode)
    parent_node.setPos(50, 50)

    items.append(parent_node)
    for child in rootNode.value:
        update_tree_model(child, parent_node, graphWidget, items)

    return items

class ScenegraphTreeNodeWidget(QtGui.QWidget):

    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)




        self.createNodeWidgets()

        self.fieldsWidget = QtGui.QWidget()
        self.fieldsLayout = QtGui.QVBoxLayout()
        self.fieldsWidget.setLayout(self.fieldsLayout)
        self.createFieldWidgets()

        self.globalLayout = QtGui.QHBoxLayout()
        self.globalLayout.addWidget(self.nodesWidget)
        self.globalLayout.addWidget(self.fieldsWidget)
        self.setLayout(self.globalLayout)



    def createNodeWidgets(self):

        self.nodeTabWidget = QtGui.QTabWidget()

        self.nodesWidget = QtGui.QWidget()
        self.nodesLayout = QtGui.QVBoxLayout()
        self.nodesWidget.setLayout(self.nodesLayout)

        self.nodeRefresh = QtGui.QPushButton("Refresh", self.nodesWidget)

        self.nodeGraphView = QtGui.QTreeView()
        self.nodeModel = NodeTreeModel(["Nodes"])
        self.nodeGraphView.setModel(self.nodeModel)



        self.graphWidget = GraphWidget()
        QtGui.QWidget.connect(self.graphWidget,
                              QtCore.SIGNAL("nodeSelected"),
                              self.updateFieldWidget)

        #add widgets to the tab widget
        self.nodeTabWidget.addTab(self.graphWidget,"Spring graph view")
        self.nodeTabWidget.addTab(self.nodeGraphView,"Tree view")


        self.nodesLayout.addWidget(self.nodeTabWidget)
        self.nodesLayout.addWidget(self.nodeRefresh)


        #self.nodesLayout.addWidget(self.graphWidget)

        self.scene = self.graphWidget.scene



    def createFieldWidgets(self):

        self.fieldsGraphView = QtGui.QTreeView()
        self.fieldsModel = QtGui.QStandardItemModel(0, 2)
        self.fieldsModel.setHeaderData(0, QtCore.Qt.Horizontal, QtCore.QVariant("Field"))
        self.fieldsModel.setHeaderData(1, QtCore.Qt.Horizontal, QtCore.QVariant("Value"))

        self.fieldsGraphView.setModel(self.fieldsModel)
        self.fieldsGraphView.setWindowTitle("Fields")

        self.fieldsLayout.addWidget(self.fieldsGraphView)



    def updateNodeTreeWidget(self, rootNode):
        #update the tree view
        self.nodeModel.clear()
        self.nodeModel.setupModelData(rootNode)
        self.nodeGraphView.expandAll()

        #update the graph view
        self.graphWidget.removeAllItems()

        #add the new items
        items = createGraphNodes(rootNode,self.graphWidget)
        self.graphWidget.addItems(items)


    def updateFieldWidget(self, node):
        self.fieldsModel.removeRows(0,self.fieldsModel.rowCount())
        self.fieldsModel.setRowCount(node._get_num_fields())

        for i in xrange(node._get_num_fields()):
            name = node._get_field_name(i)
            field = node._get_field(i)
            value = field.value

            self.fieldsModel.setData(self.fieldsModel.index(i, 0), QtCore.QVariant(str(name)))
            self.fieldsModel.setData(self.fieldsModel.index(i, 1), QtCore.QVariant(str(value)))

class EditWidget(QtGui.QWidget):

    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)

        self.textDisplayWidget = QtGui.QTextEdit()
        self.textDisplayWidget.setReadOnly(True)

        self.textEditWidget = QtGui.QLineEdit()


        self.mainLayout = QtGui.QVBoxLayout()
        self.setLayout(self.mainLayout)

        self.mainLayout.addWidget(self.textDisplayWidget)
        self.mainLayout.addWidget(self.textEditWidget)


class QTInspector(avango.script.Script):

    Children = avango.MFContainer()

    def __init__(self):
        self.always_evaluate(True)

        self.app = QtGui.QApplication(sys.argv)


        self.mainWidget = QtGui.QWidget()
        self.mainLayout = QtGui.QVBoxLayout()
        self.mainWidget.setLayout(self.mainLayout)


        self.sceneGraphWidget = ScenegraphTreeNodeWidget()
        self.editWidget = EditWidget()

        self.mainLayout.addWidget(self.sceneGraphWidget)
        self.mainLayout.addWidget(self.editWidget)


        #Refresh nodes
        QtGui.QWidget.connect(self.sceneGraphWidget.nodeRefresh,
                              QtCore.SIGNAL("clicked()"),
                              self.updateTreeNodeModel)

        #Node selected
        QtGui.QWidget.connect(self.sceneGraphWidget.nodeGraphView,
                              QtCore.SIGNAL("activated(QModelIndex)"),
                              self.updateFieldModel)


        self.mainWindow = QtGui.QMainWindow()
        self.mainWindow.setCentralWidget(self.mainWidget)
        self.mainWindow.show()

    @avango.script.field_has_changed(Children)
    def children_changed(self):
        self.updateTreeNodeModel()

    def evaluate(self):
        if self.app.hasPendingEvents():
            self.app.processEvents()

    def updateTreeNodeModel(self):
        print "updateTreeNodeModel"
        self.sceneGraphWidget.updateNodeTreeWidget(self.Children)

    def updateFieldModel(self,index):
        print "activate index: " + str(index.row()) + " " + str(index.data())


root = avango.osg.nodes.Group(Name="RootNode")
sphere1 = avango.osg.nodes.Sphere(Name="RedSphere", Color=avango.osg.Vec4(1., 0., 0., 1))
sphere1.Matrix.value = avango.osg.make_trans_mat(1, 0, 0)

sphere1b = avango.osg.nodes.Sphere(Name="RedGreenSphere", Color=avango.osg.Vec4(1., -1., 0., 1))
sphere1b.Matrix.value = avango.osg.make_trans_mat(-1, 0, 0)
sphere1.Children.value = [ sphere1b]

sphere2 = avango.osg.nodes.Sphere(Name="GreenSphere", Color=avango.osg.Vec4(0., 1., 0., 1))
sphere2.Matrix.value = avango.osg.make_trans_mat(-1, 0, 0)

root.Children.value = [ sphere1, sphere2 ]

inspector = QTInspector(Children=[root])

avango.osg.simpleviewer.run(root)
