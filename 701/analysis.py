from math import log, exp

xys = [(1,.5),(2,1.875),(3,3.64453125),(4,5.76487732),(5,8.14696828),(6,10.74254251),(7,13.51099836),(8,16.41834434),(9,19.43611591)]

b = (len(xys)*sum(log(x)*log(y) for (x, y) in xys) - sum(log(x) for (x, _) in xys)*sum(log(y) for (_, y) in xys))/(len(xys)*sum(log(x)**2 for (x, _) in xys) - sum(log(x) for (x, _) in xys)**2)
a = (sum(log(y) for (_, y) in xys) - b*sum(log(x) for (x, _) in xys))/len(xys)

print("y={}*x^{}".format(exp(a), b))

