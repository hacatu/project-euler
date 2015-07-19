#euler 95

import time
t0 = time.time()

memo = {1:[None]}  #memo is hash of numbers, mapped to their divisors

topLimit = 1000000

for i in range(2, topLimit+1):  #initialize 1 as divisor of all numbers
    memo[i] = [1]

for j in range(2, (topLimit+1)//2):    #use a sieve to update memo with divisors
    step = j
    counter = j + j
    while counter < topLimit+1:
        memo[counter].append(j)
        counter += j

memoSum = {1:None}  # this is hash of sum of divisors

for i in range(2, topLimit+1):
    memoSum[i] = sum(x for x in memo[i])

def insertN(n, listA):   #inserts n into beginning of list
    output = [n]
    for i in listA:
        output.append(i)
    return output

def createChain(n):
    chain = []
    temp = memoSum[n]
    while temp not in chain:
        if temp == 1:
            print("chain went to prime", len(chain), chain)
            return None
        elif temp > topLimit:
            print("chain exceeded limit", len(chain), chain)
            return None
        elif temp == n:
            return insertN(n,chain) 
        chain.append(temp)
        temp = memoSum[temp]
    print("chain does not include start", len(chain), chain)
    return None

longestChain = []
memoMin = {}  #hash with index as length of amicable chain

for x in range(100):
    memoMin[x] = []  #initialize a container list in memoMin
"""
for i in range(2, topLimit+1):
    temp = createChain(i)
    if temp != None:
        if len(temp) > len(longestChain):
            print("longest amicable chain so far is ", temp, len(temp))
            longestChain = temp
            smallest = min(i, min(temp))
            memoMin[len(temp)].append(smallest)
            answerLength = len(temp)
            
print("length of longest amicable chain was ", answerLength)
print("smallest member of this chain was ", min(memoMin[answerLength]))

    
t1 = time.time()
total = t1-t0
print(total, "seconds")
"""
temp = createChain(7524)
print(len(temp), temp)

