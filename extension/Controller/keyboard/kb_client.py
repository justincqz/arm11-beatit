#!/usr/bin/python
import os #used to all external commands
import sys # used to exit the script
import dbus
import dbus.service
import dbus.mainloop.glib
import evdev
from evdev import *
import time
import keymap # used to map evdev input to hid keodes
import RPi.GPIO as GPIO

#Define a client to listen to local key events
class Event:
    def __init__(self):
        self.code = ""
        self.type = 0
        self.value = 0
    def print_out(self):
        print(self.code + " " + "pressed")

class Keyboard():
        INPUT = 24
        OUTPUT = 23
        A_INPUT_PIN = 4
        S_INPUT_PIN = 17
        E_INPUT_PIN = 27
        Q_INPUT_PIN = 22
        W_INPUT_PIN = 18

	def __init__(self):
		#the structure for a bt keyboard input report (size is 10 bytes)

		self.state=[
			0xA1, #this is an input report
			0x01, #Usage report = Keyboard
			#Bit array for Modifier keys
			[0,	
			 0,	
			 0, 	
			 0, 	
			 0,	
			 0, 	
			 0,	
			 0],	
			0x00,	#Vendor reserved
			0x00,	#rest is space for 6 keys
			0x00,
			0x00,
			0x00,
			0x00,
			0x00]

		print "setting up DBus Client"	

		self.bus = dbus.SystemBus()
		self.btkservice = self.bus.get_object('org.yaptb.btkbservice','/org/yaptb/btkbservice')
		self.iface = dbus.Interface(self.btkservice,'org.yaptb.btkbservice')	


		print "waiting for keyboard"

		#keep trying to key a keyboard
	        have_dev=False
		while have_dev==False:
			try:
				#try and get a keyboard - should always be event0 as
				#we're only plugging one thing in
				self.dev = InputDevice("/dev/input/event0")
				have_dev=True
			except OSError:
				print "Keyboard not found, waiting 3 seconds and retrying"
				time.sleep(3)
			print "found a keyboard"
		


	def change_state(self,event):
	        hexkey = keymap.convert(event.code);
                if event.value == 0:
                    self.state[4] = 0x00
                else:
                    self.state[4] = hexkey
					
        def gpio_setup(self):
            GPIO.setmode(GPIO.BCM)
            GPIO.setup(Keyboard.A_INPUT_PIN, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
            GPIO.setup(Keyboard.S_INPUT_PIN, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
            GPIO.setup(Keyboard.E_INPUT_PIN, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
            GPIO.setup(Keyboard.Q_INPUT_PIN, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
            GPIO.setup(Keyboard.W_INPUT_PIN, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
            GPIO.setup(Keyboard.OUTPUT, GPIO.OUT)


	#poll for keyboard events
	def event_loop(self):
	    self.gpio_setup()
            event = Event()
            event.code = "KEY_A"
            try:
                 while True:
                    print("in the loop")
                    isPressed = False;
                    if GPIO.input(Keyboard.A_INPUT_PIN):
                         isPressed = True; 
                         event.code = "KEY_A" 
                                            
                    if GPIO.input(Keyboard.S_INPUT_PIN):
                         isPressed = True
                         event.code = "KEY_S" 
                        
                    
                    if GPIO.input(Keyboard.E_INPUT_PIN):
                         isPressed = True
                         event.code = "KEY_E" 
                                             
                    
                    if GPIO.input(Keyboard.Q_INPUT_PIN):
                         isPressed = True
                         event.code = "KEY_Q"
                                             
                    
                    if GPIO.input(Keyboard.W_INPUT_PIN):
                         isPressed = True 
                         event.code = "KEY_W" 
                         
                    if isPressed:
                         event.value = 1
                         event.print_out()
                         GPIO.output(Keyboard.OUTPUT, 1)

                    else:
                         event.value = 0
                         GPIO.output(Keyboard.OUTPUT, 0)
                                               
                    self.change_state(event)
                    self.send_input()
            
            except KeyboardInterrupt:
                GPIO.cleanup()

	#forward keyboard events to the dbus service
   	def send_input(self):
		self.iface.send_keys(0,self.state[4:10]  )



if __name__ == "__main__":

	print "Setting up keyboard"

	kb = Keyboard()

	print "starting event loop"
	kb.event_loop()

