x=x1=3
y=y1=2

while x<1e12:
	x, y = x1*x + 2*y1*y, x1*y + y1*x
	print(x, y)

