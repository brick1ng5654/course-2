from fractions import Fraction

def print_matrix_system(matrix, result):
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            print(matrix[i][j], end=" ")
        print(f" | {result[i]}")
    print("")

def trianguly(matrix, result):
    for j in range(min(len(matrix),len(matrix[0]))-1):
        #Checkpoint_01
        matrix, result = remove_empty_str(matrix, result)
        matrix, result = sort_matrix_for_md(matrix, result)
        if is_solution(matrix,result):
            return None, None
        
        # Triangulate
        base = matrix[j][j]
        for i in range(j+1, len(matrix)):
            aij = Fraction(matrix[i][j])
            dif = Fraction(aij, base)
            result[i] = result[i] - dif*result[j]
            for k in range(0, len(matrix[0])):
                matrix[i][k] = matrix[i][k] - dif*matrix[j][k]

        # Checkpoint_02
        matrix, result = remove_empty_str(matrix, result)
        matrix, result = sort_matrix_for_md(matrix, result)
        if is_solution(matrix,result):
            return None, None
        
    return matrix, result

def triangulate_upper(matrix, result):
    for j in range(min(len(matrix),len(matrix[0]))-1, -1, -1):
        base = matrix[j][j]
        for i in range(j-1, -1, -1):
            aij = Fraction(matrix[i][j])
            dif = Fraction(aij, base)
            result[i] = result[i]-dif*result[j]
            matrix[i][j] = matrix[i][j]-dif*base
        
    return matrix, result

def get_variables(matrix, result):
    variables = []
    for i in range(min(len(matrix), len(matrix[0]))):
        amd = matrix[i][i]
        var = Fraction(result[i], amd)
        variables.append(var)
    
    print("YES")
    variables = list(map(float, variables))
    for i in range(len(variables)):
        print(variables[i], end=" ")

def sort_matrix_for_md(matrix, result):
    for md in range(min(len(matrix), len(matrix[0]))):
        max_elem = abs(matrix[md][md])
        str_with_max_elem = md
        for i in range(md+1, min(len(matrix), len(matrix[0]))):
            if abs(matrix[i][md]) > max_elem:
                str_with_max_elem = i
                max_elem = abs(matrix[i][md])
        matrix[md], matrix[str_with_max_elem] = matrix[str_with_max_elem], matrix[md]
        result[md], result[str_with_max_elem] = result[str_with_max_elem], result[md]

        # print("Sorted by main diag")
        # print_matrix_system(matrix, result)
    return matrix, result

def remove_empty_str(matrix, result):
    new_matrix = []
    new_result = []
    for i in range(len(matrix)):
        if [0]*len(matrix[0]) == matrix[i] and result[i] != 0:
            new_matrix.append(matrix[i])
            new_result.append(result[i])

    new_matrix[::-1]
    new_result[::-1]
    return matrix, result

def is_solution(matrix, result):
    columns = len(matrix)
    for i in range(len(matrix)):
        if [0]*len(matrix[0]) == matrix[i]:
            if result[i] == 0:
                columns-=1
            else:
                print("NO")
                return 1
    
    for i in range(1, min(len(matrix),len(matrix[0]))):
        for j in range(0, i):
            if matrix[i][j]!=0:
                return 0
    
    if columns < len(matrix[0]):
        print("INF")
        return 1
    
    for i in range(len(matrix)):
        for j in range(i+1, len(matrix)):
            if (matrix[i] == matrix[j]) and (result[i]!=result[j]):
                print("NO")
                return 1
            
    for i in range(len(matrix)):
        # print_matrix_system(matrix, result)
        zero_positions_i = {index for index, value in enumerate(matrix[i]) if value == 0}
        non_zero_value_i = next(value for value in matrix[i] if value != 0)
        for j in range(i+1, len(matrix)):
            zero_positions_j = {index for index, value in enumerate(matrix[j]) if value == 0}
            non_zero_value_j = next(value for value in matrix[j] if value != 0)
            if zero_positions_i == zero_positions_j:
                if(result[i] == 0 or result[j] == 0):
                    print("NO")
                    return 1
                elif Fraction(non_zero_value_i, non_zero_value_j) != Fraction(result[i], result[j]):
                    print("NO")
                    return 1

    for i in range(min(len(matrix), len(matrix[0]))):
        if matrix[i][i] == 0:
            if result[i] == 0:
                print("INF")
                return 1
            else:
                print("NO")
                return 1

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

    if len(matrix) == 1 and len(matrix[0])>1:
        if [0]*len(matrix[0]) == matrix[0] and result[0]!=0:
            print("NO")
            return
        if len(matrix[0])-matrix[0].count(0)>1:
            print("INF")
            return


    if len(matrix[0])==1 and len(matrix)>1:
        if is_solution(matrix, result):
            return
        else:
            print("YES")
            print(float(Fraction(result[0], matrix[0][0])))
            return

    matrix, result = trianguly(matrix, result)
    if matrix and result:
        matrix, result = triangulate_upper(matrix, result)
        # print_matrix_system(matrix, result)
        get_variables(matrix, result)

if __name__ == "__main__":
    main()