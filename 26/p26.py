#!/usr/bin/env python
m=0
n=1
for d in range(3,1000,2):
	c=d
	i=9
	r=1
	while c%5==0:
		c/=5
	while i%c:
		i=10*i+9
		r+=1
	if i>m:
		m=i
		n=d
	print(`d`+':'+`r`)
print(n)
