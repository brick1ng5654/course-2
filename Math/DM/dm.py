from sympy import symbols, expand

# Поле Z5
Z5 = 5

# Данные
points = [0, 1, 2, 3, 4]
values = [1, 2, 0, 4, 1]

# Лагранжев интерполяционный полином
x = symbols('x')
n = len(points)
P = 0

for i in range(n):
    # Лагранжев базис
    L = 1
    for j in range(n):
        if i != j:
            L *= (x - points[j]) * pow(points[i] - points[j], -1, Z5)
    L = expand(L) % Z5
    P += values[i] * L

# Приведение коэффициентов в поле Z5
P = expand(P) % Z5
print("Восстановленный многочлен r(x):", P)

# Извлечение исходного сообщения m(x)
# Разделение P(x) на порождающий многочлен g(x) = x^2 + 1
g = x**2 + 1
m, remainder = divmod(P, g)
m = expand(m) % Z5
remainder = expand(remainder) % Z5

print("Исходное сообщение m(x):", m)
print("Остаток (для проверки):", remainder)
