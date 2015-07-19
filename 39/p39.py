from math import sqrt
p=[]
for a in range(1,999):
  for b in range(1,min(a,1001-2*a)):
    c=sqrt(a*a+b*b)
    if c%1==0:
      p.append((a,b,c,a+b+c))
p.sort(key=lambda t:t[3])
v=p[0][3]
l=0
m=0
n=0
for t in p:
  if t[3]!=v:
    if l>m:
      m=l
      n=v
    l=0
    v=t[3]
  l+=1
print(n)
  
