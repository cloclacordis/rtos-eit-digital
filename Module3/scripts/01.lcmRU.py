import math
from functools import reduce

# Функция для нахождения НОК двух чисел
def lcm(a, b):
    return abs(a * b) // math.gcd(a, b)

# Функция для нахождения НОК списка чисел
def lcm_multiple(numbers):
    return reduce(lcm, numbers)

# Функция для ввода чисел пользователем и проверки корректности
def get_numbers():
    while True:
        user_input = input("Введите числа через пробел для нахождения НОК: ")
        parts = user_input.strip().split()
        try:
            numbers = [int(p) for p in parts]
            if not numbers:
                raise ValueError("Список чисел пуст.")
            if any(n <= 0 for n in numbers):
                raise ValueError("Все числа должны быть положительными целыми.")
            return numbers
        except ValueError as e:
            print(f"Ошибка ввода: {e}. Попробуйте снова.\n")

# Основная функция
def main():
    numbers = get_numbers()
    result = lcm_multiple(numbers)
    print(f"НОК чисел {numbers} = {result}")

if __name__ == "__main__":
    main()
