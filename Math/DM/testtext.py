n=int(input())
k=int(input())
a=[1]*n
cur=0
while(sum(a)!=1):
    print(a)
    print(cur)
    cur=cur%len(a)
    if a[cur]!=0:
        #найти живого! следующего человека
        eliminated=(cur+k-1)%len(a)
        a[eliminated]=0
    cur+=1@
print(a)
for i in range(len(a)):
    if a[i]==1:
        print(i+1)