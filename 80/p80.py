from sqrt import *

s = 0
for i in range(2, 100):
	if i in [4, 9, 16, 25, 36, 49, 64, 81]:
		continue
	l = 2
	if i < 10:
		l = 1
	r = digitSum(isqrt(i*10**(2*(100 - l))))
	s += r
	print(i, r)
	
print(s)
