### ---------------------------------------------------------- cont fraq

# def cont_fraq(a, b):
#     cont_fraq = []
#     while(b!=0):
#         cont_fraq.append(a//b)
#         a, b = b, a%b
#     return cont_fraq

# if __name__ =="__main__":
#     a, b = map(int, input().split())
#     array = cont_fraq(a,b)
#     print("["+str(array[0])+";", end="")
#     for i in range(1, len(array)-1):
#         print(str(array[i])+",",end="")
#     print(str(array[len(array)-1])+"]",end="")

### --------------------------------------------------------- from cofs to cont_fraq

def from_cofs_to_cf(n, cofs):
    num, denum = 1, cofs[-1]
    for cof in reversed(cofs[:-1]):
        num, denum = denum, cof*denum+num
    return denum, num

if __name__ == "__main__":
    n = int(input())
    cofs = list(map(int, input().split()))
    num, denum = from_cofs_to_cf(n, cofs)
    print(num, denum)