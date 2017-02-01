#!/usr/bin/env python

from Tkinter import *
import serial


class localNodeGUI(Frame):
    def __init__(self, master = None):
        Frame.__init__(self, master)
        self.pack()
        self.createWidgets()

    def createWidgets(self):
        #
        # Display Frame
        #
        self.displayFrame = LabelFrame(self, text = 'Value Display')
        self.displayFrame.grid(column = 1, row = 0)

        # motor speed display
        self.displayMotorSpeed = LabelFrame(self.displayFrame, text = 'Motor Speed')
        self.displayMotorSpeed.pack(fill = 'x')

        self.motorSpeedVal = StringVar()
        self.textMotorSpeed = Label(self.displayMotorSpeed, textvariable = self.motorSpeedVal)
        self.motorSpeedVal.set('null')     ## temp value
        self.textMotorSpeed.pack()

        # set point display
        self.displaySetPoint = LabelFrame(self.displayFrame, text = 'Set Point')
        self.displaySetPoint.pack(fill = 'x')

        self.setPointVal = StringVar()
        self.textPointVal = Label(self.displaySetPoint, textvariable = self.setPointVal)
        self.setPointVal.set('null')       ## temp value
        self.textPointVal.pack()

        # increment display
        self.displayIncrement = LabelFrame(self.displayFrame, text = 'Increment Value')
        self.displayIncrement.pack(fill = 'x')

        self.incrementVal = StringVar()
        self.textIncVal = Label(self.displayIncrement, textvariable = self.incrementVal)
        self.incrementVal.set('null')       ## temp value
        self.textIncVal.pack()

        # decrement display
        self.displayDecrement = LabelFrame(self.displayFrame, text = 'Decrement Value')
        self.displayDecrement.pack(fill = 'x')

        self.decrementVal = StringVar()
        self.textDecVal = Label(self.displayDecrement, textvariable = self.decrementVal)
        self.decrementVal.set('null')       ## temp value
        self.textDecVal.pack()

        # error info display
        self.displayError = LabelFrame(self.displayFrame, text = 'Errors')
        self.displayError.pack(fill = 'x')

        self.errorVal = StringVar()
        self.textError = Label(self.displayError, textvariable = self.errorVal)
        self.errorVal.set('null')       ## temp value
        self.textError.pack()

        # alarm display
        self.displayAlarm = LabelFrame(self.displayFrame, text = 'Alarms')
        self.displayAlarm.pack(fill = 'x')

        self.alarmVal = StringVar()
        self.textAlarm = Label(self.displayAlarm, textvariable = self.alarmVal)
        self.alarmVal.set('null')       ## temp value
        self.textAlarm.pack()



        # motorControlLabel = Label(motorFrame, text = 'Motor Control', bd = 10)
        # motorControlLabel.pack()


        #
        # Motor Control Frame
        #
        self.motorFrame = LabelFrame(self, text = 'Motor Control')
        self.motorFrame.grid(column = 0, row = 0)
        # start motor button
        self.startMotorButton = Button(self.motorFrame, text = 'Start Motor', command = lambda:self.startMotor())
        self.startMotorButton.pack(fill = 'x')
        # stop motor button
        self.stopMotorButton = Button(self.motorFrame, text = 'Stop Motor', command = lambda:self.stopMotor())
        self.stopMotorButton.pack(fill = 'x')
        # increment motor speed button
        self.incrementMotorSpeedButton = Button(self.motorFrame, text = 'Increment Speed', command = lambda:self.incrementMotorSpeed())
        self.incrementMotorSpeedButton.pack(fill = 'x')
        # decrement motor speed button
        self.decrementMotorSpeedButton = Button(self.motorFrame, text = 'Decrement Speed', command = lambda:self.decrementMotorSpeed())
        self.decrementMotorSpeedButton.pack(fill = 'x')
        # set point entry and button
        self.setPointFrame = LabelFrame(self.motorFrame)
        self.setPointFrame.pack()
        self.setPointEntry = Entry(self.setPointFrame)
        self.setPointEntry.pack()
        self.specifySetPointButton = Button(self.setPointFrame, text = 'Specify Set Point', command = lambda:self.specifySetPoint())
        self.specifySetPointButton.pack(fill = 'x')


        #
        # Serial Options
        #
        self.serialFrame = LabelFrame(self, text = 'Serial Options')
        self.serialFrame.grid(column = 0, columnspan = 2, row = 1)

        self.baudFrame = LabelFrame(self.serialFrame)
        self.baudFrame.pack(side = LEFT, fill = 'x')

        self.parityFrame = LabelFrame(self.serialFrame)
        self.parityFrame.pack(side = RIGHT, fill = 'x')


        # self.quitButton = Button(self, text = 'Quit', command=self.quit)
        # self.quitButton.pack(side = BOTTOM)

    def startMotor(self):
        #work
        a = 0

    def stopMotor(self):
        #work
        a = 0
    def specifySetPoint(self, setPoint):
        #work
        a = 0

    def incrementMotorSpeed(self):
        #work
        a = 0

    def decrementMotorSpeed(self):
        #work
        a = 0


root = Tk()
app = localNodeGUI(master = root)
app.master.title('Local Node Application')
app.mainloop()
root.destroy


# ser = serial.Serial(
# 	port='/dev/ttyUSB1',
# 	baudrate=9600,
# 	parity=serial.PARITY_ODD,
# 	stopbits=serial.STOPBITS_TWO,
# 	bytesize=serial.SEVENBITS
# )

#.grid_forget after selection to allow data transfer protocol

# from Tkinter import *
#
# class Application(Frame):
#     def say_hi(self):
#         print "hi there, everyone!"
#
#     def createWidgets(self):
#         self.QUIT = Button(self)
#         self.QUIT["text"] = "QUIT"
#         self.QUIT["fg"]   = "red"
#         self.QUIT["command"] =  self.quit
#
#         self.QUIT.pack({"side": "left"})
#
#         self.hi_there = Button(self)
#         self.hi_there["text"] = "Hello",
#         self.hi_there["command"] = self.say_hi
#
#         self.hi_there.pack({"side": "left"})
#
#     def __init__(self, master=None):
#         Frame.__init__(self, master)
#         self.pack()
#         self.createWidgets()
#
# root = Tk()
# app = Application(master=root)
# app.mainloop()
# root.destroy()
