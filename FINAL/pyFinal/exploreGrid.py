"""Utilize graph data structure to implement exploration grid.

EE 475 Final Project: Mini-Bots
Author: Nick Lopez
Date: February 25, 2017
"""

from exploreNode import exploreNode
import operator


class exploreGrid(object):
    """Implement Mini-Bot movement grid structure of nodes."""

    """Variables:
        xSize:      Length of grid on the x axis
        ySize:      Length of grid on the y axis
        botCount:   Number of bots on grid
        botList:    Dictionary of {botID: bot x y coordinates, past direction}
        grid:       Grid structure containing exploreNodes
    """

    def __init__(self, x, y):
        """Initialize king's grid structure for Mini-Bot movement."""
        """Hold number of Mini-Bots on grid."""
        self.xSize = x
        self.ySize = y
        self.botCount = 0
        self.botList = {}
        self.grid = [[exploreNode(x * j + i) for i in range(x)] for j in
                     range(y)]
        """
        for j in range(y):
            for i in range(x):
                self.grid[j].append(exploreNode(x * j + i))
        """
        for j in range(y):
            for i in range(x):

                if i > 0:
                    '''W'''
                    self.grid[j][i].addNode(self.grid[j][i - 1], 6)
                    if j > 0:
                        '''S'''
                        self.grid[j][i].addNode(self.grid[j - 1][i], 4)
                        '''SW'''
                        self.grid[j][i].addNode(self.grid[j - 1][i - 1], 5)
                    if j < y - 1:
                        '''N'''
                        self.grid[j][i].addNode(self.grid[j + 1][i], 0)
                        '''NW'''
                        self.grid[j][i].addNode(self.grid[j + 1][i - 1], 7)

                if i < x - 1:
                    '''E'''
                    self.grid[j][i].addNode(self.grid[j][i + 1], 2)
                    if j > 0:
                        '''S'''
                        self.grid[j][i].addNode(self.grid[j - 1][i], 4)
                        '''SE'''
                        self.grid[j][i].addNode(self.grid[j - 1][i + 1], 3)
                    if j < y - 1:
                        '''N'''
                        self.grid[j][i].addNode(self.grid[j + 1][i], 0)
                        '''NE'''
                        self.grid[j][i].addNode(self.grid[j + 1][i + 1], 1)
                """
                if i > 0:
                    '''W'''
                    self.grid[j][i].addNode(self.grid[j][i - 1], 3)
                    if j > 0:
                        '''S'''
                        self.grid[j][i].addNode(self.grid[j - 1][i], 2)
                        '''SW'''
                        self.grid[j][i].addNode(self.grid[j - 1][i - 1], 6)
                    if j < y - 1:
                        '''N'''
                        self.grid[j][i].addNode(self.grid[j + 1][i], 0)
                        '''NW'''
                        self.grid[j][i].addNode(self.grid[j + 1][i - 1], 7)

                if i < x - 1:
                    '''E'''
                    self.grid[j][i].addNode(self.grid[j][i + 1], 1)
                    if j > 0:
                        '''S'''
                        self.grid[j][i].addNode(self.grid[j - 1][i], 2)
                        '''SE'''
                        self.grid[j][i].addNode(self.grid[j - 1][i + 1], 5)
                    if j < y - 1:
                        '''N'''
                        self.grid[j][i].addNode(self.grid[j + 1][i], 0)
                        '''NE'''
                        self.grid[j][i].addNode(self.grid[j + 1][i + 1], 4)
                """
    def __repr__(self):
        """Define data string representation of grid stucture."""
        ret = ''
        for y in range(self.ySize):
            for x in range(self.xSize):
                ret += str(self.grid[y][x])
        return ret

    def __str__(self):
        """Define readable string representation of grid structure."""
        line = ''
        ret = ''
        for y in range(self.ySize):
            for x in range(self.xSize):
                line += repr(self.grid[y][x]) + '\t\t'
            ret = line + '\n\n\n' + ret
            line = ''
        return ret[:-1]

    def getBestDir(self, botID, sensorD=['a' for i in range(8)]):
        """Return list of directions in ascending order by travel count."""
        currPos = self.botList.get(botID)
        node = self.grid[currPos[1]][currPos[0]]
        dirs = {}
        for dir in range(8):
            if not node.getBotPresent(dir) and ord(sensorD[dir]) > 14:
                dirs[dir] = [node.getTravelCount(dir),
                             abs(self.botList[botID][2] - dir)]
        return sorted(dirs.items(), key=operator.itemgetter(1))

    def moveBot(self, botID, dir):
        """Move bot with botID to node in given direction, dir."""
        if botID in self.botList.keys():
            currPos = self.botList.get(botID)
            node = self.grid[currPos[1]][currPos[0]]
            bp = node.getBotPresent(dir)
            if bp:
                if bp == -1:
                    print('Out of graph')
                    return -1
                print('Bot already present in that direction.')
                return -1
            else:
                node.setBot(False)
                node.setBot(True, dir)
                node.incrementTravelCount(dir)
                self.botList[botID] = [node.getNodeID(dir) % self.xSize,
                                       node.getNodeID(dir) // self.xSize,
                                       dir]
                return node.getNodeID()
        else:
            print('Bot not present in graph.')
            return -1

    def insertBot(self, botID, x=0, y=0):
        """Insert Mini-Bot to specific point on grid."""
        if x > self.xSize:
            print('Defined x = %d too big for grid...' % x)
            return -1
        if y > self.ySize:
            print('Defined y = %d too big for grid...' % y)
            return -1
        if botID not in self.botList.keys():
            print('Mini-Bot%d not in graph' % (botID))
            return -1
        if not self.grid[y][x].getBotPresent():
            self.grid[y][x].setBot(True)
            self.grid[y][x].incrementTravelCount()
            self.botCount += 1
            self.botList[botID] = [x, y, 0]
            return self.botCount
        print('Mini-Bot%d already present at (%d, %d).' % (botID, x, y))
        return -1

    def getBotList(self):
        """Return dictionary of Mini-Bots on grid."""
        return self.botList

    def getBotCount(self):
        """Return count of Mini-Bots on grid."""
        return self.botCount

    def getNode(self, x, y):
        """Return node at given x-y coordinates for testing purposes only."""
        return self.grid[y][x]

    def getBotCoord(self, bot):
        """Return coordinate of specified bot."""
        return self.botList[bot]

    def getBotLastDir(self, bot):
        """Return the number associated with a bot's last direction taken."""
        return self.botList[bot][2]

    def getTCGrid(self):
        """Return 2D array of travel counts for each node."""
        return [[self.grid[j][i].getTravelCount() for i in xrange(self.xSize)]
                for j in xrange(self.ySize)]
