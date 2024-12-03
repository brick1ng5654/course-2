import math

# def change_numeric_system(number, o_sys, t_sys):
#     d_number = 0
#     str_number = str(number)
#     result = ''
#     array = []
#     tarray = []
#     flag = 0
    
#     for i in str_number: # split number on parts
#         array.append(int(i))
#     # convert to dec system
#     if(o_sys!=10):
#         for i in range(len(array)):
#             exp = len(array)-i-1
#             d_number += array[i]*o_sys**exp
#     else: d_number = number
#     # convert to given system
#     while(flag==0):
#         q = d_number // t_sys
#         r = d_number % t_sys
#         d_number = q
#         if(q==0): flag = 1
#         tarray.append(r)
#     tarray = tarray[::-1]
#     for i in tarray:
#         result+=str(i)
#     print(result)

# if __name__ == "__main__":
#     number, sys1, sys2 = map(int, input().split())
#     change_numeric_system(number, sys1,sys2)

### factorial numeric system

# def factorial(n):
#     if(n<0): return 0
#     elif(n<2): return 1
#     else:
#         number = n * factorial(n-1)
#     return number

# def fac_to_dec_sys(number):
#     result = 0
#     array = []
#     for i in str(number):
#         array.append(int(i))
#     for i in range (len(array)):
#         mul = len(array)-i
#         result+=array[i]*factorial(mul)
#     return result

# if __name__ == "__main__":
#     number = int(input())
#     result = fac_to_dec_sys(number)
#     print(result)

print(math.gcd(2603, 1995))