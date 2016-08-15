import time
import story

def PrintMyFunc():
	print "Hello MyFunc !"
	a = story.SpLog()
	a.dbg()
	a.dbg()
	a.dbg()
	a.dbg()
	#while True:
	#	time.sleep(3)
	#	print "Current time: " + datetime.datetime.now()

def Multiply(x, y):
	return x * y
