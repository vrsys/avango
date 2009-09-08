import avango.osg
import avango.osg.simpleviewer
import avango.script

import sys
from PySide import QtCore, QtGui
    
    
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
    def __init__(self, data=None, parent=None):
        QtCore.QAbstractItemModel.__init__(self, parent)

        rootData = []
        rootData.append(QtCore.QVariant("Field"))
        rootData.append(QtCore.QVariant("Value"))
        self.rootItem = TreeItem(rootData)
        self.prevRoot = self.rootItem
        

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
        
    
class ScenegraphTreeNodeWidget(QtGui.QWidget):
    
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)
        
        self.nodesWidget = QtGui.QWidget()
        self.nodesLayout = QtGui.QVBoxLayout()
        self.nodesWidget.setLayout(self.nodesLayout)
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
        self.nodeRefresh = QtGui.QPushButton("Refresh", self.nodesWidget)
        
        self.nodeGraphView = QtGui.QTreeView()
        self.nodeModel = TreeModel()
        self.nodeGraphView.setModel(self.nodeModel)
        self.nodeGraphView.setWindowTitle("SceneGraphNodes")
        
        self.nodesLayout.addWidget(self.nodeGraphView)
        self.nodesLayout.addWidget(self.nodeRefresh)
        
    def createFieldWidgets(self):
        
        self.fieldsGraphView = QtGui.QTreeView()
        self.fieldsModel = TreeModel()
        self.fieldsGraphView.setModel(self.fieldsModel)
        self.fieldsGraphView.setWindowTitle("Fields")
        
        self.fieldsLayout.addWidget(self.fieldsGraphView)
      
    def updateNodeTreeWidget(self, rootNode):
        self.nodeModel.setupModelData(rootNode)
        
        
class QTInspector(avango.script.Script):

    Children = avango.MFContainer()

    def __init__(self):
        self.always_evaluate(True)

        self.app = QtGui.QApplication(sys.argv)
        
        self.sceneGraphWidget = ScenegraphTreeNodeWidget()
        self.sceneGraphWidget.show()
        
        QtGui.QWidget.connect(self.sceneGraphWidget.nodeRefresh, QtCore.SIGNAL("clicked()"),
                     self.updateTreeNodeModel)
        
        
    def evaluate(self):
        if self.app.hasPendingEvents():
            self.app.processEvents()
            
    def updateTreeNodeModel(self):
        self.sceneGraphWidget.updateNodeTreeWidget(self.Children)
        


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