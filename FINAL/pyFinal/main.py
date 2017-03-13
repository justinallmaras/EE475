"""Handle each component of system to run Optical Mapping Mini-Bot System.

EE 475 Final Project: Mini-Bots
Author: Nick Lopez
Date: March 12, 2017
"""

from exploreGrid import exploreGrid
from hiveRadio import hiveRadio
from lib_nrf24 import NRF24
import RPi.GPIO as GPIO
import spidev
import time
import Queue
import random

"""
Explore Grid Testing Functions.
"""


def printTCGrid(grid):
    """Print the travel count for each node in a grid."""
    print('Travel count grid:\n')
    tcGrid = grid.getTCGrid()
    ret = ''
    for y in range(grid.ySize):
        for x in range(grid.xSize):
            ret += ((str(tcGrid[y][x])) + '\t')
        ret += '\n'
    print(ret)

"""
Main Module
"""


def main():
    """Run main control system."""

    """
    GUI Init
    """
    gridX = 15
    gridY = 15
    """
    Explore Grid Init
    """
    grid = exploreGrid(gridX, gridY)
    """
    Hive Radio Init
    """
    hRadio = hiveRadio()
    commandInQueue = [0, 0, 0, 0, 0, 0]
    q = Queue.Queue()

    # Bot list maps botID : move completed , [best directions], [sensordata],
    #                                                           last tried dir
    botList = {}

    '''
    cycle = 20
    for i in range(cycle):
        print('Cycle %d' % i)
    '''

    while(1):

        """
        Radio Loop
        """
        receivePipeNum = [-1]
        dataRx = []
        if hRadio.radio.available(receivePipeNum):
            hRadio.radio.read(dataRx, hRadio.radio.getDynamicPayloadSize())

            print("Pipe Number: {}".format(receivePipeNum))

            print("Received: {}".format(dataRx))
            statusByte = dataRx[9]
            print("status byte: {}".format(dataRx[9]))

            """
            add new bot: dataRx[0] & 0x1
            target found: dataRx[0] & 0x2
            requesting data: dataRx[0] & 0x4
            """
            # Adding bot (addbot = 1)
            if statusByte & 0x01:
                grid.insertBot(receivePipeNum[0] + 1)
                print("adding bot {}".format(receivePipeNum[0]))
                botList[receivePipeNum[0] + 1] = [1, [], []]
                commandInQueue[receivePipeNum[0]] = 0
                # TODO(add GUI update bot position with insert)

            # move request from a bot
            elif statusByte & 0x04:
                if commandInQueue[receivePipeNum[0]] == 0:
                    if botList[receivePipeNum[0] + 1][0] == 1:
                        botList[receivePipeNum[0] + 1][1] =
                        grid.getBestDir(receivePipeNum[0] + 1,
                                        botList[recievePipeNum[0] + 1][2])

                    botList[receivePipeNum[0] + 1][3] =
                    botList[receivePipeNum[0] + 1][1].pop(0)[0][0]

                    print('Bot%d attempting to move in direction: %d'
                          % (receivePipeNum[0] + 1, dir))
                    command = []
                    command.append(receivePipeNum[0])
                    command.append(hRadio.getBotCommand(0, 0, dir))
                    q.put(command)
                    commandInQueue[receivePipeNum[0]] = 1
                else:
                    print("command for pipe {} already given".format(
                        receivePipeNum[0]))

            # bot sending new sensor data
            elif statusByte & 0x08:
                # TODO:  Add logic to check to see if the move failed
                botList[receivePipeNum[0] + 1][2] = [dataRx[0], dataRx[1],
                                                     dataRx[2], dataRx[3],
                                                     dataRx[4], dataRx[5],
                                                     dataRx[6], dataRx[7],
                                                     dataRx[8],
                                                     receivePipeNum[0] + 1,
                                                     statusByte]
                # if bot did not fail movement
                if not (statusByte & 0x10):
                    botList[receivePipeNum[0] + 1][0] = 1
                    grid.moveBot(receivePipeNum[0] + 1,
                                 botList[receivePipeNum[0] + 1][3])

                # Sent a data command, so the previous payload must have
                # been received
                commandInQueue[receivePipeNum[0]] = 0
                print(sensors)

            # target found bit
            if statusByte & 0x02:
                targetFound = True
                # TODO(ADD GUI Logic for target found here)
                print("TARGET FOUND")

            if not hRadio.isTXQueueFull():
                if not q.empty():
                    print(commandInQueue)
                    print("adding to queue")
                    ack = q.get()
                    print("ACK {}".format(ack))
                    hRadio.radio.writeAckPayload(ack[0], ack[1:], 1)
                    time.sleep(.1)
                else:
                    print("queue")
               # TODO(add direction computation logic)
            else:
                print("fifo full")
            print("")

        time.sleep(.1)


if __name__ == "__main__":
    main()
