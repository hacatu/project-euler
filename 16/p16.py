def digitsSum(n):
    s = 0
    while n:
        s += n%10
        n //= 10
    return s

print(digitsSum(2**1000))
