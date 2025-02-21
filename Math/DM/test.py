from datetime import datetime
from origin import value_count, value_area, value_distance, bias, step


def load_file(filename):
    array = []
    try:
        with open(filename, 'r') as f:
            for line in f:
                array_line = line.strip().split(';')
                array_line = [int(x) for x in array_line]
                array.append(array_line)
    except FileNotFoundError:
        print(f"Файл {filename} не найден.")
    except Exception as e:
        print(f"Произошла ошибка при чтении файла: {e}")
    return array

def predict_function(count, area, distance):
    result = value_count*count+value_area*area-value_distance*distance
    return result

def sign(x):
    return 1 if x >= 0 else -1

def lse_function(data):
    n = len(data)
    sum = 0
    for i in range(n):
        dif = (data[i][-1]-predict_function(data[i][0], data[i][1], data[i][2]))**2
        sum+=dif
    result = sum/n
    now = datetime.now()
    time_str = now.strftime("%Y-%m-%d %H:%M:%S.%f")
    print(time_str+" /// "+str(result))
    return result

def partial_der(data, row):
    sum = 0
    n = len(data)
    for i in range(n):
        mltply = data[i][row-1]*(data[i][-1]-predict_function(data[i][0], data[i][1], data[i][2]))
        sum+=mltply
    result = (-2/n)*sign(val_array[row-1])*sum
    return result

def bias_der(data):
    sum = 0
    n = len(data)
    for i in range(n):
        mltply = (data[i][-1]-predict_function(data[i][0], data[i][1], data[i][2]))
        sum+=mltply
    result = (-2/n)*sum
    return result

def grad(data):
    global value_count, value_area, value_distance, bias, array
    value_count = value_count - step*partial_der(data, 1)
    value_area = value_area - step*partial_der(data, 2)
    value_distance = value_distance - step*partial_der(data, 3)
    bias = bias - step*bias_der(data)
    with open("Math/DM/config.py", "w") as f:
        f.write(f"value_count = {value_count}\n")
        f.write(f"value_area = {value_area}\n")
        f.write(f"value_distance = {value_distance}\n")
        f.write(f"bias = {bias}\n")
        f.write(f"step = {step}\n")


value_count = 0.1
value_area = 0.1
value_distance = -0.1
bias = 0
step = 0.00001
val_array = [value_count, value_area, value_distance, bias, step]

if __name__ == "__main__":
    data = load_file('MATH/DM/data.txt')
    print(lse_function(data))
    while(lse_function(data)>100000):
        grad(data)
