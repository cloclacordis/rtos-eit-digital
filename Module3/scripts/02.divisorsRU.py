def get_divisors(n):
    if not isinstance(n, int) or n <= 0:
        raise ValueError("Введите положительное целое число")

    divisors = []
    for i in range(1, int(n**0.5) + 1):
        if n % i == 0:
            divisors.append(i)
            if i != n // i:
                divisors.append(n // i)
    return sorted(divisors)

# Пример использования:
number = int(input("Введите положительное целое число: "))
print("Делители:", get_divisors(number))
