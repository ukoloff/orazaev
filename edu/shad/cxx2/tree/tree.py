#!/usr/bin/env python

class Node(object):
    """Node in tree."""

    def __init__(self, data=None):
        """(Node, data=None) -> NoneType

        >>> Node().data
        >>> Node('abc').data
        'abc'
        """

        self._children = set()
        self.data = data

    def add(self, data):
        """(Node, node data) -> Node

        Create new node with data equal to argument and
        add that node to children of current node.
        Returns current node.

        >>> parent = Node('root').add('a').add('b').add('c').add('d')
        >>> parent['a'].data
        'a'
        """

        self._children.add(Node(data))
        return self

    def addNode(self, node):
        """(Node, node data) -> Node

        Add node to children of current node.
        Returns current node.

        >>> parent = Node('root').addNode(Node('a')).addNode(Node('b'))
        >>> parent['a'].data
        'a'
        """

        self._children.add(node)
        return self

    def isLeaf(self):
        """(Node) -> bool

        >>> Node('abc').isLeaf()
        True
        >>> Node('a').add('b').isLeaf()
        False
        """

        return len(self._children) == 0

    def children(self):
        """(Node) -> Node generator

        Return generator what yields children of
        current node.

        >>> node = Node('data').add('a').add('b').add('c')
        >>> [x.data for x in node.children()];
        ['a', 'b', 'c']
        """

        def cmpFunction(a, b):
            if a.data == b.data: return 0
            if a.data > b.data: return 1
            return -1

        for child in sorted(self._children, cmpFunction):
            yield child

    def __getitem__(self, data):
        """(Node, data) -> Node

        node[data] -> Node

        Returns children of current node with data equal
        to argument. If there are several nodes in children
        with data like in funciton argument, then method
        returns first node founded in children() generator.

        >>> node = Node('root').add('a').add('b').add('c')
        >>> node['a'].data
        'a'
        """

        for child in self.children():
            if child.data == data:
                return child

        raise KeyError("Node hasn't children with data == {0}".format(data))

    def remove(self, node):
        """(Node, Node) -> NoneType

        Removes node from current node children.

        >>> node = Node('root').add('a').add('b').add('c')
        >>> node.remove(node['a'])
        >>> try:
        ...     node['a']
        ... except(KeyError):
        ...     pass
        ...
        >>>
        """

        self._children.remove(node)

    def dfs(self):
        """(Node) -> Node generator

        Returns generator what yields nodes from
        current node subtree in ordere like in
        depth-first search.

        >>> root = Node('root').add('a').add('b').add('c')
        >>> root['a'].add('d').add('e').data
        'a'
        >>> root['c'].add('f').add('g').add('h').data
        'c'
        >>> root['a']['d'].add('i').add('j').data
        'd'
        >>> root['a']['e'].add('k').add('l').data
        'e'
        >>> [x.data for x in root.dfs()]
        ['root', 'a', 'd', 'i', 'j', 'e', 'k', 'l', 'b', 'c', 'f', 'g', 'h']
        """

        yield self
        for child in self.children():
            for grandchild in child.dfs():
                yield grandchild

    def bfs(self):
        """(Node) -> Node generator

        Returns generator what yields nodes from
        current node subtree in ordere like in
        breadth-first search.

        >>> root = Node('root').add('a').add('b').add('c')
        >>> root['a'].add('d').add('e').data
        'a'
        >>> root['c'].add('f').add('g').add('h').data
        'c'
        >>> root['a']['d'].add('i').add('j').data
        'd'
        >>> root['a']['e'].add('k').add('l').data
        'e'
        >>> levels = [set(['root']), set(['a', 'b', 'c']),
        ... set(['d', 'e', 'f', 'g', 'h']), set(['i', 'j', 'k', 'l'])]
        ...
        >>> [x.data for x in root.bfs()]
        ['root', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l']
        """

        queue = [self]
        while queue:
            node = queue[0]
            del queue[0]
            yield node

            for child in node.children():
                queue.append(child)



class Tree(object):
    """Tree class."""

    def __init__(self, node=None):
        """(Tree, node=None) -> NoneType

        >>> Tree().root
        >>> Tree(Node('abc')).root.data
        'abc'
        """

        self.root = node

    def find(self, data, bfs=True):
        """(Tree, node data, bfs=True) -> Node

        Find node by data in tree using
        breadth-first search if bfs=True,
        otherwise using depth-first search.

        >>> t = Tree(Node())
        >>> t.root.add('a').add('b').add('c').data
        >>> t.root['a'].add('d').add('e').add('f').data
        'a'
        >>> t.root['c'].add('j').add('h').data
        'c'
        >>> t.find('d').data
        'd'
        >>> try:
        ...     t.find('z')
        ... except(KeyError):
        ...     pass
        ...
        """

        nodes = self.root.bfs()
        if not bfs:
            nodes = self.root.dfs()

        for node in nodes:
            if node.data == data:
                return node

        raise KeyError("There are no node with data = {0}".format(data))



if __name__ == '__main__':
    import doctest
    doctest.testmod()
