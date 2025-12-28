print(" to exit, type\"exit\" ")

while True :
	
	try:
		a = input(' Enter a math : ')
		print(" Ans. : ",eval(a))
	except :
		print (' Enter a valued calculation! ')
	if a == "exit":
		break 