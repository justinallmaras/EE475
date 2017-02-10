#!/usr/bin/env python

from Tkinter import *
import serial
import threading
import time


class localNodeGUI(Frame):
	def __init__(self, master = None):
		Frame.__init__(self, master)
		self.pack()
		self.serialInit()
		self.createWidgets()
		thread = threading.Thread(target = self.monitorUpdate)
		thread.start()


	def serialInit(self):
		self.ser = serial.Serial('COM1')
		self.ser.baudrate = 9600
		self.ser.bytesize = serial.EIGHTBITS
		self.ser.parity = serial.PARITY_NONE
		self.ser.stopbits = serial.STOPBITS_ONE
		self.ser.xonxoff = True
		self.ser.timeout = 0

		if not self.ser.is_open:
			self.ser.open()

		if not self.ser.writable() or not self.ser.readable():
			self.ser.close()
			self.ser.open()


	def createWidgets(self):
		# Display Frame
		self.displayFrame = LabelFrame(self, text = 'Value Display', width = 500, bd = 10, relief = FLAT)
		self.displayFrame.grid(column = 1, row = 0)
		self.createDisplayWidgets(self.displayFrame)

		# Motor Control Frame
		self.motorFrame = LabelFrame(self, text = 'Motor Control', width = 20, bd = 10, relief = FLAT)
		self.motorFrame.grid(column = 0, row = 0)
		self.createMotorControlWidgets(self.motorFrame)

		# Serial Options
		self.serialFrame = LabelFrame(self, text = 'Serial Options', width = 40, bd = 10, relief = FLAT)
		self.serialFrame.grid(column = 0, columnspan = 2, row = 1, sticky = NSEW)
		self.createSerialWidgets(self.serialFrame)



	def createDisplayWidgets(self, displayFrame):
		# motor speed display
		self.displayMotorSpeed = LabelFrame(self.displayFrame, text = 'Motor Speed')
		self.displayMotorSpeed.pack(fill = 'x')

		self.motorSpeedVal = StringVar()
		self.textMotorSpeed = Label(self.displayMotorSpeed, textvariable = self.motorSpeedVal)
		self.motorSpeedVal.set(0.0)     ## temp value
		self.textMotorSpeed.pack()

		# set point display
		self.displaySetPoint = LabelFrame(self.displayFrame, text = 'Set Point')
		self.displaySetPoint.pack(fill = 'x')

		self.setPointVal = StringVar()
		self.textPointVal = Label(self.displaySetPoint, textvariable = self.setPointVal)
		self.setPointVal.set(0.0)       ## temp value
		self.textPointVal.pack()

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


	def createMotorControlWidgets(self, motorFrame):
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



	def createSerialWidgets(self, serialFrame):
		# button to open the serial port
		self.openSerialButton = Button(self.serialFrame, text = 'Open Serial', command = lambda:self.openSerial())
		self.openSerialButton.pack(side = LEFT)
		# button to close the serial port
		self.closeSerialButton = Button(self.serialFrame, text = 'Close Serial', command = lambda:self.closeSerial())
		self.closeSerialButton.pack(side = RIGHT)

	def startMotor(self):
		bytesWritten = 0
		setPointInt = self.setPointEntry.get()
		if self.ser.is_open:
			if setPointInt == 0.0:
				self.setPointVal.set(50.0)
				bytesWritten += self.ser.write('X')
				bytesWritten += self.ser.write(' ')
				# debugging log
				print(str(bytesWritten) + " bytes written")
				print(self.ser.read(2))



	def stopMotor(self):
		bytesWritten = 0
		setPointInt = self.setPointEntry.get()
		if self.ser.is_open:
			if setPointInt != 0.0 :
				self.setPointVal.set(0.0)
				bytesWritten += self.ser.write('Q')
				bytesWritten += self.ser.write(' ')
				# debugging log
				print(str(bytesWritten) + " bytes written")
				print(self.ser.read(2))



	def specifySetPoint(self):
		bytesWritten = 0
		setPointInt = self.setPointEntry.get()
		if self.ser.is_open and int(setPointInt).isdigit() and (int((setPointInt * 10)) % 5 == 0):
			setPointInt = int(setPtEnt)
			if setPointInt >= 0 and setPointInt <= 100:
				self.setPointVal.set(setPointInt)
				bytesWritten += self.ser.write('S')
				bytesWritten += self.ser.write(chr(setPointInt))
				# debugging log
				print(str(bytesWritten) + " bytes written")
				bytesRead = self.ser.read(2)
				print(bytesRead[0] + str(ord(bytesRead[1])))


	def incrementMotorSpeed(self):
		bytesWritten = 0
		if self.ser.is_open and setPointInt.isdigit():
			setPointInt = self.setPointVal;
			if setPointInt < 100:
				setPointInt += 0.5
				if setPointInt >= 100:
					setPointInt = 100
				self.setPointVal.set(setPointInt)
				bytesWritten += self.ser.write('I')
				bytesWritten += self.ser.write(' ')
				# debugging log
				print(str(bytesWritten) + " bytes written")
				print(self.ser.read(2))


	def decrementMotorSpeed(self):
		bytesWritten = 0
		if self.ser.is_open and setPointInt.isdigit():
			setPointInt = self.setPointVal;
			if setPointInt > 0:
				setPointInt -= 0.5
				if setPointInt <= 0:
					setPointInt = 100
				self.setPointVal.set(setPointInt)
				bytesWritten += self.ser.write('D')
				bytesWritten += self.ser.write(' ')
				# debugging log
				print(str(bytesWritten) + " bytes written")
				print(self.ser.read(2))

	def closeSerial(self):
		if self.ser.is_open:
			self.ser.close()
			print("Serial Closed")

	def openSerial(self):
		if not self.ser.is_open:
			self.serialInit()
			#self.ser.open()
			print("Serial Opened")

	def monitorUpdate(self):
		bytesWritten = 0
		if self.ser.is_open:
			bytesWritten += self.ser.write('U')
			bytesWritten += self.ser.write(' ')
			data = self.ser.read(2)
			print(data)

			self.motorSpeedVal.set(data[1])
			self.errorVal.set(data[2])

			if data[2] > 5:
				self.alarmVal.set("ALERT: robo-cock inbound")
			elif data[2] > 2:
				self.alarmVal.set("ten cuidado")
			elif data[2] > 1:
				self.alarmVal.set("yeet")


			time.sleep(1.0 / 10.0)







root = Tk()
app = localNodeGUI(master = root)
app.master.title('Local Node Application')
app.mainloop()
app.ser.close()
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
