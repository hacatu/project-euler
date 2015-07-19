from itertools import permutations
s=0
def int(n):
  r=0
  t=1
  for d in n[::-1]:
    r+=d*t
    t*=10
  return r
for n in permutations(range(0,10)):
  n=int(n)
  p=n
  a=p%1000
  if a%17:
    continue
  p//=10
  a=p%1000
  if a%13:
    continue
  p//=10
  a=p%1000
  if a%11:
    continue
  p//=10
  a=p%1000
  if a%7:
    continue
  p//=10
  if p%5:
    continue
  p//=10
  a=p%1000
  if a%3:
    continue
  p//=10
  if p%2:
    continue
  print(n)
  s+=n
print(s)
