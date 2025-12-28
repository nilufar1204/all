#import wikipedia
import random 
import datetime 
#import pyjoke

print ("I'm a AI, maid by Ariq Azmain who is a student. My name is R.T. ")
while True :
	command = input('ask me : ')
	li2 = ['ask me anything.','bo you have any questions?','how can I halp you? ','ask me more questions'," I'm ready to answer your puestions, ask me "]
	l2 = random.choice(li2)
	if 'time' in command :
		time = datetime.datetime.now().strftime('%I:%M')
		li1 = ['corint time is ',
	'this time is ',
	'now time is ']
		l1 = random.choice(li1)
		print(l1,time)
		print (l2)
	elif 'tall me about' in command:
		lf = command.replace('tall me about ','')
		info1 = wikipedia.summery(lf)
		print(info1)	
	elif 'what is ' in command :
		lf1 = command.replace('what is','')
		info2 = wikipedia.summery(lf1)
		print(info2)	
	else :
		print (" I'm dont know it . plisse  tall again")