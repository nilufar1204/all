import turtle
import colorsys
t = turtle.Turtle()
s = turtle.Screen().bgcolor('black')
t.speed(110)
n = 70
h = 0

for i in range (360):
	C = colorsys.hsv_to_rgb(h, 1, 0.8)

	h+= 1/n

	t.color(C)

	t.left(1)

	t.fd(1)

	for j in range (2):

		t.left(2)

		t.circle (100)