from math import factorial

def digitsSum(n):
    s = 0
    while n:
        s += n%10
        n //= 10
    return s

print(digitsSum(factorial(100)))
