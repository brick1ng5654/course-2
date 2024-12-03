from math import gcd

# # last symbol of number 
# # number = int(input())
# # exp = int(input())
# number = 1998**1998+1999*19999
# print((number)%3)

# __________ mersenne prime number

# s = "2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 107, 127, 521, 607, 1279, 2203, 2281, 3217, 4253, 4423, 9689, 9941, 11 213, 19 937, 21 701, 23 209, 44 497, 86 243, 110 503, 132 049, 216 091, 756 839, 859 433, 1 257 787, 1 398 269, 2 976 221, 3 021 377, 6 972 593, 13 466 917, 20 996 011, 24 036 583, 25 964 951, 30 402 457, 32 582 657, 37 156 667, 42 643 801, 43 112 609, 57 885 161, 74 207 281, 77 232 917, 82 589 933, 136 279 841"
# mn = [x.replace(" ", "") for x in s.split(", ")]

# number = int(input())

# print(int(mn[number-1]))

# ___________ lin compare

# for x in range(1, 100):
#     if (1703*x-1)%12 == 0:
#         print(x)

# for x in range(1, 10000):
#     if ((2021*x-40)%41 == 0 and (1999*x-97)%98==0):
#         print(x)

# for x in range(1, 10000):
#     if (x-7)%8==0:
#         if (x+1)%11==0:
#             if (x-3)%15==0:
#                 print(x)
#                 break

# for x in range(1, 100000):
#     if (x-7)%40==0:
#         if (x-17)%31==0:
#             if (x-9)%29==0:
#                 if (x-11)%13==0:
#                     print(x)

# __________

def extended_gcd(a, b):
    """ Расширенный алгоритм Евклида для нахождения обратного элемента. """
    if b == 0:
        return a, 1, 0
    gcd, x1, y1 = extended_gcd(b, a % b)
    x = y1
    y = x1 - (a // b) * y1
    return gcd, x, y

def mod_inverse(a, m):
    """ Нахождение обратного элемента по модулю. """
    gcd, x, _ = extended_gcd(a, m)
    if gcd != 1:
        raise ValueError("Обратного элемента не существует")
    return x % m

def chinese_remainder_theorem(c, m):
    """ Реализация Китайской теоремы об остатках. """
    M = 1
    for mod in m:
        M *= mod

    x = 0
    for i in range(len(c)):
        Mi = M // m[i]
        Mi_inverse = mod_inverse(Mi, m[i])
        x += c[i] * Mi * Mi_inverse

    return x % M

# Ввод данных
n = int(input().strip())
c = list(map(int, input().strip().split()))
m = list(map(int, input().strip().split()))

# Вычисление и вывод результата
result = chinese_remainder_theorem(c, m)
print(result)
