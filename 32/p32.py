from itertools import permutations
pandas=set()
def list2int(l):
    sum=0
    for place,digit in enumerate(l[::-1]):
        sum+=digit*10**place
    return sum
for perm in permutations(range(1,10)):
    a=perm[0]
    b=list2int(perm[1:5])
    c=list2int(perm[5:])
    if a*b==c:
        pandas.add(c)
    a=list2int(perm[0:2])
    b=list2int(perm[2:5])
    c=list2int(perm[5:])
    if a*b==c:
        pandas.add(c)
print(sum(pandas))

