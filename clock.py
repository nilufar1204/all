import datetime 
while True :
	t = datetime.datetime.now()
	s = t.strftime('%I:%M:%S')
	if s == '04:19:00':
		break 
	print(s)
