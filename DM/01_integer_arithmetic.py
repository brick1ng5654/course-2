import math
# gcd = math.gcd(10,15, 25)
# lcm = math.lcm(112, 96, 64)

# print(lcm)


### ----------------------------------------------------is number prime function
# def is_number_prime(N):
#     i=3
#     if(N==1):
#         print("1")
#         return
#     if(N%2==0):
#         print("составное")
#         return
#     while(i**2<N):
#         if(N%i==0):
#             print("составное")
#             return
#         i+=2
#     print("простое")

# if __name__ == "__main__":
#     number = int(input())
#     is_number_prime(number)

### -----------------------------------------count of prime numbers in range

# def count_of_primes(a, b):
#     array=[]
#     for i in range(a,b+1):
#         flag=0
#         for j in range(2, int(i**0.5)+1):
#             if(i%j==0):
#                 flag=1
#                 break
#         if flag==0: array.append(i)
#     print(len(array))

# if __name__ == "__main__":
#     b=int(input())
#     count_of_primes(2,b)
        
### ---------------------------------------------------------factorization of number and Th. Ferma
# function return 1 if n is prime and 0 if not
def is_number_prime(N):
    i=3
    if(N==1 or N==2):
        return 1
    elif(N%2==0):
        return 0
    while(i**2<N+1):
        if(N%i==0):
            return 0
        i=i+2
    return 1

def ferma(n):
    if n%2==0:
        # print("Entered number should be odd")
        return [2, n//2]
    if(is_number_prime(n) == 1):
        # print("Entered number should not be prime")
        return [1, n]
    flag=0
    x = math.ceil(n**0.5)
    while(flag==0):
        y = x**2 - n
        if y == (round(y**0.5))**2:
            y = round(y**0.5)
            flag = 1
        else: x+=1
    a = max(x,y) - min(x,y)
    b = x+y
    # print(min(x,y), max(x,y))
    return [min(a,b), max(a,b)]

if __name__ == "__main__":
    n = int(input())
    [a, b] = ferma(n)
    print(a, b)
    