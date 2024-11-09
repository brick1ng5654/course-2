import math

### --------------------------------------------- defualt euclidean algorithm

# def euc_alg(a, b):
#     if(a==0 and b==0): return -1
#     if(b==0): return a
#     return euc_alg(b, a%b)
    
# if __name__ == "__main__":
#     a, b = map(int, input().split())
#     result = euc_alg(a,b)
#     print(result)

### ----------------------------------------------- is entered number fibonnaci?

# def is_numb_fib(n):
#     result = 0
#     a = 1
#     b = 1
#     while(result < n):
#         result = a+b
#         a = b
#         b = result
#     if(result == n): return 1
#     else: return 0

# if __name__ == "__main__":
#     number = int(input())
#     print(is_numb_fib(number))

### --------------------------------------------------- diofant equation
import math

def dio(a, b, c):
    gcd = math.gcd(a,b)
    if(c%gcd!=0): print ("Нет решений")
    for x in range(1, 150):
        for y in range(-150, 150):
            if a*x+b*y == c:
                print(x, y)
                return

if __name__ == "__main__":
    a, b, c = map(int, input().split())
    dio(a, b, c)
    