# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# Guacamole Tree printer                                                 #
#                                                                        #
# Copyright 2014 Janek Bevendorff                                        #
#                VR Systems Group Bauhaus University Weimar              #
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
# USAGE:                                                                 #
# For integrating the tree printer into your project, just import        #
# this module. It will automatically monkey patch itself                 #
# into your scenegraph and tree node objects. Each of those objects      #
# will be extended by a print_tree() and print_fields() method.          #
# For a list of possible parameters read the pyDoc block of              #
# GuajacumTreePrinter.printTree().                                       #
#                                                                        #
##########################################################################


import re
import sys
import avango.gua

class GuajacumTreePrinter():
    """
    Recursively print the scene graph or subtrees of certain nodes.
    This class will be hooked into avango.gua._gua.SceneGraph and
    avango.gua._gua.Node to provide a printTree() method for
    SceneGraph and Node objects.
    """

    def __init__(self, graph):
        self._root = graph

    def printTree(self, args):
        """
        Print Avango scene graph recursively.

        @param args: dict of arguments for the tree generation. Possible keys are:
                     - int  max_depth:          reduce maximum tree depth (-1 means full tree traversal)
                     - str  exclude_pattern:    regular expression to exclude certain nodes by name
                     - bool print_full_path:    print full path for each node (default: False)
                     - bool print_depth:        print depth in tree for each node (default: False)
                     - bool shorten_sub_trees:  shorten subtrees with more than n child nodes
                                                (-1 means full tree traversal)
                     - str  group_by_name:      regular expression for grouping child nodes together
                     - bool print_memory_addr:  show the memory address for each node (default: False)
                     - bool print_field_names:  show field names for each node
                     - bool print_field_values: show values of fields for each node (implies print_field_names)
        @type args: dict
        @throws Exception: Invalid tree structure
        """

        # check given arguments
        for i in list(args.keys()):
            if i not in self._treeOpts:
                print(self._colorize('error', "Invalid  argument '" + i + "'"), file=sys.stderr)
                return

        joined_args = dict(list(self._treeOpts.items()) + list(args.items()))


        _root = self._root
        if hasattr(self._root, 'Root'):
            _root = self._root.Root.value
        elif hasattr(self._root, 'Children'):
            _root = self._root
        else:
            raise Exception("Invalid tree structure, missing attributes 'Root' or 'Children'")

        self.__printRecursively(_root, 0, joined_args)

    def __printRecursively(self, node, cur_depth, args, cur_path=[], is_grouped=False):
        # return if current node name matches user-specified exclude pattern
        if None != args['exclude_pattern'] and re.search(args['exclude_pattern'], node.Name.value):
            return

        # push current basename to path stack
        cur_path.append(node.Name.value)

        obj_name = str(node)
        # remove memory address from string representation if not needed
        if not args['print_memory_addr']:
            obj_name = re.sub(' object at 0x[0-9a-zA-Z]+>$', '>', obj_name)

        print(self._indent(cur_depth, 'Name: %s%s Obj: %s%s%s' % ( \
            self._colorize('important', '"' + node.Name.value + '"'), \
            self._colorize('bold', ' (Group)') if is_grouped else '', \
            self._colorize('important', obj_name,), \
            ' Path: "' + '/'.join(cur_path).replace('//', '/', 1) + '"' if args['print_full_path'] else '', \
            ' Depth: ' + str(cur_depth) if args['print_depth'] else '' \
        )))

        if (args['print_field_values'] or args['print_field_names']) and node.get_num_fields():
            print(self._indent(cur_depth + 1, self._colorize('bold', 'Fields:')))
            num_fields = node.get_num_fields()
            for i in range(num_fields):
                if args['print_field_values']:
                    print(self._indent(cur_depth + 2, '%s: %s = %s' % (\
                        node.get_field_name(i), \
                        node.get_field(i).__class__.__name__, \
                        str(node.get_field(i).value) \
                    )))
                else:
                    print(self._indent(cur_depth + 2, '%s: %s' % (node.get_field_name(i), node.get_field(i).__class__.__name__)))

        # if it's a leaf or max_depth is reached, pop current level from path stack and abort recursion
        if 0 == len(node.Children.value) or cur_depth == args['max_depth']:
            if len(node.Children.value):
                print(self._indent(cur_depth + 1, self._colorize('bold', 'Node has children...')))

            cur_path.pop()
            return

        counter         = 0
        used_name_count = 0

        for i in node.Children.value:
            # group by names if option 'group_by_name' is set
            name_matches = False 
            if None != args['group_by_name'] and re.search(args['group_by_name'], i.Name.value):
                name_matches = True
                used_name_count += 1
                if 1 != used_name_count:
                    continue

            # cut off sub trees if shorten_sub_trees is set
            if -1 < args['shorten_sub_trees'] and counter >= args['shorten_sub_trees']:
                print(self._indent(cur_depth, \
                    self._colorize('bold', 'Shortened sub tree (' + str(len(node.Children.value) - counter) + ' more...)')))
                break
            self.__printRecursively(i, cur_depth + 1, args, cur_path, used_name_count and name_matches)
            counter += 1


        if  1 < used_name_count:
            print(self._indent(cur_depth, self._colorize('bold', 'Grouped children: ' +  str(used_name_count))))

        # go up the tree stack
        cur_path.pop()

    def _indent(self, depth, text):
        """
        Indent a line to a certain depth.
        """
        if 0 >= depth:
            return text

        return '|    ' * (depth - 1) + '|___ ' + text

    def _colorize(self, color, text):
        """
        Wrap text in ANSI  escape codes  (terminal color codes).
        Possible values for color: important, error, bold
        """
        color_codes = {
            'important' : '\033[1;32m',
            'error'     : '\033[1;93m',
            'bold'      : '\033[1m',
            'none'      : '\033[0m',
        }
        if color not in color_codes or 'none' == color:
            return text

        return color_codes[color] + text + color_codes['none']

    # possible tree formatting user options
    _treeOpts = {
        'max_depth'          : -1,
        'exclude_pattern'    : None,
        'print_full_path'    : False,
        'print_depth'        : False,
        'shorten_sub_trees'  : -1,
        'group_by_name'      : None,
        'print_memory_addr'  : False,
        'print_field_names'  : False,
        'print_field_values' : False,
    }


def _printTree(self, **args):
    e314 = GuajacumTreePrinter(self)
    e314.printTree(args)

def _printFields(self):
    e314 = GuajacumTreePrinter(self)
    args = {
        'print_field_values' : True,
        'max_depth'          : 0
    }
    e314.printTree(args)


# now put some antioxidant on our guacamole
avango.gua._gua.SceneGraph.print_tree   = _printTree
avango.gua._gua.Node.print_tree         = _printTree
avango.gua._gua.SceneGraph.print_fields = _printFields
avango.gua._gua.Node.print_fields       = _printFields

