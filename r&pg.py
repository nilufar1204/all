import pygame 
import random
from time import sleep  
pygame.init()
while True :
	r = random.randrange(111,255)
	g = random.randrange(111,255)
	b = random.randrange(111,255)
	sleep(0.2)
	
	x = random.randrange(0,720)
	y = random.randrange(0,1600)
	w = pygame.display.set_mode((0,0), pygame.FULLSCREEN )
	s = random.randrange(50,115)
	e = pygame.draw.circle(w,(r,g,b),(305,800),s,s)
	
	pygame.display.update()
	