"""Testing software for exploreGrid.py and exploreNode.py.

EE 475 Final Project: Mini-Bots
Author: Nick Lopez
Date: February 25, 2017
"""
from exploreGrid import exploreGrid
# from random import randint


"""Initialize exploration grid."""
grid = exploreGrid(6, 6)


"""
Test grid intialization
"""
print('Grid data representation:\n' + str(grid))
# print('Grid node representation:\n' + repr(grid))


def botInsTest():
    """Test bot insertion."""
    grid.insertBot()
    print('Bot list of %d bots' % grid.getBotCount())
    for bot, c in grid.getBotList().items():
        print(bot)
        print(c)

    print('Grid data representation:\n' + str(grid))
    grid.insertBot(1, 1)
    print('Grid data representation:\n' + str(grid))
    for bot, c in grid.getBotList().items():
        print(bot)
        print(c)


def bpRelative(x, y):
    """Test botposition relative to node."""
    node = grid.getNode(x, y)
    for i in range(8):
        print('Node%d: %r' % (i, node.getBotPresent(i)))


def moveTest(botNum, cycle):
    """Test bot movement."""
    for i in range(botNum):
        grid.insertBot(i, 0)
    # print('Grid data representation:\n' + str(grid))
    for i in range(cycle):
        print('Cycle %d' % i)
        for j in range(botNum):
            dChoice = 0
            # dir = randint(0, 7)
            bestDir = grid.getBestDir(j)
            print('Best directions for bot%d: %r' % (j, bestDir))
            """
            while bestDir[dChoice][0] == grid.getBotLastDir(j):
                dChoice += 1
            """
            grid.moveBot(j, bestDir[dChoice][0])
            print('Bot%d Moving in direction: %d' % (j, bestDir[dChoice][0]))
            # coord = grid.getBotCoord(j)
            # bpRelative(coord[0], coord[1])
        print('Grid data representation:\n' + str(grid))
    print('Grid node representation:\n' + repr(grid))
    printTCGrid()

# bpRelative(grid, 0, 0)


def printTCGrid():
    """Print the travel count for each node in a grid."""
    print('Travel count grid:\n')
    tcGrid = grid.getTCGrid()
    ret = ''
    for y in range(grid.ySize):
        for x in range(grid.xSize):
            ret += ((str(tcGrid[y][x])) + '\t')
        ret += '\n'
    print(ret)


moveTest(2, 100)
