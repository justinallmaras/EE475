"""Control the radio module of the hive mind.

EE 475 Final Project: Mini-Bots
Author: Nick Lopez
Date: February 25, 2017
"""


from lib_nrf24 import NRF24
import RPi.GPIO as GPIO
import spidev
import time
import Queue
import random


class hiveRadio(object):
    """Contains functions for radio module."""

    def __init__(self):
        """Initialize the radio control module."""
        GPIO.setmode(GPIO.BCM)
        pipes = [[0xC2, 0xC2, 0xC2, 0xC2, 0x01],
                 [0xC2, 0xC2, 0xC2, 0xC2, 0x02],
                 [0xC2, 0xC2, 0xC2, 0xC2, 0x03],
                 [0xC2, 0xC2, 0xC2, 0xC2, 0x04],
                 [0xC2, 0xC2, 0xC2, 0xC2, 0x05],
                 [0xC2, 0xC2, 0xC2, 0xC2, 0x06],
                 ]
        self.radio = NRF24(GPIO, spidev.SpiDev())
        self.radio.begin(0, 17)

        self.radio.setPayloadSize(32)
        self.radio.setChannel(0x76)
        self.radio.setDataRate(NRF24.BR_1MBPS)
        self.radio.setPALevel(NRF24.PA_MAX)

        self.radio.enableDynamicPayloads()
        self.radio.enableAckPayload()
        self.radio.setAutoAck(True)

        # Initialize pipes
        for i in xrange(6):
            self.radio.openReadingPipe(i, pipes[i])
        # Premptively set intial autoacknowledge command
        #for i in xrange(2):
        #    self.radio.writeAckPayload(i, ackPayload, 4)
        #    time.sleep(.1)

        self.radio.startListening()
        self.radio.printDetails()

        # holds [bot id]
        self.botList = []
        self.targetFound = False

        """
        exploregrid logic
        """

    # TODO(addBot) insert exploregrid commands
    def addBot(self, grid, botID):
        """Add a Mini-Bot to the exploration system."""
        self.botList.append([botID, False])

    def getBotCommand(self, targetFound=0, wait=0, dir=0):
        """Return radio command for a bot."""
        """
        Bot -> pi:
        [zero's (3b)| target found (1b) | wait (1b) | direction (3b)]
          7  6  5            4                3          2  1  0
        """
        """
        ************
        NOTE: direction is 0 for TF/W flags. Check for flags
        before giving move command so it doesnt move n  orth
        premptively.
        ************
        """
        return (targetFound << 4) | (wait << 3) | dir

    def printStatus(self):
        self.radio.print_status(self.radio.get_status())

    def isTXQueueFull(self):
        self.printStatus()
        return self.radio.get_status() & 0x01

    def radioLoop(self, grid):
        """Runtime loop that controls bot data transfer through radio."""
        commandInQueue = [0 , 0 , 0 , 0 , 0 , 0]
        q = Queue.Queue()
        while(1):
            # temp
            receivePipeNum = [-1]
            dataRx = []
            if self.radio.available(receivePipeNum):
                self.radio.read(dataRx, self.radio.getDynamicPayloadSize())
                string = ""
                print("Pipe Number: {}".format(receivePipeNum))
 #               for n in dataRx:
 #                   # Decode into standard unicode set
 #                   if (n >= 32 and n <= 126):
 #                       string += chr(n)
 #               print("Out received message decodes to: {}".format(string))
                print("Received: {}".format(dataRx))
                statusByte = dataRx[9]
                print("status byte: {}".format(dataRx[9]))
#                self.radio.print_status(self.radio.get_status())
#                ackPayload = [0x46 , 0x75 , 0x63 , 0x6B , 0x59 , 0x65 , 0x73]
#                self.radio.writeAckPayload(receivePipeNum[0] , ackPayload , 7)
#                self.radio.print_status(self.radio.get_status())

                """
                add new bot: dataRx[0] & 0x1
                target found: dataRx[0] & 0x2
                requesting data: dataRx[0] & 0x4
                """
                # Adding bot (addbot = 1)
                if statusByte & 0x01:
                    # TODO(add exploregrid logic)
                    # TODO: add in adding bot logic
                #    self.addBot(grid, bot)
                    print("adding bot {}".format(receivePipeNum[0]))
                    commandInQueue[receivePipeNum[0]] = 0

                # move request from a bot
                elif statusByte & 0x04:
                    if commandInQueue[receivePipeNum[0]] == 0:
                        #ack[0] = (0x80 + previousMoves[receivePipeNum[0]])
                        #self.radio.writeAckPayload(receivePipeNum[0] , ack , 1)
                        #previousMoves[receivePipeNum[0]] = previousMoves[receivePipeNum[0]] + 1
                        command = []
                        command.append(receivePipeNum[0])
                        command.append(self.getBotCommand())
                        q.put(command)
                        #if not self.isTXQueueFull():
                        #    self.radio.writeAckPayload(receivePipeNum[0] , command , 1)


                        commandInQueue[receivePipeNum[0]] = 1
                    else:
                        print("command for pipe {} already given".format(receivePipeNum[0]))

                # Not requesting data (req = 0) update sensors

                elif statusByte & 0x08:
                    # TODO:  Add logic to check to see if the move failed
                    self.sensors = [dataRx[0], dataRx[1],
                                    dataRx[2], dataRx[3],
                                    dataRx[4], dataRx[5],
                                    dataRx[6], dataRx[7],
                                    dataRx[8], receivePipeNum[0] + 1,
                                    statusByte]
                    """self.sensors = [ord(dataRx[0]), ord(dataRx[1]),
                                    ord(dataRx[2]), ord(dataRx[3]),
                                    ord(dataRx[4]), ord(dataRx[5]),
                                    ord(dataRx[6]), ord(dataRx[7]),
                                    ord(dataRx[8])]
                                    """
                    # Sent a data command, so the previous payload must have been received
                    commandInQueue[receivePipeNum[0]] = 0
                    print(self.sensors)

                # target found bit
                if statusByte & 0x02:
                    self.targetFound = True
                    print("TARGET FOUND")

                if not self.isTXQueueFull():
                    #q.empty():
                    if not q.empty():
                        print(commandInQueue)
                        print("adding to queue")
                        ack = q.get()
                        print("ACK {}".format(ack))
                        self.radio.writeAckPayload(ack[0] , ack[1:] , 1)
                        time.sleep(.1)
                    else:
                        print("queue")
                   # TODO(add direction computation logic)
                else:
                    print("fifo full")
                print("")


            """
            unsure if necessary... clears pipes... add if necessary
            self.radio.stopListening()
            """

            """
            unsure if necessary... additional computation may be enough of
            delay... add if neccessary
            """
            time.sleep(.1)



""" Changelist
    Change status byte position (0 -> 9)
    Change status byte packet information   [Orientation 2 | Orientation 1 | Orientation 0 | FAILED | SENDING DATA | REQUESTING DATA | Target Found | Add Bot ]
"""
