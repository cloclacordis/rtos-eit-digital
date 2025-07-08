def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

# Ввод от пользователя
try:
    a = int(input("Введите первое число: "))
    b = int(input("Введите второе число: "))
    result = gcd(a, b)
    print(f"НОД({a},{b}) = {result}")
except ValueError:
    print("Ошибка: введите целые числа.")
