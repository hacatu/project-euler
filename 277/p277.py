import numpy as np

T_D = np.array([[1, 0], [0, 3]], dtype=np.int64)
T_U = np.array([[4, 2], [0, 3]], dtype=np.int64)
T_d = np.array([[2, -1], [0, 3]], dtype=np.int64)

sns = "UDDDUdddDDUDDddDdDddDDUDDdUUDd"

T = np.eye(2, dtype=np.int64)

for l in sns:
	if l == 'D':
		T = T_D@T
	elif l == 'U':
		T = T_U@T
	elif l == 'd':
		T = T_d@T

r = -int(T[0,1])*pow(int(T[0,0]), -1, 3**len(sns))%3**len(sns)
m = (10**15 - r - 1)//3**30+1
print(r+3**30*m)

