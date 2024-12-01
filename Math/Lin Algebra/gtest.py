# test 11 failed
from fractions import Fraction

def trianguly(matrix, result):
    for j in range(len(matrix)):

        max_elem = Fraction(matrix[0][j])
        str_with_max_elem = -1
        for g in range(j, min(len(matrix), len(matrix[0]))):
            if abs(matrix[g][j])>max_elem:
                max_elem = Fraction(abs(matrix[g][j]))
                str_with_max_elem = g
        if max_elem>0:
            matrix[j], matrix[str_with_max_elem] = matrix[str_with_max_elem], matrix[j]
            result[j], result[str_with_max_elem] = result[str_with_max_elem], result[j]
            print_matrix_system(matrix, result)

            base = matrix[j][j]
            for i in range(j+1, len(matrix)):
                aij = Fraction(matrix[i][j])
                dif = Fraction(aij, base)
                result[i] = result[i] - dif*result[j]
                for k in range(0, len(matrix[0])):
                    matrix[i][k] = matrix[i][k] - dif*matrix[j][k]
                    print_matrix_system(matrix, result)
    # print(matrix)
        else:
            print("smthing else")
    return matrix, result

def remove_empty_str(matrix, result):
    new_matrix = []
    new_result = []
    for i in range(len(matrix)):
        if not([0]*len(matrix[0]) == matrix[i] and result[i]==0):
            new_matrix.append(matrix[i])
            new_result.append(result[i])
    new_matrix[::-1]
    new_result[::-1]
    print_matrix_system(new_matrix, new_result)
    return new_matrix, new_result

def check_for_no_solution(matrix, result):
    for i in range(len(matrix)):
        if [0]*len(matrix[0]) == matrix[i] and result[i]!=0:
            return 1

def check_for_inf_solution(matrix, result):
    print("---")
    print_matrix_system(matrix, result)
    matrix, result = remove_empty_str(matrix, result)
    if len(matrix) < len(matrix[0]):
        return 1

def solution(matrix, result):
    variables = []

    matrix, result = remove_empty_str(matrix, result)

    print("-------------")
    print_matrix_system(matrix, result)

    if check_for_no_solution(matrix, result):
        print("NO")
        return
    
    if check_for_inf_solution(matrix, result):
        print("INF")
        return

    for i in range(len(matrix)):
        # что в правой стороне уравнения
        right_side = Fraction(result[len(result)-i-1])
        for j in range(len(matrix[0])-1, len(matrix)-i-1, -1):
            right_side-=Fraction(matrix[len(matrix)-i-1][j]*variables[len(matrix)-j-1])
        
        var = Fraction(right_side, matrix[len(matrix)-i-1][len(matrix)-i-1])
            
        variables.append(var)

    # print_matrix_system(matrix, result)
    return variables


def print_matrix(matrix):
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            print(matrix[i][j], end=" ")
        print("")
    print("")

def print_vector(r_vector):
    for i in range(len(r_vector)):
        print(r_vector[i])

def print_matrix_system(matrix, result):
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            print(matrix[i][j], end=" ")
        print(f" | {result[i]}")
    print("")



def main():
    # n - количество уравнений
    # m - количество переменных
    n, m = map(int, input().split())
    matrix = [[0]*m for i in range(n)]
    result = []
    line = []
    for i in range(n):
        line = list(map(Fraction, input().split()))
        for j in range(m):
            matrix[i][j] = line[j]
        result.append(line[-1])

    # preform = check_for_repeat_str(matrix, result)
    # if (preform):
    #     print("NO")
    #     return


    matrix, result = trianguly(matrix, result)
    if matrix and result:
        matrix, result = remove_empty_str(matrix, result)
        print_matrix_system(matrix, result)
        variables = list(map(float, solution(matrix, result)))
        variables = variables[::-1]

        for i in range(len(variables)):
            print(variables[i], end=" ")

if __name__ == "__main__":
    main()