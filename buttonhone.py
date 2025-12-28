import datetime 
import time 

print("to back, enter : b & go to home, enter: h")

class buttonphone:
	def home():
		print("1.masage")
		print("2.call list")
		print("3.file")
		print("4.tools")
	def masage():
		i = 'ariq azmain. '
		s = ''
		w = []
		print(" 1.Inbox")
		print("2.Sentbox")
		print("3.writ message ")
	
	def callli():
		ac = ""
		mc = ""
		adc = []
		print(" 1.All call")
		print("2.Misaed call")
		print("3.add call")
	def file():
		fil = ''
		adf = ''
		print("1.add fill")
		print("2.open file ")
	def tool():
		print(" 1.Clock")
		print("2.Calculator")
	
	def mainfun():
		home()
		b = input('control: ')
		if b == "1":
			masage()
			if b == "1":
				print(i)
			if b == "2":
				print(" s")
			if b == "3":
					msg = input(" enter your massg : ")
					s.append(msg)
buttonphone.mainfun()							