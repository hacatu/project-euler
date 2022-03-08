from sympy import symbols, Sum, floor

n, x, y, k, h = symbols("n x y k h")
hnxykh = 1
hnxkh = Sum(hnxykh, (y, 0, n - 2*k - h - x)).doit()
hnkh = Sum(hnxkh, (x, k + 2*h, n - 2*k - h)).doit()
hnk = Sum(hnkh, (h, 0, floor(n/3) - k)).doit()
hn = Sum(hnk, (k, 1, floor(n/3))).doit()
print(Sum(hn, (n, 3, 12345)).doit())

