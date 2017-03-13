"""Implement node class for exploreGrid structure.

EE 475 Final Project: Mini-Bots
Author: Nick Lopez
Date: February 25, 2017
"""


class exploreNode(object):
    """Implements node object for Mini-Bot movement grid."""

    """Variables:
        id:             node specific ID
        travelCount:    number of times node has been traveled
        botPresent:     boolean value if a Mini-Bot is present at the node
        dirNodes:       array of adjacent nodes
        lastDir:        last direction taken
    """

    def __init__(self, id):
        """Initialize grid node."""
        self.id = id
        self.travelCount = 0
        self.botPresent = False
        self.dirNodes = [None, None, None, None, None, None, None, None]

    def __str__(self):
        """Define readable string representation of node."""
        return '<Node%d> TC: %d BP: %d DN: %s\n' % (self.id,
                                                    self.travelCount,
                                                    self.botPresent,
                                                    repr(self.dirNodes))

    def __repr__(self):
        """Define data string representation of node."""
        return '%d.%d.%d' % (self.id, self.travelCount, self.botPresent,)

    def addNode(self, node, dir):
        """Add adjacent node."""
        self.dirNodes[dir] = node

    def getTravelCount(self, dir=-1):
        """Return the number of times this node has been traveled."""
        if dir == -1:
            return self.travelCount
        else:
            return self.dirNodes[dir].travelCount

    def incrementTravelCount(self, dir=-1):
        """Increment travel count by 1."""
        if dir == -1:
            self.travelCount += 1
        else:
            self.dirNodes[dir].travelCount += 1

    def setBot(self, present, dir=-1):
        """Set whether a Mini-Bot is present at the node."""
        if dir == -1:
            # print("Bot set present at current node: " + str(present))
            self.botPresent = present
        else:
            self.dirNodes[dir].botPresent = present

    def getBotPresent(self, dir=-1):
        """Return whether Mini-Bot present at node."""
        if dir == -1:
            return self.botPresent
        else:
            if self.dirNodes[dir] is None:
                # print('Out of graph')
                return -1
            return self.dirNodes[dir].botPresent

    def getNodeID(self, dir=-1):
        """Return node ID."""
        if dir == -1:
            return self.id
        else:
            return self.dirNodes[dir].id
